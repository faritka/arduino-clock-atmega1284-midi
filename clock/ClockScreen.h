/**
 * The class to display contents on the LED 32x16 screen from Sure Electronics with the chips ht1632
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_SCREEN_H
#define __CLOCK_SCREEN_H

#include <Arduino.h>
#include <ht1632c.h>

#include "config.h"
#include "ClockConfig.h"
#include "ClockLightSensor.h"
#include "ClockTime.h"
#include "ClockWeather.h"

class ClockScreen 
{
    public:

        //Constructor
        ClockScreen(ClockConfig *clockConfig, ClockLightSensor *clockLightSensor);

        /**
         * Shows the time using big numbers on the screen
         *
         */
        void showTime(time_t currentTime);

        /**
         * Shows the date using small characters on the screen
         *
         */
        void showDate(time_t currentTime);

        /**
         * Shows the full weather forecast on the whole screen
         *
         * @param ClockWeather *clockWeather The object
         * @param time_t currentTime The current time
         * @param bool fullScreen If show more information on the whole screen
         *
         */
        void showWeather(ClockWeather *clockWeather, time_t currentTime, bool fullScreen = false);

        /**
         * Shows the title of the menu
         *
         * @param char *title The menu title
         * @param bool fromFlash If true, read the title value from the flash memory
         */
        void showMenuTitle(const char *title, bool fromFlash = true);

        /**
         * Shows the value of the menu
         *
         */
        void showMenuValue(const char *value, bool isAdjusting = false, int position = -1, byte positionY = 8);
        void showMenuValue(const int value, bool isAdjusting = false, byte positionY = 8);
        void showMenuValue(const String &value, bool isAdjusting = false, byte positionY = 8);

        /**
         * Shows the menu value using scrolling for longer texts
         *
         * @param char *value The value that is displayed
         * @param bool isAdjusting If true, changes the color of the value
         * @param byte positionY Where to start displaying the value
         */
        void showMenuValueScrolling(char *value, bool isAdjusting = false, byte positionY = 8);
        void showMenuValueScrolling(const String &value, bool isAdjusting = false, byte positionY = 8);

        /**
         * Shows the last part of the string value that fits into the screen
         *
         * @param bool isAdjusting If the value is being adjusted, change the color
         * @param int position The position of the character that is being adjusted
         */
        void showMenuValueString(const String value, bool isAdjusting = false, int position = -1);

        /**
         * Returns the color name by its number
         */
        static const char *getColorNameByNumber(byte colorNumber)
        {
            return (ClockScreen::colorNames[colorNumber]);
        }
        /**
         * Returns the color ID by its number
         */
        static const byte getColorIdByNumber(byte colorNumber)
        {
            return (ClockScreen::colorIds[colorNumber]);
        }
        /**
         * Returns the color number by its ID
         */
        static const byte getColorNumberById(byte colorId)
        {
            for (int i = 0; i < (sizeof(ClockScreen::colorIds)/sizeof(byte)); i++) {
                if (ClockScreen::colorIds[i] == colorId) {
                    return i;
                }
            }
            //the default color
            return 1;
        }

    private:
        /**
         * Stores the config object
         */
        ClockConfig *config;

        /**
          * @var ht1632 For the LED screen
          */
        ht1632c *dotMatrix;

        /**
         * The light sensor object
         */
        ClockLightSensor *lightSensor;

        /**
         * The color names array
         */
        static const char colorNames[5][7];
        /**
         * The color IDs array
         * The IDs are for ht1632c
         */
        static const byte colorIds[5];

        /**
         * The no weather forecast message
         */
        static const char noWeatherMessage[] PROGMEM;

        /**
         * Sets the screen brightness
         */
        void setBrightness()
        {
            //the system has the light sensor
            if (config->getLightSensorEnabled()) {
                  dotMatrix->pwm(lightSensor->getBrightness());
              }
              else {
                  dotMatrix->pwm(config->getBrightnessMax());
              }
        }

};

#endif
