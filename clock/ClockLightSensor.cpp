#include "ClockLightSensor.h"
#include "ClockScreen.h"

/**
 * Constructor
 */
ClockLightSensor::ClockLightSensor(ClockConfig *clockConfig) : config(clockConfig)
{    
#ifdef LIGHT_SENSOR    
    //declare the processor pin for the light sensor as input
    pinMode(LIGHT_SENSOR_PIN, INPUT);
#endif    
    
    //the default brightness
    brightness = config->getBrightnessMin();
    
    lastUpdate = 0;
}

void ClockLightSensor::calibrate(ClockScreen *screen)
{
    //the calibration time
    unsigned long calibrationTime = millis();
    // the sensor value
    int sensorValue = 0;
    // minimum sensor value
    int sensorMin = 1023;
    // maximum sensor value
    int sensorMax = 0;
        
    // calibrate for two minutes
    while ((millis() - calibrationTime) < 120000) {
        sensorValue = analogRead(LIGHT_SENSOR_PIN);
#ifdef DEBUG        
        Serial.print(F("Light Sensor value: "));
        Serial.println(sensorValue);
#endif                    
        screen->showMenuValue(sensorValue, true);

        // record the maximum sensor value
        if (sensorValue > sensorMax) {
            sensorMax = sensorValue;
        }
                              
        // record the minimum sensor value
        if (sensorValue < sensorMin) {
            sensorMin = sensorValue;
        }
        
        delay(400);
    }  

    //save in EEPROM
    config->setLightSensorMin(sensorMin)
        ->setLightSensorMax(sensorMax)
        ->saveConfig();
}


void ClockLightSensor::update(time_t currentTime)
{    
    //update only once a few minutes
    if (!lastUpdate || ((currentTime - lastUpdate) > LIGHT_SENSOR_UPDATE_SECONDS)) {
        // read the value from the sensor
        int sensorValue = analogRead(LIGHT_SENSOR_PIN);
         
        //the brighter the room, the brighter the screen should be     
        // in case the sensor value is outside the range seen during calibration
        sensorValue = constrain(sensorValue, config->getLightSensorMin(), config->getLightSensorMax());
         
        // apply the calibration to the sensor reading
        brightness = map(sensorValue, config->getLightSensorMin(), config->getLightSensorMax(), 
            config->getBrightnessMin(), config->getBrightnessMax());
                             
#ifdef DEBUG                     
        //Serial.print(F("Sensor Value: "));
        //Serial.print(sensorValue);
        //Serial.print(F(", Screen Brightness: "));
        //Serial.println(brightness);
#endif  
        lastUpdate = currentTime;

    }
}



