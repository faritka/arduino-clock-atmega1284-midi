/**
 * The class to connect to the Internet via the WiFly device
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_INTERNET_H
#define __CLOCK_INTERNET_H

#include <Arduino.h>
#include <WiFlyHQ.h>

#include "config.h"
#include "ClockConfig.h"

class ClockInternet
{
    public:
            
        //Constructor
        ClockInternet(ClockConfig *clockConfig);
        //Destructor
        ~ClockInternet();

        /**
         * Connect to the internet
         *
         * @return WiFly wifi
         */
        WiFly &connect();

        /**
         * Resets the WiFly device
         */
        static void reset(ClockConfig *clockConfig);

        /**
         * Reads response into the buffer
         */
        //bool readResponse(WiFly &wifi, byte *buf, const int size, const unsigned long timeout = defaultTimeout);

#ifdef DEBUG
        /**
         * The terminal for manual interaction with WiFly
         * To enter the command mode, set the Arduino terminal to "No line ending"
         * Enter $$$. Then change the terminal to "Carriage return"
         */
        void terminal() 
        {
            wifi.terminal();
        }; 
#endif

        /**
         * Encodes the URL for the GET parameter
         * @param String url
         */
        static String urlencode(const String &url);

        /** Reads the next character from the WiFly serial interface.
         * Waits up to timeout milliseconds to receive the character.
         * @param chp pointer to store the read character in
         * @param timeout the number of milliseconds to wait for a character
         * @retval true - character read
         * @retval false - timeout reached, character not read
         */
        boolean readByte(byte *buf, unsigned long timeout = defaultTimeout);
        
        /**
         * Reads next bytes from the WiFly interface
         *
         * @param buf The buffer to store the result
         * @param bufSize The size of the buffer
         *
         * @return int The number of read bytes
         */ 
        int read(byte *buf, const unsigned int bufSize, const unsigned long timeout = defaultTimeout);


    private:
        /**
         * Stores the config object
         */
        ClockConfig *config;
    
        /**
         * The WiFly object
         */
        WiFly wifi;         
    
        /**
         * Converts the character to hex
         */        
        static String char2hex(char dec);

        /**
         * The default timeout
         */
        const static unsigned long defaultTimeout = 60000UL;
};

#endif
