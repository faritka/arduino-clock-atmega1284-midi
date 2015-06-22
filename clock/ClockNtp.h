/**
 * The class to update the time from the NTP server
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_NTP_H
#define __CLOCK_NTP_H

#include <Arduino.h>
#include <Time.h>
#include <WiFlyHQ.h>

#include "config.h"
#include "ClockConfig.h"
#include "ClockInternet.h"


class ClockNtp
{
    public:

        //Constructor
        ClockNtp(ClockConfig *clockConfig);

        /**
         * Gets the UTC time in the chip from the NTP server
         */
        time_t getTime();

    private:
        //stores the config object
        ClockConfig *config;

};

#endif
