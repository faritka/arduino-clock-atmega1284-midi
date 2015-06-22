/**
 * The class to get the weather forecast via the Internet
 *
 * Uses the API from http://worldweatheronline.com
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_WEATHER_H
#define __CLOCK_WEATHER_H

#include <Arduino.h>
#include <WiFlyHQ.h>
#include <aJSON.h>
#include <Time.h>
#include <MemoryFree.h>

#include "config.h"
#include "ClockConfig.h"
#include "ClockFile.h"
#include "ClockInternet.h"

//temperature scales
#define WEATHER_CELSIUS 0
#define WEATHER_FAHRENHEIT 1

/**
 * Each day of the forecast
 */
class ClockForecastDay
{
  public:
        ClockForecastDay();
  
        /**
         * The maximum temperature forecast in Celcius
         */
        int tempMaxC;
        /**
         * The maximum temperature forecast in Fahrenheit
         */
        int tempMaxF;

        /**
         * The minimum temperature forecast in Celcius
         */
        int tempMinC;
        /**
         * The minimum temperature forecast in Fahrenheit
         */
        int tempMinF;

        /**
         * The weather code forecast
         * See http://www.worldweatheronline.com/free-weather-feed.aspx
         */
        int weatherCode; 
        /**
         * The weather symbol index to be displayed on the screen
         */
        byte symbol; 
    
        /**
         * The weather description
         */
        char description[24]; 

        /**
         * The time when the forecast day starts
         */
        time_t date; 

        /**
         * Inits the values
         */
        void initValues(); 

        /**
         * Converts the text date in the format "2013-01-01" into the unix date
         */
        time_t dateToUnixtime(char *date); 

  private:
   
};

class ClockWeather
{
    public:            
        //Constructor
        ClockWeather(ClockConfig *clockConfig);        

        /**
         * Updated the weather forecast
         */
        void update(time_t currentTime);     
    
        /**
         * Returns the temperature scale name by the number
         * 0 - Celsius, 1 - Fahrenheit
         */
        static const char *getTemperatureScaleName(byte scaleNumber)
        {
            return (ClockWeather::temperatureScaleNames[scaleNumber]);
        }

        /**
         * The three days forecast
         */
        ClockForecastDay forecast[3]; 


    private:
        //stores the config object
        ClockConfig *config;

        /**
         * Inits the values
         */
        void initValues(); 

        /**
         * Connects to the server and gets the weather page
         */
        boolean getFromServer(time_t currentTime);

        /**
         * Parses the JSON object from the wifi stream
         */
        boolean parseJson(Stream *serialStream, time_t currentTime);

        /**
         * Defines the weather symbol depending on the weather code
         */
        void defineWeatherSymbol(ClockForecastDay &forecastDay); 

        /**
         * The last Weather server update time
         */
        time_t lastUpdate;
        /**
         * The last unsuccessful Weather server update time
         */
        time_t lastUnsuccessfulUpdate;
        /**
         * The number of unsuccessful connection tries
         */
        byte updateCounter;

        /**
         * The temperature names: C and F
         */
        static const char temperatureScaleNames[2][11];

};

#endif
