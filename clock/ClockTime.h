/**
 * The class to read and write data from the Real Time Clock chip
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_TIME_H
#define __CLOCK_TIME_H

#include <Arduino.h>
#include <Wire.h>
#include <Time.h>       //http://www.arduino.cc/playground/Code/Time
#include <Timezone.h>   //https://github.com/JChristensen/Timezone
#include <DS3232RTC.h>  //https://github.com/JChristensen/DS3232RTC

#include "config.h"
#include "ClockConfig.h"
#include "ClockAlarm.h"
#include "ClockMusic.h"
#include "ClockNtp.h"

class ClockTime
{
    public:

        //Constructor
        ClockTime(ClockConfig *clockConfig, ClockMusic *clockMusic);

        /**
          * Sets the local timezone from config
          */
        ClockTime *setTimezone();

        /**
          * Returns the current time in the local timezone
          */
        time_t getCurrentTime();
        /**
         * Saves the current time into the chip in the UTC time
         */
        ClockTime *saveCurrentTime();

        /**
         * Returns the time elements in the local timezone
         */
        tmElements_t getCurrentTimeElements();

        /**
         * Saves the time elements into the chip in the UTC time
         */
        ClockTime *saveCurrentTimeElements();

        /**
         * Returns the hour in the local timezone
         */
        int getHour();
        /**
         * Sets the hour into the clock chip in the UTC time
         */
        ClockTime *setHour(int hour); 
        /**
         * Returns the minute in the local timezone
         */
        int getMinute();
        /**
         * Sets the minute into the clock chip in the UTC time
         */
        ClockTime *setMinute(const int minute); 
        /**
         * Returns the AM/PM in the local timezone
         * 0 - AM, 1 - PM
         */
        byte getAmPm();
        /**
         * Returns the AM/PM description in the local timezone
         */
        void getAmPmText(char *text, byte newAmPm = -1);
        /**
         * Sets the AM/PM 
         */
        ClockTime *setAmPm(const byte amPm); 
        /**
         * Returns the year in the local timezone
         */
        int getYear();
        /**
         * Sets the year into the clock chip in the UTC time
         */
        ClockTime *setYear(int year); 
        /**
         * Returns the month in the local timezone
         */
        int getMonth();
        /**
         * Returns the month name by its number
         */
        static const char *getMonthName(byte monthNumber)
        {        
            return (ClockTime::monthNames[monthNumber - 1]);
        }

        /**
         * Sets the month into the clock chip in the UTC time
         */
        ClockTime *setMonth(const int month); 
        /**
         * Returns the day in the local timezone
         */
        int getDay();
        /**
         * Sets the day into the clock chip in the UTC time
         */
        ClockTime *setDay(const int day); 

        /**
         * Returns the weekday name by its number
         */
        static const char *getWeekdayName(byte weekdayNumber)
        {
            return (ClockTime::weekdayNames[weekdayNumber]);
        }

        /**
         * Checks if any of the alarms matches the current time
         */
        ClockTime *checkAlarm();

        /**
         * Updates the current time from the NTP server
         */
        ClockTime *updateNtpTime();

        /**
         * Calculates the next alarm if any is active
         */
        ClockTime *calculateNextAlarm();

        /**
         * Calculates the date for tomorrow
         */
        time_t getTomorrow(unsigned int &year, byte &month, byte &day);

        /**
          * Runs the watchdog for hang ups
          * The RTC will send periodic interrupts to the microprocessor.
          * When the processor hangs up, an interrupt will reset it.
          */
        ClockTime *runWatchdog();

        /**
         * Resets the watchdog alarms after an alarm was triggered
         *
         */
        void resetWatchdog();

    private:
        //stores the config object
        ClockConfig *config;

        //the alarm music object
        ClockMusic *music;
        //UTC time
        time_t utc;
        //local timezone
        Timezone *localTimeZone;
        //current time
        time_t currentTime;
        //current time elements structure
        tmElements_t currentTimeElements;

        //next alarm time
        time_t nextAlarm;
        //next alarm number
        byte nextAlarmNumber;

        //the last NTP server update time
        time_t lastNtpUpdate;
        //the last unsuccessful NTP server update time
        time_t lastUnsuccessfulNtpUpdate;
        //the number of unsuccessful NTP connection tries
        byte ntpUpdateCounter;

        //the month names
        static const char monthNames[12][4];

        //the weekday names
        static const char weekdayNames[8][3];

};

#endif
