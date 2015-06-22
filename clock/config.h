#ifndef __CONFIG_H
#define __CONFIG_H

//uncomment this define for debugging information printed into Serial
#define DEBUG

//the Serial speed, the same as in the terminal
#define SERIAL_SPEED 115200

//the digital pin for the SD card
#define SD_CHIP_SELECT 4

//for the do-matrix screen
//write data clock input pin
#define DISPLAY_WRITE_DATA 4
//chip select signal input pin
#define DISPLAY_SELECT_SIGNAL 5
//data input pin
#define DISPLAY_DATA_INPUT 6
//chip select clock signal input pin
#define DISPLAY_SELECT_CLOCK 7


//a pair of digital pins for the speaker
#define SPEAKER_1  22
#define SPEAKER_2  23

//buttons
// the pin for the UP button (named MENU on the pcb)
#define BUTTON_PIN_UP 0
// the pin for the ENTER button (named SET)
#define BUTTON_PIN_ENTER 1
// the pin for the DOWN button (named PLUS)
#define BUTTON_PIN_DOWN 3
//A debounce time of 20 milliseconds usually works well for tactile button switches.
#define BUTTON_DEBOUNCE_MS 20
//We define a "long press" to be 1000 milliseconds.
#define BUTTON_LONG_PRESS 1000 


//the number of the possible alarms
#define NUMBER_ALARMS 4

//the file names on the SD card
//the file for music in the RTTTL format for alarms
#define SD_FILE_MUSIC "/rtttl/music.txt"

//the maximum number of songs 
#define NUMBER_SONGS 128
//the maximum song name length
#define SONG_NAME_LENGTH 13

//the directory on the SD card with MIDI songs
#define SD_DIRECTORY_MIDI "/midi"
//the extension of MIDI files
#define MIDI_FILE_EXTENSION ".MID"

//the menu title maximum length
#define MENU_TITLE_LENGTH 8
//the menu value maximum length
#define MENU_VALUE_LENGTH 70
//the menu value maximum length that fits into the screen
#define MENU_VALUE_SCREEN_LENGTH 8

//the maximum NTP server name length
#define NTP_SERVER_LENGTH 32
//the default NTP server IP
#define NTP_SERVER_IP_DEFAULT "129.6.15.28"
//the period in hours between updates
#define NTP_UPDATE_HOURS 3

//the Reset pin of the XBee WiFly device
#define XBEE_RESET_PIN 28

//the Serial1 port baud rate. It is connected to the XBee device
#define SERIAL1_BAUD_RATE 9600
//the maximum length of the Wi-Fi SSID
#define WIFI_SSID_LENGTH 32
//the maximum length of the Wi-Fi key or passphrase
#define WIFI_PASSWORD_LENGTH 64
//the device ID of the XBee WiFly device
#define WIFI_DEVICE_ID "Phi Clock"


//the weather forecast
//the period in hours between weather forecast updates
#define WEATHER_UPDATE_HOURS 1
//the weather API key maximum length
#define WEATHER_API_KEY_LENGTH 32
//the maximum length of the location for the weather forecast
#define WEATHER_LOCATION_LENGTH 32

//the light sensor digital pin on the microprocessor
#define LIGHT_SENSOR_PIN 31
//the period in seconds between the sensor readings
#define LIGHT_SENSOR_UPDATE_SECONDS 15

//the MIDI device digital pin
#define MIDI_PIN 30

//the sound amplifier digital pin on the microprocessor
#define AMPLIFIER_PIN 29

//the SD card syncing server name maximum length
#define SYNC_SERVER_NAME_LENGTH 50
//the SD card syncing URL length of the script on the server
#define SYNC_SERVER_URL_LENGTH 75


//the watchdog interrupt pin
//interrupts come from the RTC when alarms are activated
#define WATCHDOG_PIN 2
//watchdog interrupt number
#define WATCHDOG_INTERRUPT 2

//the watchdog timeout in minutes
#define WATCHDOG_TIMEOUT 10
//minutes between two alarms
#define WATCHDOG_MINUTES_BETWEEN_ALARMS 30
//the first watchdog alarm minutes
#define WATCHDOG_ALARM1_MINUTES 18
//the second watchdog alarm minutes, it must be 30 minutes after the first one
#define WATCHDOG_ALARM2_MINUTES 48


#endif

