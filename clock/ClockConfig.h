/**
 * The class to read and write the config data into the EEPROM inside of the microcontroller
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_CONFIG_H
#define __CLOCK_CONFIG_H

#include <Arduino.h>
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>
//will store the time change rules in the EEPROM
#include <Timezone.h>
//the font defines
#include <ht1632c.h>

#include "config.h"
#include "ClockAlarm.h"
#include "ClockTimezone.h"

// ID of the settings block
#define CONFIG_ID "clk"

// Tell it where to store your config data in the EEPROM
#define CONFIG_START 0

class ClockConfig
{
    public:
        /**
         * The timezone number in the ClockTimezone timezoneNameList
         */
        byte timezoneNumber; 
        /**
         * The daylight time start
         */
        TimeChangeRule daylightTimeStart;
        /**
         * Standard time start
         */
        TimeChangeRule standardTimeStart;

        /**
         * The screen color
         */
        byte color;

        /**
         * The minimum screen brightness: 0-15
         * The brightness changes from min to max depending on the light sensor
         */
        byte brightnessMin;
        /**
         * The maximum screen brightness: 0-15
         */
        byte brightnessMax;

        /**
         * The light sensor is enabled
         * 0 - disabled, 1 - enabled
         */
        byte lightSensorEnabled; 
        /**
         * The minimum value that the light sensor returns: 0-1023
         */
        int lightSensorMin;
        /**
         * The maximum value that the light sensor returns: 0-1023 
         */
        int lightSensorMax;

        /**
         * The 12-hour format
         * 0 - 24 hour, 1 -12 hour
         */
        byte hourFormat;

        /**
         * The time font number
         * The number is defined in ht1632c.h
         */
        int timeFont;

        /**
         * The Internet is enabled
         * 0 - disabled, 1 - enabled
         */
        byte internetEnabled; 

        /**
         * The NTP server name
         */
        char ntpServer[NTP_SERVER_LENGTH + 1];

        /**
         * The Wi-Fi access point SSID
         */
        char wifiSsid[WIFI_SSID_LENGTH + 1]; 

        /**
         * The Wi-Fi access point password
         */ 
        char wifiPassword[WIFI_PASSWORD_LENGTH + 1];

        /**
         * The Wi-Fi baud rate
         */
        long wifiBaudRate;

        /**
         * The Wi-Fi encryption
         * 0 - WPA, 1 - WEP
         */
         byte wifiEncryption;
        
        
        /**
         * The weather API key
         * Get it here: http://www.worldweatheronline.com
         */
        char weatherApiKey[WEATHER_API_KEY_LENGTH + 1]; 

        /**
         * The Weather Feed is enabled
         * 0 - disabled, 1 - enabled
         */
        byte weatherEnabled; 
        
        /**
         * The weather location: ZIP code or city
         */
        char weatherLocation[WEATHER_LOCATION_LENGTH + 1]; 
        /**
         * The temperature scale: 0 - Celsius or 1 - Fahreinheit
         */
        byte weatherTemperatureScale; 
        /**
         * The hour after which the forecast is displayed for tomorrow
         */
        byte weatherForecastHour; 

        /**
         * The SD syncing server name
         */
        char syncServerName[SYNC_SERVER_NAME_LENGTH + 1]; 
        /**
         * The SD syncing server URL
         */
        char syncServerUrl[SYNC_SERVER_URL_LENGTH + 1]; 

        /**
         * The array of alarms
         */
        ClockAlarm alarm[NUMBER_ALARMS];

        /**
         * This variable must be the last one	
         * It is used for verification
         */
        char configId[4];


        //Constructor
        ClockConfig();

        /**
         * Sets the screen color
         */
        ClockConfig *setColor(const byte);
        /**
         * Gets the screen color
         */
        const byte getColor() const;

        /**
         * Sets the minimum screen brightness
         */
        ClockConfig *setBrightnessMin(const byte);
        /**
         * Gets the minimum screen brightness
         */
        const byte getBrightnessMin() const;
        /**
         * Sets the maximum screen brightness
         */
        ClockConfig *setBrightnessMax(const byte);
        /**
         * Gets the maximum screen brightness
         */
        const byte getBrightnessMax() const;

        /**
         * Sets if the light sensor is enabled
         */
        ClockConfig *setLightSensorEnabled(const byte);
        /**
         * Gets the light sensor enabled value
         */
        const byte getLightSensorEnabled() const;
        /**
         * Sets the minimum light sensor value
         */
        ClockConfig *setLightSensorMin(const int);
        /**
         * Gets the minimum light sensor value
         */
        const int getLightSensorMin() const;
        /**
         * Sets the maximum light sensor value
         */
        ClockConfig *setLightSensorMax(const int);
        /**
         * Gets the maximum light sensor value
         */
        const int getLightSensorMax() const;

        /**
         * Sets the hour format
         */
        ClockConfig *setHourFormat(const byte);
        /**
         * Gets the hour format
         */
        const byte getHourFormat() const;
        /**
         * Gets the hour format description
         */
        void getHourFormatText(char *text, byte hourFormat = -1) const;
        /**
         * Sets the timezone number
         */
        ClockConfig *setTimezoneNumber(const byte);
        /**
         * Gets the timezone number
         */
        const byte getTimezoneNumber() const;
        /**
         * Sets the standard time start
         */
        ClockConfig *setStandardTimeStart(const TimeChangeRule rule);
        /**
         * Gets the standard time start
         */
        const TimeChangeRule &getStandardTimeStart() const;
        /**
         * Sets the daylight time start
         */
        ClockConfig *setDaylightTimeStart(const TimeChangeRule rule);
        /**
         * Gets the dailight time start
         */
        const TimeChangeRule &getDaylightTimeStart() const;

        /**
         * Sets the time font number
         */
        ClockConfig *setTimeFont(const int);
        /**
         * Gets the time font number
         */
        const int getTimeFont() const;
        /**
         * Gets the first font number
         */
        const int getFirstTimeFont() const;
        /**
         * Gets the last font number
         */
        const int getLastTimeFont() const;
        /**
         * Gets the time font description
         */
        void getTimeFontText(char *text, byte timeFont = NULL) const;

        /**
         * Sets if the Internet is enabled
         */
        ClockConfig *setInternetEnabled(const byte);

        /**
         * Gets the Internet enabled value
         */
        const byte getInternetEnabled() const;

        /**
         * Sets the NTP server name
         */
        ClockConfig *setNtpServer(const char *newNtpServer);
        /**
         * Gets the NTP server name
         */
        const char *getNtpServer() const;
        /**
         * Sets the Wi-Fi SSID
         */
        ClockConfig *setWifiSsid(const char *newWifiSsid);
        ClockConfig *setWifiSsid(const String newWifiSsid);
        /**
         * Gets the wifi SSID
         */
        const char *getWifiSsid() const;
        /**
         * Sets the Wi-Fi password
         */
        ClockConfig *setWifiPassword(const char *newWifiPassword);
        ClockConfig *setWifiPassword(const String newWifiPassword);
        /**
         * Gets the wifi password
         */
        const char *getWifiPassword() const;
        /**
         * Sets the Wi-Fi baud rate
         */
        ClockConfig *setWifiBaudRate(const long);
        /**
         * Gets the Wi-Fi baud rate
         */
        const long getWifiBaudRate() const;

        /**
         * Sets the Wi-Fi encryption
         */
        ClockConfig *setWifiEncryption(const byte);
        /**
         * Gets the Wi-Fi encryption
         */
        const long getWifiEncryption() const;


        /**
         * Sets if the Weather Feed is enabled
         */
        ClockConfig *setWeatherEnabled(const byte);
        /**
         * Gets the Weather enabled value
         */
        const byte getWeatherEnabled() const;
        /**
         * Sets the Weather API key
         */
        ClockConfig *setWeatherApiKey(const char *newWeatherApiKey);
        ClockConfig *setWeatherApiKey(const String newWeatherApiKey);
        /**
         * Gets the Weather API key
         */
        const char *getWeatherApiKey() const;

        /**
         * Sets the Weather location
         */
        ClockConfig *setWeatherLocation(const char *newWeatherLocation);
        ClockConfig *setWeatherLocation(const String newWeatherLocation);
        /**
         * Gets the Weather location
         */
        const char *getWeatherLocation() const;
        /**
         * Sets the weather temperature scale
         */
        ClockConfig *setWeatherTemperatureScale(const byte);
        /**
         * Gets the weather temperature scale
         */
        const byte getWeatherTemperatureScale() const;
        /**
         * Sets if the Weather forecast switch hour
         */
        ClockConfig *setWeatherForecastHour(const byte);
        /**
         * Gets the Weather forecast switch hour
         */
        const byte getWeatherForecastHour() const;

        /**
         * Sets the SD Sync server name
         */
        ClockConfig *setSyncServerName(const char *newSyncServerName);
        ClockConfig *setSyncServerName(const String newSyncServerName);
        /**
         * Gets the SD Sync server name
         */
        const char *getSyncServerName() const;

        /**
         * Sets the SD Sync server URL
         */
        ClockConfig *setSyncServerUrl(const char *newSyncServerUrl);
        ClockConfig *setSyncServerUrl(const String newSyncServerUrl);
        /**
         * Gets the SD Sync server URL
         */
        const char *getSyncServerUrl() const;


        /**
         * Loads the variables of the class from the EEPROM
         */
        int loadConfig();

        /**
         * Saves the variables of the class  into the EEPROM
         */
        int saveConfig();


    private:
        //sets default values
        void setDefaultValues();

};

#endif
