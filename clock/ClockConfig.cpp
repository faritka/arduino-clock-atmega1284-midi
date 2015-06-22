#include "ClockConfig.h"

/**
 * Constructor
 */
ClockConfig::ClockConfig() 
{
    setDefaultValues();
}

void ClockConfig::setDefaultValues()
{
    //the ID of the config
    strncpy(configId, CONFIG_ID, 4);

    //US Pacific Time Zone (Las Vegas, Los Angeles)
    timezoneNumber = 4;
    ClockTimezone zone;
    zone.setTimezoneIntoConfig(this, timezoneNumber);
    /*
    TimeChangeRule usPDT = {"PDT", Second, dowSunday, Mar, 2, -420};
    TimeChangeRule usPST = {"PST", First, dowSunday, Nov, 2, -480};
    daylightTimeStart = usPDT;
    standardTimeStart = usPST;
    */


    //the screen color, 3 - ORANGE
    color = 3;
    //the screen brightness
    brightnessMin = 1;
    brightnessMax = 15;

    //the light sensor values
    lightSensorEnabled = 0;
    lightSensorMin = 0;
    lightSensorMax = 1023;

    //the hour format: 12/24 hours
    hourFormat = 0;

    //the time font
    timeFont = getFirstTimeFont();

    //the Internet defaults
    internetEnabled = 0;
    strncpy(ntpServer, "0.pool.ntp.org", sizeof(ntpServer));
    strncpy(wifiSsid, "DG1670AA2", sizeof(wifiSsid));
    strncpy(wifiPassword, "DG1670A7250A2", sizeof(wifiPassword));
    //2MB
    wifiBaudRate = 200000;
    //WPA
    wifiEncryption = 0;    

    //the Weather Feed defaults
    weatherEnabled = 0;
    strncpy(weatherApiKey, "2qdvnjka3js9s6zjbsfcnj9u", sizeof(weatherApiKey));
    strncpy(weatherLocation, "Los Angeles", sizeof(weatherLocation));
    weatherTemperatureScale = 0;
    weatherForecastHour = 18;


    //SD syncing
    strncpy(syncServerName, "program.farit.ru", sizeof(syncServerName));
    strncpy(syncServerUrl, "/clock/script/", sizeof(syncServerUrl));    

#ifdef DEBUG
    Serial.print(F("EEPROM config size: "));
    Serial.println(sizeof(*this));
#endif

    //loads the config from EEPROM
    loadConfig();
}

ClockConfig *ClockConfig::setColor(const byte newColor)
{
    color = newColor;
    return this;
}
const byte ClockConfig::getColor() const
{
    return color;
}

ClockConfig *ClockConfig::setBrightnessMin(const byte newBrightnessMin)
{
    brightnessMin = newBrightnessMin;
    return this;
}
const byte ClockConfig::getBrightnessMin() const
{
    return brightnessMin;
}
ClockConfig *ClockConfig::setBrightnessMax(const byte newBrightnessMax)
{
    brightnessMax = newBrightnessMax;
    return this;
}
const byte ClockConfig::getBrightnessMax() const
{
    return brightnessMax;
}

ClockConfig *ClockConfig::setLightSensorEnabled(const byte newLightSensorEnabled)
{
    lightSensorEnabled = newLightSensorEnabled;
    return this;
}
const byte ClockConfig::getLightSensorEnabled() const
{
    return lightSensorEnabled;
}
ClockConfig *ClockConfig::setLightSensorMin(const int newLightSensorMin)
{
    lightSensorMin = newLightSensorMin;
    return this;
}
const int ClockConfig::getLightSensorMin() const
{
    return lightSensorMin;
}
ClockConfig *ClockConfig::setLightSensorMax(const int newLightSensorMax)
{
    lightSensorMax = newLightSensorMax;
    return this;
}
const int ClockConfig::getLightSensorMax() const
{
    return lightSensorMax;
}

ClockConfig *ClockConfig::setHourFormat(const byte newHourFormat)
{
    hourFormat = newHourFormat;
    return this;
}
const byte ClockConfig::getHourFormat() const
{
    return hourFormat;
}
void ClockConfig::getHourFormatText(char *text, byte newHourFormat) const
{
    byte format = (newHourFormat != -1) ? newHourFormat : hourFormat;

    strncpy(text, ((format == 1) ? "12h" : "24h"), sizeof(text));
}

ClockConfig *ClockConfig::setTimezoneNumber(const byte newTimezoneNumber)
{
    timezoneNumber = newTimezoneNumber;
    return this;
}
const byte ClockConfig::getTimezoneNumber() const
{
    return timezoneNumber;
}

ClockConfig *ClockConfig::setStandardTimeStart(const TimeChangeRule rule)
{
    standardTimeStart = rule;
    return this;
}
const TimeChangeRule &ClockConfig::getStandardTimeStart() const
{
    return standardTimeStart;
}

ClockConfig *ClockConfig::setDaylightTimeStart(const TimeChangeRule rule)
{
    daylightTimeStart = rule;
    return this;
}
const TimeChangeRule &ClockConfig::getDaylightTimeStart() const
{
    return daylightTimeStart;
}

ClockConfig *ClockConfig::setTimeFont(const int newTimeFont)
{
    timeFont = newTimeFont;
    return this;
}
const int ClockConfig::getTimeFont() const
{
    return timeFont;
}
const int ClockConfig::getFirstTimeFont() const
{
    return FONT_7x9CL;
}
const int ClockConfig::getLastTimeFont() const
{
    return FONT_7x9CLB;
}
void ClockConfig::getTimeFontText(char *text, byte newTimeFont) const
{
    int font = (newTimeFont != NULL) ? newTimeFont : timeFont;

    switch (font) {
        case FONT_7x9CL:
        strcpy(text, "Plain");
            break;
        case FONT_7x9CLB:
            strcpy(text, "Bold");
            break;
        default:
            strcpy(text, "Default");
    }
}

ClockConfig *ClockConfig::setInternetEnabled(const byte newInternetEnabled)
{
    internetEnabled = newInternetEnabled;
    return this;
}
const byte ClockConfig::getInternetEnabled() const
{
    return internetEnabled;
}
ClockConfig *ClockConfig::setNtpServer(const char *newNtpServer)
{
    strncpy(ntpServer, newNtpServer, NTP_SERVER_LENGTH + 1);
    return this;
}
const char *ClockConfig::getNtpServer() const
{
    return ntpServer;
}


ClockConfig *ClockConfig::setWifiSsid(const char *newWifiSsid)
{
    strncpy(wifiSsid, newWifiSsid, WIFI_SSID_LENGTH + 1);
    return this;
}
ClockConfig *ClockConfig::setWifiSsid(const String newWifiSsid)
{
    newWifiSsid.toCharArray(wifiSsid, WIFI_SSID_LENGTH + 1);
    return this;
}

const char *ClockConfig::getWifiSsid() const
{
    return wifiSsid;
}

ClockConfig *ClockConfig::setWifiPassword(const char *newWifiPassword)
{
    strncpy(wifiPassword, newWifiPassword, WIFI_PASSWORD_LENGTH + 1);
    return this;
}
ClockConfig *ClockConfig::setWifiPassword(const String newWifiPassword)
{
    newWifiPassword.toCharArray(wifiPassword, WIFI_PASSWORD_LENGTH + 1);
    return this;
}
const char *ClockConfig::getWifiPassword() const
{
    return wifiPassword;
}
ClockConfig *ClockConfig::setWifiBaudRate(const long newWifiBaudRate)
{
    wifiBaudRate = newWifiBaudRate;
    return this;
}
const long ClockConfig::getWifiBaudRate() const
{
    return wifiBaudRate;
}

ClockConfig *ClockConfig::setWifiEncryption(const byte newWifiEncryption)
{
    wifiEncryption = newWifiEncryption;		
    return this;
}
const long ClockConfig::getWifiEncryption() const
{
    return wifiEncryption;
}

ClockConfig *ClockConfig::setWeatherEnabled(const byte newWeatherEnabled)
{
    weatherEnabled = newWeatherEnabled;
    return this;
}
const byte ClockConfig::getWeatherEnabled() const
{
    return weatherEnabled;
}


ClockConfig *ClockConfig::setWeatherApiKey(const char *newWeatherApiKey)
{
    strncpy(weatherApiKey, newWeatherApiKey, WEATHER_API_KEY_LENGTH + 1);
    return this;
}
ClockConfig *ClockConfig::setWeatherApiKey(const String newWeatherApiKey)
{
    newWeatherApiKey.toCharArray(weatherApiKey, WEATHER_API_KEY_LENGTH + 1);
    return this;
}

const char *ClockConfig::getWeatherApiKey() const
{
    return weatherApiKey;
}

ClockConfig *ClockConfig::setWeatherLocation(const char *newWeatherLocation)
{
    strncpy(weatherLocation, newWeatherLocation, WEATHER_LOCATION_LENGTH + 1);
    return this;
}
ClockConfig *ClockConfig::setWeatherLocation(const String newWeatherLocation)
{
    newWeatherLocation.toCharArray(weatherLocation, WEATHER_LOCATION_LENGTH + 1);
    return this;
}

const char *ClockConfig::getWeatherLocation() const
{
    return weatherLocation;
}

ClockConfig *ClockConfig::setWeatherTemperatureScale(const byte newWeatherTemperatureScale)
{
    weatherTemperatureScale = newWeatherTemperatureScale;
    return this;
}
const byte ClockConfig::getWeatherTemperatureScale() const
{
    return weatherTemperatureScale;
}
ClockConfig *ClockConfig::setWeatherForecastHour(const byte newWeatherForecastHour)
{
    weatherForecastHour = newWeatherForecastHour;
    return this;
}
const byte ClockConfig::getWeatherForecastHour() const
{
    return weatherForecastHour;
}


ClockConfig *ClockConfig::setSyncServerName(const char *newSyncServerName)
{
    strncpy(syncServerName, newSyncServerName, SYNC_SERVER_NAME_LENGTH + 1);
    return this;
}
ClockConfig *ClockConfig::setSyncServerName(const String newSyncServerName)
{
    newSyncServerName.toCharArray(syncServerName, SYNC_SERVER_NAME_LENGTH + 1);
    return this;
}
const char *ClockConfig::getSyncServerName() const
{
    return syncServerName;
}

ClockConfig *ClockConfig::setSyncServerUrl(const char *newSyncServerUrl)
{
    strncpy(syncServerUrl, newSyncServerUrl, SYNC_SERVER_URL_LENGTH + 1);
    return this;
}
ClockConfig *ClockConfig::setSyncServerUrl(const String newSyncServerUrl)
{
    newSyncServerUrl.toCharArray(syncServerUrl, SYNC_SERVER_URL_LENGTH + 1);
    return this;
}
const char *ClockConfig::getSyncServerUrl() const
{
    return syncServerUrl;
}


int ClockConfig::loadConfig()
{
    char* p = (char*)(void*)this;
    unsigned int i;   

    //check if the config id at the correct place in the end
    if ((EEPROM.read(CONFIG_START + sizeof(*this) - 2) == configId[2]) 
        && (EEPROM.read(CONFIG_START + sizeof(*this) - 3) == configId[1]) 
        && (EEPROM.read(CONFIG_START + sizeof(*this) - 4) == configId[0])) {
        for (i = 0; i < sizeof(*this); i++) {
            *p++ = EEPROM.read(CONFIG_START + i);
    }

#ifdef DEBUG        
        Serial.println(F("The config was read from EEPROM"));    
#endif
    }
    //save with the default settings
    else {
#ifdef DEBUG
        Serial.println(F("Saving the default config into EEPROM"));
#endif
        saveConfig();
    }

    return i;
}

int ClockConfig::saveConfig()
{
    //write all config variables of the config class in the EEPROM
    const char* p = (const char*)(const void*)this;
    unsigned int i;

    for (i = 0; i < sizeof(*this); i++) {
        EEPROM.write(CONFIG_START + i, *p++);
    }

#ifdef DEBUG
    Serial.print(F("Writing the config object into EEPROM."));
    Serial.print(F(" The number of bytes written into EEPROM: "));
    Serial.println(i);
#endif

    return i;
}
