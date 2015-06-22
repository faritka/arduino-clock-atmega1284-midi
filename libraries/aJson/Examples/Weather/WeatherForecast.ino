/*
 Sample sketch to parse the weather forecast feed
 
 
 */
#include <aJSON.h>

// function definitions
char* parseJson(char *jsonString) ;

// Json string to parse
//http://free.worldweatheronline.com/feed/weather.ashx?q=los+angeles&format=json&num_of_days=1&key=55222222&cc=no
char jsonString[] = "{ \"data\": { \"request\": [ {\"query\": \"Los Angeles, United States Of America\", \"type\": \"City\" } ],  \"weather\": [ {\"date\": \"2012-09-26\", \"precipMM\": \"0.0\", \"tempMaxC\": \"27\", \"tempMaxF\": \"80\", \"tempMinC\": \"16\", \"tempMinF\": \"62\", \"weatherCode\": \"122\",  \"weatherDesc\": [ {\"value\": \"Overcast\" } ],  \"weatherIconUrl\": [ {\"value\": \"http:\/\/www.worldweatheronline.com\/images\/wsymbols01_png_64\/wsymbol_0004_black_low_cloud.png\" } ], \"winddir16Point\": \"N\", \"winddirDegree\": \"0\", \"winddirection\": \"N\", \"windspeedKmph\": \"18\", \"windspeedMiles\": \"11\" } ] }}";

void setup() {
    Serial.begin(9600);
    Serial.println(jsonString);
    Serial.println("Starting to parse");

    char* value = parseJson(jsonString);

    if (value) {
        Serial.print(F("Successfully Parsed: "));
        Serial.println(value);
    } else {
        Serial.print(F("There was some problem in parsing the JSON"));
    }
}

/**
 * Parse the JSON String. Uses aJson library
 * 
 * Refer to http://hardwarefun.com/tutorials/parsing-json-in-arduino
 */
char* parseJson(char *jsonString) 
{
    char* value;
    int code = 0;

    aJsonObject* root = aJson.parse(jsonString);

    if (root != NULL) {
        Serial.println("Parsed successfully 1 " );
        aJsonObject* data = aJson.getObjectItem(root, "data"); 

        if (data != NULL) {
            Serial.println("Parsed successfully 2 " );
            aJsonObject* weather = aJson.getObjectItem(data, "weather"); 

            if (weather != NULL) {
                Serial.println("Parsed successfully 3 " );
                aJsonObject* forecast1 = aJson.getArrayItem(weather, 0);
                if (forecast1) {
                	Serial.println("Parsed successfully 4 " );
            		aJsonObject* tempMaxC = aJson.getObjectItem(forecast1, "tempMaxC"); 
            		aJsonObject* weatherCode = aJson.getObjectItem(forecast1, "weatherCode"); 
            		
            	
            		if (tempMaxC != NULL) {	
                		Serial.println("Parsed successfully 5 " );
                		value = tempMaxC->valuestring;
                	}	

            		if (weatherCode != NULL) {	
                		Serial.println("Parsed successfully 5a " );
                		code = atoi(weatherCode->valuestring);
                		Serial.print("Weather code");
                		Serial.println(code);
                	}	
                }
            }
        }
    }

    if (value) {
        return value;
    } else {
        return NULL;
    }
}

void loop() {
   // Nothing to do 
   ;
}
