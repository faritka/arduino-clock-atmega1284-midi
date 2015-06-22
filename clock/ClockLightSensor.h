/**
 * The class to read the light sensor value 
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_LIGHT_SENSOR_H
#define __CLOCK_LIGHT_SENSOR_H

#include <Arduino.h>
#include <Time.h>

#include "config.h"
#include "ClockConfig.h"

//forward declaration
class ClockScreen;

class ClockLightSensor
{
    public:            
        //Constructor
        ClockLightSensor(ClockConfig *clockConfig);        

        /**
         * Calibrates the light sensor by reading 
         * the minimum and maximum values
         * and storing them in the EEPROM.
         * The sensor must be exposed to the bright light 
         * and then covered.
         */
        void calibrate(ClockScreen *screen);
        
        /**
         * Gets the screen brightness
         */
        const byte getBrightness() const
        {
            return brightness;
        } 
        
        /**
         * Updates the screen brightness depending on the light in the room
         */
        void update(time_t currentTime);
                
    private:
        /**
         * Stores the config object
         */
        ClockConfig *config;
                
        /**
         * The screen brightness
         */
        byte brightness;
        
        /**
         * The last light sensor update time
         */
        time_t lastUpdate;

};

#endif
