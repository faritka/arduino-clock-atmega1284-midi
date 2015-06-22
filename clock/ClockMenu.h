/**
 * The class to display menus
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_MENU_H
#define __CLOCK_MENU_H

#include <Arduino.h>
#include <Button.h>        //https://github.com/JChristensen/Button
#include <MenuInterface.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "ClockAlarm.h"
#include "ClockConfig.h"
#include "ClockScreen.h"
#include "ClockSync.h"
#include "ClockTime.h"
#include "ClockTimezone.h"

//The debouncing time for button interrupts in ms
#define DEBOUNCING_TIME 15

//The time in ms required before repeats on long press
#define REPEAT_FIRST 1000
//Repeat interval for long press
#define REPEAT_INCR 600


//The forecast menu level
#define FORECAST_LEVEL 1

struct ClockMenuAlarm
{
    public:

        MenuItem *alarm;
        MenuItem *active;
        static const char titleActive[] PROGMEM;
          MenuItem *music;
        static const char titleMusic[] PROGMEM;
          MenuItem *songName;
        static const char titleSongName[] PROGMEM;
          MenuItem *songRandom;
        static const char titleSongRandom[] PROGMEM;
          MenuItem *songTime;
        static const char titleSongTime[] PROGMEM;
          MenuItem *hour;
        static const char titleHour[] PROGMEM;
          MenuItem *minute;
        static const char titleMinute[] PROGMEM;
          MenuItem *amPm;
        static const char titleAmPm[] PROGMEM;
          MenuItem *when;
        static const char titleWhen[] PROGMEM;
          MenuItem *weekdays;
        static const char titleWeekdays[] PROGMEM;
          MenuItem *repeat;
        static const char titleRepeat[] PROGMEM;
          MenuItem *date;
        static const char titleDate[] PROGMEM;
        MenuItem *year;
        static const char titleYear[] PROGMEM;
          MenuItem *month;
        static const char titleMonth[] PROGMEM;
        MenuItem *day;
        static const char titleDay[] PROGMEM;
          MenuItem *sunday;
        static const char titleSunday[] PROGMEM;
        MenuItem *monday;
        static const char titleMonday[] PROGMEM;
          MenuItem *tuesday;
        static const char titleTuesday[] PROGMEM;
        MenuItem *wednesday;
        static const char titleWednesday[] PROGMEM;
          MenuItem *thursday;
        static const char titleThursday[] PROGMEM;
        MenuItem *friday;
        static const char titleFriday[] PROGMEM;
          MenuItem *saturday;
        static const char titleSaturday[] PROGMEM;
          MenuItem *repeatTimes;
        static const char titleRepeatTimes[] PROGMEM;
          MenuItem *repeatInterval;
        static const char titleRepeatInterval[] PROGMEM;
};


class ClockMenu : MenuInterface
{
    public:
        //Constructor
        ClockMenu(ClockConfig *clockConfig, ClockScreen *clockScreen, ClockTime *clockTime,
            ClockMusic *clockMusic, ClockLightSensor *clockLightSensor, ClockWeather *clockWeather);

        /**
         * The debouncing time for button interrupts in ms
         */
        static const unsigned int debouncingTime = 15;

        /**
         * Handles all events  
         * This is where you define a behaviour for a menu item
         */
        void menuUse(MenuUseEvent used);

        /**
         * Here we get a notification whenever the user changes the menu
         * That is, when the menu is navigated
         */
        void menuChange(MenuChangeEvent changed);

        /**
          * Here we get a notification whenever
         */
        void menuItemChange(MenuItemChangeEvent changed);

        /**
         * Checks if any of the buttons was pressed
         */
        void checkButtons();

        /**
         * Checks if the menu is active
         */
        bool isActive(time_t currentTime);

        /**
         * Goes to the forecast menu
         */
        void goForecast();

    private:
        /**
         * The config object
         */
        ClockConfig *config; 

        /**
         * The screen object
         */
        ClockScreen *screen;
        /**
         * The time object
         */
        ClockTime *time;
        /**
         * The clock music object
         */
        ClockMusic *music;
        /**
         * The light sensor object
         */
        ClockLightSensor *lightSensor;
        /**
         * The weather object
         */
        ClockWeather *weather;

        /**
         * The Enter character for string values
         */
        const static int enterCharacter = 127;

        /**
         * The time in ms required before repeats on long press
         */
        static const unsigned int repeatFirst = 1000;

        /**
         * Repeat interval for long presses
         */
        static const unsigned int repeatIncr = 600;

        /**
         * The forecast menu level
         */
        static const unsigned int forecastLevel = 1;

        /**
         * The last time when the menu was used
         */
        unsigned long lastUseTime;

        /**
         * The control buttons
         */
        Button *buttonUp;
        Button *buttonEnter;
        Button *buttonDown;

        /**
         * Repeats the increment or decrement 
         * when the buttons are pressed for long time
         */
        unsigned long buttonRepeat;

        /**
         * Initializes interrupts for the buttons
         */
        void initializeInterrupt();

        /**
         * The menu backend
         */
        MenuBackend *menu;

        /**
         * The menu items
         */
        //shows the full forecast
        MenuItem *itemForecast;
        static const char titleForecast[] PROGMEM;

        //sets the time and date 
        MenuItem *itemTimeAndDate;
        static const char titleTimeAndDate[] PROGMEM;

        //the time
        MenuItem *itemHourFormat;
        static const char titleHourFormat[] PROGMEM;
        MenuItem *itemHour;
        static const char titleHour[] PROGMEM;
        MenuItem *itemMinute;
        static const char titleMinute[] PROGMEM;
        MenuItem *itemAmPm;
        static const char titleAmPm[] PROGMEM;

        //the date
        MenuItem *itemDay;
        static const char titleDay[] PROGMEM;
        MenuItem *itemMonth;
        static const char titleMonth[] PROGMEM;
        MenuItem *itemYear;
        static const char titleYear[] PROGMEM;

        /**
         * The timezone
         */
        MenuItem *itemTimezone;
        static const char titleTimezone[] PROGMEM;

        /**
         * The screen menu
         */
        MenuItem *itemScreen;
        static const char titleScreen[] PROGMEM;

        /**
         * The screen color
         */
        MenuItem *itemColor;
        static const char titleColor[] PROGMEM;

        /**
         * The minimum screen brightness
         */
        MenuItem *itemBrightnessMin;
        static const char titleBrightnessMin[] PROGMEM;

        /**
         * The maximum screen brightness
         */
        MenuItem *itemBrightnessMax;
        static const char titleBrightnessMax[] PROGMEM;

        /**
         * The light sensor
         */
        MenuItem *itemLightSensor; 
        static const char titleLightSensor[] PROGMEM;

        /**
         * The light sensor enabled
         */
        MenuItem *itemLightSensorEnabled;
        static const char titleLightSensorEnabled[] PROGMEM;

        /**
         * The light sensor calibration
         */
        MenuItem *itemLightSensorCalibration;
        static const char titleLightSensorCalibration[] PROGMEM;

        /**
         * The Internet setup
         */
        MenuItem *itemInternet;
        static const char titleInternet[] PROGMEM;
        MenuItem *itemInternetEnabled;
        static const char titleInternetEnabled[] PROGMEM;

        /**
         * The Wi-Fi setup
         */
        MenuItem *itemWifiSsid;
        static const char titleWifiSsid[] PROGMEM;
        MenuItem *itemWifiPassword;
        static const char titleWifiPassword[] PROGMEM;

        /**
         * The weather setup
         */
        MenuItem *itemWeather;
        static const char titleWeather[] PROGMEM;
        MenuItem *itemWeatherEnabled;
        static const char titleWeatherEnabled[] PROGMEM;

        /**
         * Setup for the weather forecast feed from
         * http://www.worldweatheronline.com 
         */
        MenuItem *itemWeatherApiKey;
        static const char titleWeatherApiKey[] PROGMEM;

        MenuItem *itemWeatherLocation;
        static const char titleWeatherLocation[] PROGMEM;

        /**
         * Celsius or Fahrenheit
         */
        MenuItem *itemWeatherTemperatureScale;
        static const char titleWeatherTemperatureScale[] PROGMEM;

        /**
         * The hour after whick the weather forecast is displayed for tomorrow
         */
        MenuItem *itemWeatherForecastHour;
        static const char titleWeatherForecastHour[] PROGMEM;

        /**
         * Synchronizing
         */
        MenuItem *itemSync; 
        static const char titleSync[] PROGMEM;
        /**
         * The sync server name
         */
        MenuItem *itemSyncServerName; 
        static const char titleSyncServerName[] PROGMEM;
        /**
         * The sync server URL
         */
        MenuItem *itemSyncServerUrl; 
        static const char titleSyncServerUrl[] PROGMEM;

        /**
         * Synchronizing start
         */
        MenuItem *itemSyncStart; 
        static const char titleSyncStart[] PROGMEM;

        /**
         * The array of alarms
         */
        MenuItem *itemAlarms;
        static const char titleAlarms[] PROGMEM;
        ClockMenuAlarm menuAlarms[NUMBER_ALARMS];
        /**
         * The names for alarms
         */
        static const char titleAlarm1[] PROGMEM;
        static const char titleAlarm2[] PROGMEM;
        static const char titleAlarm3[] PROGMEM;
        static const char titleAlarm4[] PROGMEM;
        static const char titleAlarm5[] PROGMEM;
        /**
         * The array for alarm names
         */
        static const char *titleAlarmNames[5];


        /**
         * Sets the menu item value adjusting mode
         * The UP and DOWN buttons will change the value of the item
         */
        bool isAdjusting;
        /**
         * Sets the string menu value adjusting mode
         * It requires that several characters are entered
         */
        bool isStringAdjusting;

        /**
         * The menu item value for integers that is being adjusted
         */
        int itemValue;

        /**
         * The menu item value for strings
         */
        String itemValueString;

        /**
         * The position of a character that is being changed in the item string value
         */
        byte itemValueStringPosition;

        /**
         * Values yes and no for boolean values
         */
        static const char yesNo[2][4];


        /**
         * The help messages
         */
        static const char stringHelp[] PROGMEM;
        static const char weatherApiKeyHelp[] PROGMEM;
        static const char weatherLocationHelp[] PROGMEM;
        static const char weatherForecastHourHelp[] PROGMEM;
        static const char lightSensorHelp[] PROGMEM;
        static const char syncHelp[] PROGMEM;

        /**
         * Sets up the menu
         */
        void menuSetup();

        /**
         * Adds the item AM/PM when the 12-hour format is used
         */
        void addItemAmPm();

        /**
         * When the user clicked on the UP or DOWN button and changed the item value
         */
        void itemValueChange(MenuItem *currentItem, bool upPressed, bool downPressed);

         /**
          * Sets the menu item value
          */
         void setItemValue(bool upPressed, bool downPressed, 
             int minValue, int maxValue, bool circle = true);

         /**
          * Sets the menu item string value
          */
         void setItemValueString(bool upPressed, bool downPressed);

        /**
         * Toggle the adjusting status
         */
        inline void toggleAdjusting()
        {
              isAdjusting = isAdjusting ? false : true;
        }

        /**
         * Returns integer 0/1 as yes/no text
         */
        inline const char *getYesNo(byte value) const
        {
            return (ClockMenu::yesNo[value]);
        }

         /**
          * What to do when a character was entered into the string value
          */
         void enterStringValueCharacter();

         /**
          * Shows the help 
          */
         void showHelp(char *buf, const char PROGMEM *helpText); 

};

#endif
