/**
 * The class to define and store clock alarms
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_ALARM_H
#define __CLOCK_ALARM_H

#include <Arduino.h>
#include <Time.h>

#include "config.h"

//when an alarm is played
#define ALARM_WHEN_DAILY 0
#define ALARM_WHEN_TOMORROW 1
#define ALARM_WHEN_DATE 2


class ClockAlarm
{
    public:
        //Constructor
        ClockAlarm();

        /**
         * Gets the UNIX time of the next alarm relative to the current time
         */
        time_t convertToTime(const time_t currentTime, const time_t checkTime);

        /**
         * Sets the active flag
         */
        ClockAlarm *setActive(byte active);
        /**
         * Gets the active flag
         */
        byte getActive() const;		
        /**
         * Sets the number of the times the alarm should be repeated
         */
        /**
         * Sets the song name
         */
        ClockAlarm *setSongName(const char *newSongName);
        /**
         * Gets the song name
         */
        const char *getSongName() const;		
        /**
         * Sets the song randomness. 1 - random, 0 - not
         */
        ClockAlarm *setSongRandom(byte newSongRandom);
        /**
         * Gets the song randomness
         */
        byte getSongRandom() const;		
        /**
         * Sets a song maximum playing time
         */
        ClockAlarm *setSongTime(int newSongTime);
        /**
         * Gets a song maximum playing time
         */
        int getSongTime() const;		

        /**
         * Sets the number of the times the alarm should be repeated
         */
        ClockAlarm *setRepeatTimes(byte repeatTimes);
        /**
         * Gets the number of the alarm repeats
         */
        byte getRepeatTimes() const;	 
        /**
         * Sets the repeat interval in minutes
         */
        ClockAlarm *setRepeatInterval(byte repeatInverval);
        /**
         * Gets the repeat interval
         */
        byte getRepeatInterval() const;		
        /**
         * Sets the When flag
         * 0 - daily, 1 - tomorrow, 2 - date
         */		 			 
        ClockAlarm *setWhen(byte when);
        /**
         * Gets the When flag
         */
        byte getWhen() const;        
        /**
         * Returns the When name by its number
         */
        static const char *getWhenName(byte number)
        {
            return (ClockAlarm::whenNames[number]);
        }

        /**
         * Sets the alarm hour
         */
        ClockAlarm *setHour(byte hour);
        /**
         * Gets the alarm hour
         */
        byte getHour(const byte hourFormat) const;
        /**
         * Sets the alarm minute
         */
        ClockAlarm *setMinute(byte minute);
        /**
         * Gets the alarm minute
         */
        byte getMinute() const;
        /**
         * Sets the alarm AM/PM
         * 0 - AM, 1 - PM
         */
        ClockAlarm *setAmPm(byte newAmPm);
        /**
         * Gets the alarm AM/PM
         */
        byte getAmPm() const;
        /**
         * Sets the alarm second
         */
        ClockAlarm *setSecond(byte second);
        /**
         * Gets the alarm second
         */
        byte getSecond() const; 
        /**
         * Sets the alarm year
         */
        ClockAlarm *setYear(unsigned int year);
        /**
         * Gets the alarm year
         */
        unsigned int getYear() const;
        /**
         * Sets the alarm month
         */
        ClockAlarm *setMonth(byte month);
        /**
         * Gets the alarm month
         */
        byte getMonth() const;
        /**
         * Sets the alarm day
         */
        ClockAlarm *setDay(byte day);
        /**
         * Gets the alarm day
         */
        byte getDay() const;
        /**
         * Sets if the alarm is active on Sunday
         */
        ClockAlarm *setSunday(byte activeSunday);
        /**
         * Gets if the alarm is active on Sunday
         */
        byte getSunday() const;
        /**
         * Sets if the alarm is active on Monday
         */
        ClockAlarm *setMonday(byte activeMonday);
        /**
         * Gets if the alarm is active on Monday
         */
        byte getMonday() const;        
        /**
         * Sets if the alarm is active on Tuesday
         */
        ClockAlarm *setTuesday(byte activeTuesday);
        /**
         * Gets if the alarm is active on Tuesday
         */
        byte getTuesday() const;
        
        /**
         * Sets if the alarm is active on Wednesday
         */
        ClockAlarm *setWednesday(byte activeWednesday);
        /**
         * Gets if the alarm is active on Wednesday
         */
        byte getWednesday() const;
        
        /**
         * Sets if the alarm is active on Thursday
         */
        ClockAlarm *setThursday(byte activeThursday);
        /**
         * Gets if the alarm is active on Thursday
         */
        byte getThursday() const;
        
        /**
         * Sets if the alarm is active on Friday
         */
        ClockAlarm *setFriday(byte activeFriday);
        /**
         * Gets if the alarm is active on Friday
         */
        byte getFriday() const;
        
        /**
         * Sets if the alarm is active on Saturday
         */
        ClockAlarm *setSaturday(byte activeSaturday);
        /**
         * Gets if the alarm is active on Saturday
         */
        byte getSaturday() const;

        /**
         * Gets the weekdays enabled in the string
         */
        void getWeekdays(char *weekdays);

    private:

        /**
         * If the alarm is active
         */
        byte active;
        /**
         * The song name
         */
        char songName[SONG_NAME_LENGTH];
        /** 
         * If the played song is random from the list of the songs
         * 0 - not random, 1 - is random
         */
        byte songRandom;
        /**
         * The maximum playing time of a song in seconds.
         * After it's reached, the playing is stopped
         */
        int songTime;

        /**
         * How many times the alarm should be repeated after the repeatInterval
         */
        byte repeatTimes;
        /**
         * The interval in minutes between repeats
         */
        byte repeatInterval;
        /**
         * Activate the alarm daily - 0, once tomorrow - 1, specific date - 2
         */
        byte when;
        /**
         * If the alarm is for the selected week days, where false - no alarm, true - alarm
         * 1 is Sunday
         */
        byte week[7];

        /**
         * The year if it plays only once
         */
        unsigned int year;
        /**
         * The month
         */
        byte month;
        /**
         * The day
         */
        byte day;
        
        /**
         * The hour
         */
        byte hour;
        /**
         * The minute
         */
        byte minute;
        /**
         * The second
         */
        byte second;

        /**
         * The When name texts
         */
        static const char whenNames[3][9];

};

#endif
