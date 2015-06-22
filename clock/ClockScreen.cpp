#include "ClockScreen.h"

const char ClockScreen::noWeatherMessage[] PROGMEM = "No Frcst";

/**
 * Constructor
 */
ClockScreen::ClockScreen(ClockConfig *clockConfig, ClockLightSensor *clockLightSensor) 
    : config(clockConfig), lightSensor(clockLightSensor)
{
    //init the dot-matrix screen
    dotMatrix = new ht1632c(&PORTD, DISPLAY_DATA_INPUT, DISPLAY_WRITE_DATA, 
        DISPLAY_SELECT_CLOCK, DISPLAY_SELECT_SIGNAL, GEOM_32x16, 2);

    // Start the I2C interface
    Wire.begin();

    // clear the screen
    dotMatrix->clear();

    // set the brightness level
    setBrightness();
}

const char ClockScreen::colorNames[5][7] = {"Black", "Green", "Red", "Orange", "Multi"};
const byte ClockScreen::colorIds[5] = {BLACK, GREEN, RED, ORANGE, MULTICOLOR};


void ClockScreen::showTime(time_t currentTime)
{
    char tmp[6];
    memset(tmp, 0, sizeof(tmp));

    // set the brightness level
    setBrightness();

    //12 hour format
    if (config->getHourFormat() == 1) {
        snprintf(tmp, sizeof(tmp), "%2u:%02u", hourFormat12(currentTime), minute(currentTime));
    }
    //24 hour format
    else {
        snprintf(tmp, sizeof(tmp), "%2u:%02u", hour(currentTime), minute(currentTime));
    }

    //set the font for the digits
    dotMatrix->setfont(config->getTimeFont());

    byte len = strlen(tmp);
    //coordinate x
    byte x = 0;

    for (byte i = 0; i < len; i++) {
        dotMatrix->putchar(x, 7, tmp[i], config->getColor());

        //move the delimiter closer to save the space
        if (tmp[i] == ':') {
            x += 5;
        }
        else {
            x += 7;
        }
    }

    dotMatrix->sendframe();
}

void ClockScreen::showDate(time_t currentTime)
{
    char tmp[6];
    //coordinate x
    byte x = 17;
    byte y = 0;
    byte fontWidth = 4;

    memset(tmp, 0, sizeof(tmp));

    // set the brightness level
    setBrightness();

    //format the date
    snprintf(tmp, sizeof(tmp), "%s%2u", ClockTime::getWeekdayName(weekday(currentTime)), day(currentTime));

    //set the font
    dotMatrix->setfont(FONT_4x6CL);

    for (byte i = 0; i < strlen(tmp); i++) {
        dotMatrix->putchar(x, y, tmp[i], config->getColor());
        //the size of the font
        x += fontWidth;
    }

    dotMatrix->sendframe();
}


void ClockScreen::showWeather(ClockWeather *clockWeather, time_t currentTime, bool fullScreen)
{
    char tmp[5];

    //coordinate x
    byte x = 0;
    byte y = 0;
    byte fontWidth = 4;
    int maxTemperature = 0;
    int minTemperature = 0;
    //for which day get the forecast: 0 - today, 1 - tomorrow
    byte forecastDay = 0;
    //is the current time after midnight
    bool isOverMidnight = false;

    //after the forecast hour show the forecast for tomorrow
    if (hour(currentTime) >= config->getWeatherForecastHour()) {
        forecastDay = 1;
    }    
    //after midnight show the next day too
    else if (currentTime >= clockWeather->forecast[1].date) {
        forecastDay = 1;
        isOverMidnight = true;
    }

    // set the brightness level
    setBrightness();
    //set the font
    dotMatrix->setfont(FONT_4x6CL);

    memset(tmp, 0, sizeof(tmp));

    if (!fullScreen) {
        //get the maximum forecast temperature
        if (config->getWeatherTemperatureScale() == WEATHER_CELSIUS) {
            maxTemperature = clockWeather->forecast[forecastDay].tempMaxC;
        }
        else {
            maxTemperature = clockWeather->forecast[forecastDay].tempMaxF;
        }

        //don't show the undefined weather
        if (maxTemperature != -255) {
            //no space for the symbol, it's too hot anyway
            if (maxTemperature >= 100) {
                snprintf(tmp, sizeof(tmp), "%3d%c", maxTemperature, clockWeather->forecast[forecastDay].symbol);
            }
            else {
                //format the temperature
                snprintf(tmp, sizeof(tmp), "+%2i%c", maxTemperature, clockWeather->forecast[forecastDay].symbol);
            }
        }

        for (byte i = 0; i < strlen(tmp); i++) {
            dotMatrix->putchar(x, y, tmp[i], config->getColor());
            //the size of the font
            x += fontWidth;
        }
    }
    //showing the whole forecast on the screen
    else {
        //long scrolling line
        char tmpLong[2 * sizeof(tmp) + sizeof(clockWeather->forecast[0].description) + 2];

        //show 2 days
        for (int i = 0; i <= 1; i++) { 
            //after midnight show the next day
            forecastDay = isOverMidnight ? i + 1 : i;

            memset(tmpLong, 0, sizeof(tmpLong));
            memset(tmp, 0, sizeof(tmp));

            //get the maximum and  minimum forecast temperatures
            if (config->getWeatherTemperatureScale() == WEATHER_CELSIUS) {
                minTemperature = clockWeather->forecast[forecastDay].tempMinC;
                maxTemperature = clockWeather->forecast[forecastDay].tempMaxC;
            }
            else {
                minTemperature = clockWeather->forecast[forecastDay].tempMinF;
                maxTemperature = clockWeather->forecast[forecastDay].tempMaxF;
            }

            if ((maxTemperature != -255) && (minTemperature != -255)) {
                //no space for the symbol, it's too hot anyway
                if (maxTemperature >= 100) {
                    snprintf(tmp, sizeof(tmp), "%3d%c", maxTemperature, clockWeather->forecast[forecastDay].symbol);
                }
                else {
                    //format the temperature
                    snprintf(tmp, sizeof(tmp), "+%2i%c", maxTemperature, clockWeather->forecast[forecastDay].symbol);
                }

                strcpy(tmpLong, tmp);
                memset(tmp, 0, sizeof(tmp));

                //add the '/' sign
                if (minTemperature >= 100) {
                    snprintf(tmp, sizeof(tmp), "/%3d", minTemperature);
                }
                else {
                    //format the temperature
                    snprintf(tmp, sizeof(tmp), "/+%2i", minTemperature);
                }

                strcat(tmpLong, tmp);

                strcat(tmpLong, " ");
                strcat(tmpLong, clockWeather->forecast[forecastDay].description);

                //show the full length scrolling
                //the first day starting from the pixel 0, the second from the pixel 8 to the middle
                showMenuValueScrolling(tmpLong, false, i * 8);

                //show only the first part
                showMenuValue(tmpLong, false, -1, i * 8);
            }
            else {
                showMenuTitle(noWeatherMessage, true);
                break;
            }
        }
    }

    dotMatrix->sendframe();

}



void ClockScreen::showMenuTitle(const char *title, bool fromFlash)
{
    char tmp[MENU_TITLE_LENGTH + 1];
    memset(tmp, 0, sizeof(tmp));

    //copy the text from the flash
    if (fromFlash) {
        strncpy_P(tmp, title, MENU_TITLE_LENGTH);
    }
    else {
        strncpy(tmp, title, MENU_TITLE_LENGTH);
    }

    // set the brightness level
    setBrightness();

    // clear the screen
    dotMatrix->clear();

    dotMatrix->setfont(FONT_4x6CL);
    byte len = strlen(tmp);
    byte x = 0;
    byte y = 0;
    byte fontWidth = 4;

    for (byte i = 0; i < len; i++) {
        dotMatrix->putchar(x, y, tmp[i], config->getColor());
        x += fontWidth;
    }

    dotMatrix->sendframe();
}

void ClockScreen::showMenuValue(const char *value, bool isAdjusting, int position, byte positionY)
{
    // set the brightness level
    setBrightness();

    dotMatrix->setfont(FONT_4x6CL);
    byte len = min(strlen(value), MENU_VALUE_LENGTH);
    byte x = 0;
    byte fontWidth = 4;

    //clear the bottom part of the screen
    for (byte i = positionY; i < 16; i++) {
        dotMatrix->line(0, i, 31, i, BLACK); 
    }

    for (byte i = 0; i < len; i++) {
        //if show a different color that means the value is being adjusted
        bool showAdjustingColor = false;
        if (isAdjusting) {
            //if the position was undefined
            if (position == -1) {
                showAdjustingColor = true; 
            }
            else if (i == position) {
                showAdjustingColor = true;
            }
        }

        dotMatrix->putchar(x, positionY, value[i], 
            (showAdjustingColor ? ((config->getColor()) != RED ? RED : ORANGE) : config->getColor()));
        x += fontWidth;
    }

    dotMatrix->sendframe();
}

void ClockScreen::showMenuValue(const int value, bool isAdjusting, byte positionY)
{
    char tmp[MENU_TITLE_LENGTH + 1];
    snprintf(tmp, sizeof(tmp), "%2d", value);

    showMenuValue(tmp, isAdjusting, -1, positionY);
}

void ClockScreen::showMenuValue(const String &value, bool isAdjusting, byte positionY)
{
    char tmp[MENU_VALUE_LENGTH + 1];
    value.toCharArray(tmp, MENU_VALUE_LENGTH + 1);

    showMenuValue(tmp, isAdjusting, -1, positionY);
}


void ClockScreen::showMenuValueScrolling(char *value, bool isAdjusting, byte positionY)
{
    // set the brightness level
    setBrightness();

    dotMatrix->setfont(FONT_4x6CL);

    //clear the place on the screen where the text will be displayed
    for (byte i = positionY; i < 16; i++) {
        dotMatrix->line(0, i, 31, i, BLACK); 
    }

    dotMatrix->hscrolltext(positionY, value, (isAdjusting ? RED : config->getColor()), 10, 1, LEFT);

    dotMatrix->sendframe();
}

void ClockScreen::showMenuValueScrolling(const String &value, bool isAdjusting, byte positionY)
{
    const int stringLength = 100;
    char tmp[stringLength];
    //cut the string to fit in the buffer
    value.toCharArray(tmp, stringLength);

    showMenuValueScrolling(tmp, isAdjusting);
}

void ClockScreen::showMenuValueString(String value, bool isAdjusting, int position)
{
    char tmp[MENU_VALUE_SCREEN_LENGTH + 1];

    //leave the last part of the string that fits
    if (value.length() > MENU_VALUE_SCREEN_LENGTH) {
        int from = value.length() - MENU_VALUE_SCREEN_LENGTH;
        value = value.substring(from);
        position = (position == 0) ? 0 : position - from;
    }

    value.toCharArray(tmp, MENU_VALUE_SCREEN_LENGTH + 1);

    showMenuValue(tmp, isAdjusting, position);

}

