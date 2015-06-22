#include "ClockTime.h"

/**
 * Constructor
 */
ClockTime::ClockTime(ClockConfig *clockConfig, ClockMusic *clockMusic) : config(clockConfig), music(clockMusic)
{
    // the function to get the time from the RTC
      setSyncProvider(RTC.get);
      if (timeStatus() != timeSet) {
#ifdef DEBUG
         Serial.println(F("Unable to sync the time with RTC"));
#endif
    }
      else {
#ifdef DEBUG
         Serial.println(F("RTC has set the system time"));
#endif
    }

    localTimeZone = NULL;
    setTimezone();

    lastNtpUpdate = lastUnsuccessfulNtpUpdate = 0;
    ntpUpdateCounter = 0;

    calculateNextAlarm();    

    runWatchdog();

    //RTC.set(1333860500);
}

const char ClockTime::monthNames[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", 
    "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Sunday is 1
const char ClockTime::weekdayNames[8][3] = {"", "Su", "Mo", "Tu", "We", 
    "Th", "Fr", "Sa"};

ClockTime *ClockTime::setTimezone()
{
    if (localTimeZone) {
        delete localTimeZone;
    }

    localTimeZone = new Timezone(config->daylightTimeStart, config->standardTimeStart);

    return this;
}

ClockTime *ClockTime::runWatchdog()
{
    //use the internal pullup resistor to set the pin high
    //LOW is alarm interrupt
    pinMode(WATCHDOG_PIN, INPUT_PULLUP);

    //set the first alarm
    RTC.setAlarm(ALM1_MATCH_MINUTES, WATCHDOG_ALARM1_MINUTES, 0, 0);
    //set the second alarm for 30 minutes later
    RTC.setAlarm(ALM2_MATCH_MINUTES, WATCHDOG_ALARM2_MINUTES, 0, 0);

    //enable the alarm interrupts
    RTC.alarmInterrupt(1, 1);
    RTC.alarmInterrupt(2, 1);

    resetWatchdog();
#ifdef DEBUG    
    Serial.println(F("Initialized RTC watchdog"));
#endif
    
    return this;
}

void ClockTime::resetWatchdog()
{
    //reads and resets the status of alarms
    RTC.alarm(1);
    RTC.alarm(2);

#ifdef DEBUG
    Serial.println(F("Resetting RTC alarms"));
#endif
}

time_t ClockTime::getCurrentTime()
{
    utc = now();

    //according to the local timezone    
    currentTime = localTimeZone->toLocal(utc);

    return currentTime;
}

ClockTime *ClockTime::saveCurrentTime()
{
#ifdef DEBUG
    Serial.print(F("Saving the current time into RTC: "));
    Serial.println(currentTime);
#endif

    //convert it into the UTC time
    time_t utcTime = localTimeZone->toUTC(currentTime);

#ifdef DEBUG
    Serial.print(F("UTC time: "));
    Serial.println(utcTime);
#endif

    //now write into the RTC chip
    RTC.set(utcTime);
    //synchronize the function now()
    setTime(utcTime);

    return this;
}


ClockTime *ClockTime::setHour(int hour)
{
    getCurrentTimeElements();

    //convert the 12-hour format into the 24-hour
    if (config->getHourFormat() == 1) {
        //from 1:00 PM to 11:59 PM  add 12 hours
        if (isPM(currentTime) && (hour > 1) && (hour < 12)) {
            hour += 12;
        }    
        //from 12:00 AM (midnight) to 12:59 AM subtract 12 hours
        else if (isAM(currentTime) && (hour == 12)) {
            hour -= 12;
        }
    }

    currentTimeElements.Hour = hour;

    saveCurrentTimeElements();

    return this;
}

int ClockTime::getHour()
{
    getCurrentTime();

    //12 or 24 hour format
    return ((config->getHourFormat() == 0) ? hour(currentTime) : hourFormat12(currentTime));
}

ClockTime *ClockTime::setMinute(const int minute)
{
    getCurrentTimeElements();
    currentTimeElements.Minute = minute;
    //set the second
    //when the minute changes, the user is usually 1 second late
    currentTimeElements.Second = 1;
    saveCurrentTimeElements();

    return this;
}

int ClockTime::getMinute()
{
    getCurrentTime();

    return (minute(currentTime));
}

ClockTime *ClockTime::setAmPm(const byte amPm)
{
    getCurrentTime();

    //the time is already correct
    if ((!amPm && isAM(currentTime)) || (amPm && isPM(currentTime))) {
        return this;
    }
    //set the AM time, minus 12 hours
    else if (!amPm) {
        currentTime -= 12 * SECS_PER_HOUR;
    }
    //set the PM time, plus 12 hours
    else if (amPm) {
        currentTime += 12 * SECS_PER_HOUR;
    }

    saveCurrentTime();

    return this;
}

byte ClockTime::getAmPm()
{
    getCurrentTime();

    return (isPM(currentTime));
}

void ClockTime::getAmPmText(char *text, byte newAmPm)
{
    getCurrentTime();
    byte value = (newAmPm != -1) ? newAmPm : isPM(currentTime);

    strcpy(text, (value == 1) ? "pm" : "am");
}


ClockTime *ClockTime::setYear(int year)
{
    getCurrentTimeElements();
    currentTimeElements.Year = year - 1970;
    saveCurrentTimeElements();

    return this;
}

int ClockTime::getYear()
{
    getCurrentTime();

    return (year(currentTime));
}

ClockTime *ClockTime::setMonth(const int month)
{
    getCurrentTimeElements();
    currentTimeElements.Month = month;
    saveCurrentTimeElements();

    return this;
}

int ClockTime::getMonth()
{
    getCurrentTime();

    return (month(currentTime));
}

ClockTime *ClockTime::setDay(const int day)
{
    getCurrentTimeElements();
    currentTimeElements.Day = day;
    saveCurrentTimeElements();

    return this;
}

int ClockTime::getDay()
{
    getCurrentTime();

    return (day(currentTime));
}

tmElements_t ClockTime::getCurrentTimeElements()
{
    getCurrentTime();

    //set the values of the time structure from the local time
    breakTime(currentTime, currentTimeElements);

    return currentTimeElements;
}


ClockTime *ClockTime::saveCurrentTimeElements()
{
    //convert the elements structure into the UNIX time
    currentTime = makeTime(currentTimeElements);

    saveCurrentTime();
}

ClockTime *ClockTime::calculateNextAlarm()
{
    //if one of alarm has been found
    bool foundAlarm = false;
    //the alarm time
    time_t alarmTime = 0;
    //the time to check alarms against
    time_t checkTime = 0;

    //reset the next alarm
    nextAlarm = 0;

    //get the current time
    checkTime = currentTime = getCurrentTime();

#ifdef DEBUG
    Serial.print(F("Calculating the next alarm. The Current time: "));
    Serial.println(currentTime);
#endif

    //check alarms for the next 7 days including today
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < NUMBER_ALARMS; i++) {
            alarmTime = config->alarm[i].convertToTime(currentTime, checkTime);
#ifdef DEBUG
            Serial.print(F("Alarm number: "));
            Serial.print(i);
            Serial.print(F(", Alarm time: "));
            Serial.println(alarmTime);
#endif

            //set the closest alarm
            if (alarmTime) {
                foundAlarm = true;
                 if (!nextAlarm || (alarmTime < nextAlarm)) {
                    nextAlarm = alarmTime;
                    nextAlarmNumber = i;
#ifdef DEBUG
                    Serial.print(F("Setting the next alarm: "));
                    Serial.println(nextAlarm);
#endif
                }
            }
            //if the alarm was for tomorrow and has been played, deactivate it
            else if ((config->alarm[i].getWhen() == ALARM_WHEN_TOMORROW) && config->alarm[i].getActive()) {
                config->alarm[i].setActive(0);
                config->saveConfig();
#ifdef DEBUG
                Serial.println(F("Tomorrow's alarm was deactivated"));
#endif
            }
        }

        if (foundAlarm) {
            break;
        }

        //if an alarm not found for the day, try the next day
        checkTime += SECS_PER_DAY;
    }

    if (!foundAlarm) {
        nextAlarm = 0;
        nextAlarmNumber = 0;
    }

    return this;
}

ClockTime *ClockTime::checkAlarm()
{
    if (nextAlarm && (nextAlarm <= currentTime)) {
#ifdef DEBUG
        Serial.println(F("The alarm is playing"));
#endif
        //play the alarm music
        music->playSong(config->alarm[nextAlarmNumber].getSongName(), 
            config->alarm[nextAlarmNumber].getSongTime(), config->alarm[nextAlarmNumber].getSongRandom());

        //now find the new alarm time
        calculateNextAlarm();
    }

    return this;
}


time_t ClockTime::getTomorrow(unsigned int &year, byte &month, byte &day)
{
    tmElements_t tm;

    //calculate the tomorrow's UNIX time
    time_t nextDay = getCurrentTime() + SECS_PER_DAY;

    //set the values of the time structure from the check time
    breakTime(nextDay, tm);

    year = tm.Year + 1970;
    month = tm.Month;
    day = tm.Day;

    return nextDay;
}

ClockTime *ClockTime::updateNtpTime()
{
    //two subsequent requests
    time_t ntpTime1, ntpTime2 = 0;
    //the number of tries before giving up for the NTP_UPDATE_HOURS
    const byte numberOfTries = 5;
    //when to try to connect again after unsuccessful tries
    const unsigned long tryAgain = 30 * SECS_PER_MIN;

    //no Internet
    if (!config->getInternetEnabled()) {
        return this;
    }

    //try to connect after some time again
    if ((currentTime - lastUnsuccessfulNtpUpdate) > tryAgain) {
        ntpUpdateCounter = 0;
    }

    if ((ntpUpdateCounter < numberOfTries) 
        && (!lastNtpUpdate 
            || ((currentTime - lastNtpUpdate) > NTP_UPDATE_HOURS * SECS_PER_HOUR))) {
        ClockNtp clockNtp(config);

        //connect to the NTP server and get the current UTC time
        ntpTime1 = clockNtp.getTime();
        
        //don't trust the first request, try again
        if (ntpTime1) {
            ntpTime2 = clockNtp.getTime();
        }

        if (ntpTime1 && ntpTime2 
            //the second time should be greater than the first one
            && (ntpTime2 > ntpTime1)
            //if two times are within 5 minutes
            && ((ntpTime2 - ntpTime1) < 5 * SECS_PER_MIN)) {
            //now write into the RTC chip
            RTC.set(ntpTime2);
            //synchronize the function now()
            setTime(ntpTime2);

            //save the last NTP update time
            lastNtpUpdate = localTimeZone->toLocal(ntpTime2);
            //sets the counter to zero because the try was successful
            ntpUpdateCounter = 0;

#ifdef DEBUG
            Serial.print(F("Current time was synchronized from NTP server at: "));
            Serial.println(lastNtpUpdate);
#endif
            //now find the new alarm time
            calculateNextAlarm();
        }
        else {
            ntpUpdateCounter++;
            lastUnsuccessfulNtpUpdate = currentTime;
        }
    }

    return this;
}
