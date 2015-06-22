#include "ClockAlarm.h"

/**
 * Constructor
 */
ClockAlarm::ClockAlarm() : week {1, 1, 1, 1, 1, 1, 1}
{
    //set the default values
    active = 0;
    //the song on SD
    strncpy(songName, "Korobeyniki", SONG_NAME_LENGTH);
    //the song is not random
    songRandom = 0;
    //the maximum playing time is 1 minute
    songTime = 60;

    repeatTimes = 0;
    repeatInterval = 5;
    when = 0;
    //week = {1, 1, 1, 1, 1, 1, 1};

    year = 2015;
    month = 1;
    day = 1;
    hour = 7;
    minute = 0;
    //set it a second later, so the alarm is triggered after the screen was updated
    second = 1;
}

const char ClockAlarm::whenNames[3][9] = {"Daily", "Tomorrow", "Date"};


ClockAlarm *ClockAlarm::setActive(byte newActive)
{
    active = newActive;		
    return this;
}

byte ClockAlarm::getActive() const
{
    return active;
}

ClockAlarm *ClockAlarm::setSongName(const char *newSongName)
{
    strncpy(songName, newSongName, SONG_NAME_LENGTH);
    return this;
}

const char *ClockAlarm::getSongName() const
{
    return songName;
}

ClockAlarm *ClockAlarm::setSongRandom(byte newSongRandom)
{
    songRandom = newSongRandom;
    return this;	
}
byte ClockAlarm::getSongRandom() const
{
    return songRandom;
}

ClockAlarm *ClockAlarm::setSongTime(int newSongTime)
{
    songTime = newSongTime;
    return this;	
}
int ClockAlarm::getSongTime() const
{
    return songTime;
}

ClockAlarm *ClockAlarm::setRepeatTimes(byte newRepeatTimes)
{
    repeatTimes = newRepeatTimes;		
    return this;
}
byte ClockAlarm::getRepeatTimes() const
{
    return repeatTimes;
}
			 
ClockAlarm *ClockAlarm::setRepeatInterval(byte newRepeatInterval)
{
    repeatInterval = newRepeatInterval;
    return this;	
}
byte ClockAlarm::getRepeatInterval() const
{
    return repeatInterval;
}

ClockAlarm *ClockAlarm::setWhen(byte newWhen)
{
    when = newWhen;

    return this;
}
byte ClockAlarm::getWhen() const
{
    return when;
}

ClockAlarm *ClockAlarm::setHour(byte newHour)
{
    hour = newHour;
    return this;	
}
byte ClockAlarm::getHour(const byte hourFormat) const
{
    //24-hour format
    if (hourFormat == 0) {
        return hour;
    }
    //12-hour format
    // 12 midnight
    else if (hour == 0) {
        return 12; 
    }    
    else if (hour > 12) {
        return (hour - 12);
    }
    else {
        return hour;
    }
}

ClockAlarm *ClockAlarm::setMinute(byte newMinute)
{
    minute = newMinute;
    return this;
}
byte ClockAlarm::getMinute() const
{
    return minute;
}

ClockAlarm *ClockAlarm::setAmPm(byte amPm)
{
    //the time is already correct
    if ((!amPm && !getAmPm()) || (amPm && getAmPm())) {
        return this;
    }
    //set the AM time, minus 12 hours
    else if (!amPm) {
        hour -= 12;
    }
    //set the PM time, plus 12 hours
    else if (amPm) {
        hour += 12;
    }

    return this;	
}

byte ClockAlarm::getAmPm() const
{		
    return (hour >= 12);
}


ClockAlarm *ClockAlarm::setSecond(byte newSecond)
{
    second = newSecond;
    return this;	
}
byte ClockAlarm::getSecond() const
{
    return second;
}

ClockAlarm *ClockAlarm::setYear(unsigned int newYear)
{
    year = newYear;
    return this;
}
unsigned int ClockAlarm::getYear() const
{
    return year;
}

ClockAlarm *ClockAlarm::setMonth(byte newMonth)
{
    month = newMonth;
    return this;
}
byte ClockAlarm::getMonth() const
{
    return month;
}
ClockAlarm *ClockAlarm::setDay(byte newDay)
{
    day = newDay;
    return this;
}
byte ClockAlarm::getDay() const
{
    return day;
}

ClockAlarm *ClockAlarm::setSunday(byte activeSunday)
{
    week[0] = activeSunday;
    return this;	
}
byte ClockAlarm::getSunday() const
{
    return week[0];
}
ClockAlarm *ClockAlarm::setMonday(byte activeMonday)
{
    week[1] = activeMonday;
    return this;
}
byte ClockAlarm::getMonday() const
{
    return week[1];
}
ClockAlarm *ClockAlarm::setTuesday(byte activeTuesday)
{
    week[2] = activeTuesday;
    return this;
}
byte ClockAlarm::getTuesday() const
{
    return week[2];
}      
ClockAlarm *ClockAlarm::setWednesday(byte activeWednesday)
{
    week[3] = activeWednesday;
    return this;
}
byte ClockAlarm::getWednesday() const
{
    return week[3];
}       
ClockAlarm *ClockAlarm::setThursday(byte activeThursday)
{
    week[4] = activeThursday;
    return this;
}
byte ClockAlarm::getThursday() const
{
    return week[4];
}       
ClockAlarm *ClockAlarm::setFriday(byte activeFriday)
{
    week[5] = activeFriday;
    return this;
}
byte ClockAlarm::getFriday() const
{
    return week[5];
}       
ClockAlarm *ClockAlarm::setSaturday(byte activeSaturday)
{
    week[6] = activeSaturday;
    return this;
}
byte ClockAlarm::getSaturday() const
{
    return week[6];
}

void ClockAlarm::getWeekdays(char *weekdays)
{
    //plus - active
    strcpy(weekdays, "+++++++");

    //display Monday as the first day		
    for (int i = 1, d = 0; i < 7; i++, d++) {
        if (!week[i]) {
            weekdays[d] = '-';
        }    
    }
    //now Sunday
    if (!week[0]) {
        weekdays[6] = '-';
    }
}


time_t ClockAlarm::convertToTime(time_t currentTime, time_t checkTime)
{
    time_t alarmTime;
    tmElements_t tm;

    //not active, no alarm
    if (active == 0) {
#ifdef DEBUG
        Serial.println(F("The alarm is inactive"));
#endif
        return 0;
    }

    //set the values of the time structure from the check time
    breakTime(checkTime, tm);

    //set the time of the alarm for today
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = second;	

    //once on the defined date
    if (when != ALARM_WHEN_DAILY)	{
        tm.Year = year - 1970;		
        tm.Month = month;
        tm.Day = day;
    }
    //check if alarm is active for this week day
    else {
#ifdef DEBUG
        Serial.print(F("Week now: "));
        Serial.println(tm.Wday);	
#endif
        //tm.Wday starts from Sunday = 1, the week array starts from 0
        if (week[tm.Wday - 1] == 0) {
            Serial.println(F("The alarm is inactive for the week day"));
            return 0;
        }
    }

    //recreate the UNIX time
    alarmTime = makeTime(tm);

    //the alarm time has passed
    if (alarmTime < currentTime) {
#ifdef DEBUG
        Serial.print(F("The alarm time is < the current time. The currentTime: "));
        Serial.println(currentTime);
#endif

        //check if the alarm must be repeated after the repeatInterval minutes
        if (repeatTimes > 0) {
            for (int i = 0; i < repeatTimes; i++) {
                alarmTime += SECS_PER_MIN * repeatInterval;
                if (alarmTime >= currentTime) {
                    break;
                }
            }
        }

        //disable the alarm
        if (alarmTime < currentTime) {
            alarmTime = 0;
        }
    }

#ifdef DEBUG
    Serial.print(F("Returning alarmTime: "));
    Serial.println(alarmTime);
#endif

    return alarmTime;
}

