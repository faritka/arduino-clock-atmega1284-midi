#include "ClockWeather.h"

ClockForecastDay::ClockForecastDay()
{
    initValues();
}

void ClockForecastDay::initValues()
{
    tempMaxC = tempMaxF = weatherCode = -255;
    symbol = date = 0;

    memset(description, 0, sizeof(description));
}

time_t ClockForecastDay::dateToUnixtime(char *date)
{
    time_t unixTime = 0;
    int i = 0;
    tmElements_t tm;

    //the date is "2013-01-01"
    const char s[2] = "-";
    char *token;

    tm.Hour = 0;
    tm.Minute = 0;
    tm.Second = 0;

    // get the first token
    token = strtok(date, s);

    // walk through the other tokens
    for (i = 0; i < 3; i++) {
        switch (i) {
            case 0:
                tm.Year = atoi(token) - 1970;
                break;
            case 1:
                tm.Month = atoi(token);
                break;
            case 2:
                tm.Day = atoi(token);
                break;
        }

        token = strtok(NULL, s);
    }

    unixTime = makeTime(tm);
#ifdef DEBUG
    Serial.print(F("Forecast time: "));
    Serial.println(unixTime);
#endif

    return unixTime;
}


/**
 * Constructor
 */
ClockWeather::ClockWeather(ClockConfig *clockConfig) : config(clockConfig)
{
    updateCounter = lastUpdate = lastUnsuccessfulUpdate = 0;
}

const char ClockWeather::temperatureScaleNames[2][11] = {"Celsius", "Fahrenheit"};

void ClockWeather::update(time_t currentTime)
{
    //the number of tries before giving up for the WEATHER_UPDATE_HOURS
    const byte numberOfTries = 5;
    //when to try to connect again after unsuccessful tries
    const unsigned long tryAgain = 30 * SECS_PER_MIN;

    //no Internet
    if (!config->getInternetEnabled() || !config->getWeatherEnabled()) {
        return;
    }

    //if all atempts were unsuccessful, try to connect after some time again
    if (updateCounter && (currentTime - lastUnsuccessfulUpdate) > tryAgain) {
        updateCounter = 0;

        //clear the forecast to not show incorrect data
        for (byte i = 0; i <= 1; i++) {
            forecast[i].initValues();
        }
    }

    if ((updateCounter < numberOfTries) 
        && (!lastUpdate 
            || ((currentTime - lastUpdate) > WEATHER_UPDATE_HOURS * SECS_PER_HOUR))) {

        if (getFromServer(currentTime)) {
            //save the last forecast update time
            lastUpdate = currentTime;
            //sets the counter to zero because the try was successful
            updateCounter = 0;
#ifdef DEBUG
            Serial.print(F("Weather forecast was updated at: "));
            Serial.println(lastUpdate);
#endif
        }
        else {
#ifdef DEBUG
            Serial.println(F("The weather forecast update failed"));
#endif
            updateCounter++;
            lastUnsuccessfulUpdate = currentTime;
        }
    }
}

boolean ClockWeather::getFromServer(time_t currentTime)
{
    const char site[] = "api.worldweatheronline.com";
    //wait for this time for the data
    const unsigned long readTimeout = 30000UL;
    unsigned long startTime = millis();
    //the number of tries after which try to reboot WiFly
    const byte rebootTry = 2;


    if (!strlen(config->getWeatherApiKey())) {
#ifdef DEBUG
        Serial.println(F("Weather API key is not defined"));
#endif
        return false;
    }

    if (!strlen(config->getWeatherLocation())) {
#ifdef DEBUG
        Serial.println(F("Weather location is not defined"));
#endif
        return false;
    }

    //connect to the internet
    ClockInternet internet(config);
    WiFly wifi = internet.connect();

    //if not associated, the connection has failed 
    if (!wifi.isAssociated()) {
        return false;
    }

    //the old connection is active, close it
    if (wifi.isConnected()) {
        wifi.close();
    }

    //connect to the webserver
    if (wifi.open(site, 80)) {
#ifdef DEBUG
        Serial.print(F("Connected to "));
        Serial.println(site);
#endif

        //send the request
        //the 2-day forecast
        wifi.print(F("GET /free/v1/weather.ashx?q="));
        //the ZIP code or the city name
        wifi.print(ClockInternet::urlencode(config->getWeatherLocation()));
        wifi.print(F("&key="));
        wifi.print(config->getWeatherApiKey());
        wifi.println(F("&format=json&num_of_days=3&cc=no HTTP/1.0"));
        wifi.print(F("Host: "));
        wifi.println(site);
        wifi.println();
    }
    else {
#ifdef DEBUG
        Serial.print(F("Failed to connect to the weather feed server: "));
        Serial.println(site);
#endif

        //try to reboot WiFly
        if (updateCounter == rebootTry) {
            wifi.reboot();
#ifdef DEBUG
            Serial.println(F("WiFly was rebooted"));
#endif
            delay(10000);
        }

        return false;
    }

    //read the response from the server
    //skip the HTTP headers
    if (!wifi.match(F("\r\n\r\n"), readTimeout)) {
        return false;
    }

    bool jsonFound = false;
    bool jsonParsed = false;
    //check if the JSON response is available
    while ((millis() - startTime) < readTimeout) {
        if (wifi.available() > 0) {
            char ch = wifi.peek();
            //look for the JSON starting {
            if (!jsonFound && (ch != '{')) {
                wifi.read();

                continue;
            }
            else {
                jsonFound = true;
                jsonParsed = parseJson(&wifi, currentTime);
                break;
            }
        }
    }

    return jsonParsed;
}

boolean ClockWeather::parseJson(Stream *serialStream, time_t currentTime) 
{
    char* value;
    int code = 0;

    //the stream to read the JSON data
    aJsonStream jsonStream(serialStream);

    aJsonObject *root = aJson.parse(&jsonStream);

    if (!root) {
#ifdef DEBUG
        Serial.println(F("No root element"));
#endif
        return false;
    }

    aJsonObject *data = aJson.getObjectItem(root, "data"); 
    if (!data) {
#ifdef DEBUG
        Serial.println(F("No data element"));
#endif
        return false;
    }

    aJsonObject *weather = aJson.getObjectItem(data, "weather"); 
    if (!weather) {
#ifdef DEBUG
        Serial.println(F("No weather element"));
#endif
        return false;
    }

    //get the forecast for three days, including today
    for (byte i = 0; i <= 2; i++) {
#ifdef DEBUG
        Serial.println(F("Getting the forecast"));
#endif
        aJsonObject *dayForecast = aJson.getArrayItem(weather, i);
        if (!forecast) {
#ifdef DEBUG
            Serial.println(F("No weather day"));
#endif
            return false;
        }

        aJsonObject *tempMaxCObject = aJson.getObjectItem(dayForecast, "tempMaxC"); 
        aJsonObject *tempMaxFObject = aJson.getObjectItem(dayForecast, "tempMaxF"); 
        aJsonObject *tempMinCObject = aJson.getObjectItem(dayForecast, "tempMinC"); 
        aJsonObject *tempMinFObject = aJson.getObjectItem(dayForecast, "tempMinF"); 
        aJsonObject *weatherCodeObject = aJson.getObjectItem(dayForecast, "weatherCode"); 

        aJsonObject *weatherDescObject = aJson.getObjectItem(dayForecast, "weatherDesc"); 
        aJsonObject *weatherDescValueObject = aJson.getObjectItem(aJson.getArrayItem(weatherDescObject, 0), "value");

        aJsonObject *dateObject = aJson.getObjectItem(dayForecast, "date");

        if (!tempMaxCObject || !tempMaxFObject || !tempMinFObject || 
            !tempMinCObject || !weatherCodeObject || !weatherDescObject || !dateObject) {
            //clean the memory
            aJson.deleteItem(root);
#ifdef DEBUG
            Serial.println(F("No temperature found"));
#endif
            return false;
        }

        forecast[i].tempMaxC = atoi(tempMaxCObject->valuestring);
        forecast[i].tempMaxF = atoi(tempMaxFObject->valuestring);
        forecast[i].tempMinC = atoi(tempMinCObject->valuestring);
        forecast[i].tempMinF = atoi(tempMinFObject->valuestring);
        forecast[i].weatherCode = atoi(weatherCodeObject->valuestring);

        strncpy(forecast[i].description, weatherDescValueObject->valuestring, 
            sizeof(forecast[i].description) - 1);

        forecast[i].date = forecast[i].dateToUnixtime(dateObject->valuestring);    
#ifdef DEBUG
        Serial.print(F("The weather forecast: "));
        Serial.println(forecast[i].tempMaxC);
        Serial.println(forecast[i].tempMaxF);
        Serial.println(forecast[i].tempMinC);
        Serial.println(forecast[i].tempMinF);
        Serial.println(forecast[i].weatherCode);
        Serial.println(forecast[i].description);
#endif

        //get the weather symbol
        defineWeatherSymbol(forecast[i]);
    }

    //clean the memory
    aJson.deleteItem(root);

    return true;
}


void ClockWeather::defineWeatherSymbol(ClockForecastDay &forecastDay)
{
    //weather codes are here: http://www.worldweatheronline.com/feed/wwoConditionCodes.txt

    switch (forecastDay.weatherCode) {
        //sunny
        case 113:
            forecastDay.symbol = 128;
            break;
        //light rain
        case 176:
        case 293:
        case 296:
        case 311:
        case 353:
        case 386:
            forecastDay.symbol = 129;
            break;
        //moderate rain
        case 299:
        case 302:
        case 356:
        case 389:
            forecastDay.symbol = 130;
            break;
        //heavy rain
        case 305:
        case 308:
        case 314:
        case 359:
            forecastDay.symbol = 131;
            break;
        //cloudy
        case 119:
            forecastDay.symbol = 132;
            break;
        //partly cloudy
        case 116:
            forecastDay.symbol = 133;
            break;
        //overcast
        case 122:
            forecastDay.symbol = 138;
            break;
        //mist
        case 143:
            forecastDay.symbol = 136;
            break;
        //drizzle
        case 185:
        case 263:
        case 266:
        case 281:
        case 284:
            forecastDay.symbol = 134;
            break;
        //snow
        case 179:
        case 227:
        case 323:
        case 326:
        case 329:
        case 332:
        case 335:
        case 338:
        case 368:
        case 371:
        case 392:
        case 395:
            forecastDay.symbol = 135;
            break;
        //fog    
        case 248:
        case 260:
            forecastDay.symbol = 137;
            break;

        default:
            forecastDay.symbol = 0;
    }
}


