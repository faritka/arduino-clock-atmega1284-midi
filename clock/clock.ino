/**
 * The Arduino project for a clock
 *
 * It uses the PCB from the Wise Clock 4.
 * The main microcontroller is Atmega1284p.
 * The multi-color 32x16 LED display is from Sure Electronics.
 * The time chip is DS3231.
 * It connect to the Internet via a WiFly XBee board.
 * It adjusts the current time via NTP.
 * It gets the weather forecast via the Internet.
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#include <Wire.h>
#include <EEPROM.h>
//watchdog for soft resets
#include <avr/wdt.h>
//for interrupts
#include <avr/interrupt.h>

#include <SD.h>             //matched the pins to the Sanguino format in the local copy
#include <ht1632c.h>        //http://code.google.com/p/ht1632c
#include <Time.h>           //http://www.arduino.cc/playground/Code/Time
#include <Timezone.h>       //https://github.com/JChristensen/Timezone
#include <DS3232RTC.h>      //https://github.com/JChristensen/DS3232RTC
#include <toneAC2.h>        // http://code.google.com/p/arduino-tone-ac/
#include <Button.h>         //https://github.com/JChristensen/Button
#include <MemoryFree.h>
#include <MidiPlayer.h>
#include <SendOnlySoftwareSerial.h>
#include <WiFlyHQ.h>        //https://github.com/harlequin-tech/WiFlyHQ
#include <aJSON.h>          //http://interactive-matter.eu/how-to/ajson-arduino-json-library/
#include <MenuInterface.h>
#include <QueueList.h>

#include "config.h"

#include "ClockAlarm.h"
#include "ClockConfig.h"
#include "ClockFile.h"
#include "ClockInternet.h"
#include "ClockLightSensor.h"
#include "ClockMenu.h"
#include "ClockMusic.h"
#include "ClockNtp.h"
#include "ClockScreen.h"
#include "ClockSync.h"
#include "ClockTime.h"
#include "ClockTimezone.h"
#include "ClockWeather.h"


ClockConfig *config;
ClockLightSensor *clockLightSensor;
ClockMenu *clockMenu;
ClockMusic *clockMusic;
ClockScreen *clockScreen;
ClockTime *clockTime;
ClockWeather *clockWeather;

//saves the previous reading of the current time from RTC
volatile unsigned long previousTime = 0;
//the previous watchdog interrupt time
volatile unsigned long previousWatchdogTime = 0;
//indicates that an interrupt has been triggered
volatile bool watchdogInterruptTriggered = false;
//the previous button click time
volatile unsigned long previousButtonTime = 0;

void setup()
{
    //disable the microprocessor watchdog
    wdt_disable();

#ifdef DEBUG
    Serial.begin(SERIAL_SPEED);
#endif

    // init the SD card
    if (!SD.begin(SD_CHIP_SELECT)) {
#ifdef DEBUG
        Serial.println(F("SD card has failed or is not present"));
#endif
    }

    config = new ClockConfig();
    clockLightSensor = new ClockLightSensor(config);
    clockMusic = new ClockMusic(config);
    clockTime = new ClockTime(config, clockMusic);
    clockScreen = new ClockScreen(config, clockLightSensor);
    clockWeather = new ClockWeather(config);

    clockMenu = new ClockMenu(config, clockScreen, clockTime, clockMusic, clockLightSensor, clockWeather);

    //resets the Wi-Fi device
    ClockInternet::reset(config);

    //enable the watchdog interrupt
    attachInterrupt(WATCHDOG_INTERRUPT, watchdogInterruptHandler, FALLING);
/*    
    EICRA = _BV(ISC21);          //external interrupt on falling edge
    EIFR = _BV(INTF2);           //clear the interrupt flag (setting ISCnn can cause an interrupt)
    EIMSK = _BV(INT2);           //enable external interrupt
*/    
}

void loop()
{
    //clear the interrupt alarms, so they can fire again
    if (watchdogInterruptTriggered) {
        previousWatchdogTime = previousTime;

        clockTime->resetWatchdog();
        watchdogInterruptTriggered = false;
    }

    //check if a button was pressed and process the menu
    clockMenu->checkButtons();

    //read the current time in the local timezone
    time_t currentTime = clockTime->getCurrentTime();

    //change the time on the display only once a second
    if (currentTime != previousTime) {
        //update the light level from the sensor
        clockLightSensor->update(currentTime);

        //if the menu is not active, show the clock
        if (!clockMenu->isActive(currentTime)) {
            //show the current time on the screen
            clockScreen->showTime(currentTime);  
            //show the current date on the screen
            clockScreen->showDate(currentTime);  
            //show the weather forecast
            clockScreen->showWeather(clockWeather, currentTime);
        }

#ifdef DEBUG
//      Serial.print("freeMemory: ");
//      Serial.println(freeMemory());
#endif

        //check the alarm time
        clockTime->checkAlarm();

        //update the time from the NTP server
        clockTime->updateNtpTime();

        //update the weather forecast
        clockWeather->update(currentTime);

        previousTime = currentTime;
    }

}

/**
 * Processes watchdog interrupts coming from the RTC
 * If the system hang up, restarts the microprocessor
 */
void watchdogInterruptHandler()
{
    watchdogInterruptTriggered = true;

    //the watchdog runs periodically 
    //if the interrupt has been called at least once
    if ((previousWatchdogTime > 1414000000UL)
        //if the previousTime hasn't been updated for long time, the loop() might have stalled
        //give 10 minutes for the longest process in loop()
        && ((previousWatchdogTime + (WATCHDOG_MINUTES_BETWEEN_ALARMS - WATCHDOG_TIMEOUT) * 60) > previousTime)) {
        //software restart
        wdt_enable(WDTO_8S);
    }
}

/**
 * Processes the button interrupts
 */ 
ISR(PCINT1_vect)
{
#ifdef DEBUG
    Serial.println("B");
#endif
    //debounce button clicks
    if ((millis() - previousButtonTime) <= ClockMenu::debouncingTime) {
        return;
    }

    //stop alarm music if it plays
    if (clockMusic->isPlaying()) {
        clockMusic->stop();
    }
    //change the menu
    else {
        
    }

    previousButtonTime = millis();
} 
