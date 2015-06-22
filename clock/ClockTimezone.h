/**
 * The class to define timezones
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_TIMEZONE_H
#define __CLOCK_TIMEZONE_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Time.h>
#include <Timezone.h>

#include "config.h"
#include "ClockConfig.h"

//forward declaration
class ClockConfig;

class ClockTimezone 
{
    public:

        /**
         * The number of timezones
         */
        static const byte NUMBER_OF_TIMEZONES = 34;

        /**
         * Constructor
         */
        ClockTimezone();

        /**
         * Returns the timezone name by its number
         */
        void getTimezoneNameByNumber(char *buf, const int bufSize, const byte timezoneNumber);

        /**
         * Sets the timezone in the config object
         */
        void setTimezoneIntoConfig(ClockConfig *config, byte timezoneNumber);

    private:
        /**
         * The list of timezone names
         */
        static const char* const timezoneNames[] PROGMEM;

        static const char kwajalein[] PROGMEM;
        static const char apia[] PROGMEM;
        static const char honolulu[] PROGMEM;
        static const char anchorage[] PROGMEM;
        static const char losAngeles[] PROGMEM;
        static const char calgary[] PROGMEM;
        static const char phoenix[] PROGMEM;
        static const char chicago[] PROGMEM;
        static const char toronto[] PROGMEM;
        static const char caracas[] PROGMEM;
        static const char asuncion[] PROGMEM;
        static const char labrador[] PROGMEM;
        static const char buenos[] PROGMEM;
        static const char fernando[] PROGMEM;
        static const char azores[] PROGMEM;
        static const char london[] PROGMEM;
        static const char paris[] PROGMEM;
        static const char istanbul[] PROGMEM;
        static const char baghdad[] PROGMEM;
        static const char moscow[] PROGMEM;
        static const char tehran[] PROGMEM;
        static const char dubai[] PROGMEM;
        static const char kabul[] PROGMEM;
        static const char tashkent[] PROGMEM;
        static const char colombo[] PROGMEM;
        static const char kathmandu[] PROGMEM;
        static const char ufa[] PROGMEM;
        static const char cocos[] PROGMEM;
        static const char novosibirsk[] PROGMEM;
        static const char krasnoyarsk[] PROGMEM;
        static const char irkutsk[] PROGMEM;
        static const char adelaide[] PROGMEM;
        static const char yakutsk[] PROGMEM;
        static const char vladivostok[] PROGMEM;
        static const char magadan[] PROGMEM;

        /**
         * The list of offsets for each timezone
         */
        static const int16_t standardOffsets[] PROGMEM;
        static const int16_t daylightOffsets[] PROGMEM;
        /**
         * The list of the weeks when standard times start
         */
        static const int8_t standardWeeks[] PROGMEM;
        /**
         * The list of the weeks when daylight times start
         */
        static const int8_t daylightWeeks[] PROGMEM;
        /**
         * The list of the days of weeks when standard times start
         */
        static const int8_t standardDayOfWeeks[] PROGMEM;
        /**
         * The list of the days of weeks when daylight times start
         */
        static const int8_t daylightDayOfWeeks[] PROGMEM;
        /**
         * The list of the months when standard times start
         */
        static const int8_t standardMonths[] PROGMEM;
        /**
         * The list of the months when daylight times start
         */
        static const int8_t daylightMonths[] PROGMEM;
        /**
         * The list of the hours when standard times start
         */
        static const int8_t standardHours[] PROGMEM;
        /**
         * The list of the hours when daylight times start
         */
        static const int8_t daylightHours[] PROGMEM;

};

#endif
