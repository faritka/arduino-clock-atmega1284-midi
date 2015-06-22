#include "ClockNtp.h"

/**
 * Constructor
 */
ClockNtp::ClockNtp(ClockConfig *clockConfig) : config(clockConfig)
{
}

time_t ClockNtp::getTime()
{
    //NTP time stamp is in the first 48 bytes of the message
    const int NTP_PACKET_SIZE = 48;
    //the NTP port
    const int NTP_PORT = 123;
    //the timeout value
    const unsigned long TIMEOUT = 30000UL;
    
    unsigned long ntpTime = 0;
    //contains the IP of the NTP server
    char ntpIP[32];
    byte packetBuffer[NTP_PACKET_SIZE];
    time_t currentTime = 0;

    //connect to the internet
    ClockInternet internet(config);
    WiFly wifi = internet.connect();


    //if not associated, the connection has failed 
    if (!wifi.isAssociated()) {
#ifdef DEBUG
        Serial.println(F("The Wi-Fi is not associated"));
#endif
        return 0;
    }

    if (wifi.isConnected()) {
        wifi.close();
    }

    //to test the WiFly settings
    //To enter the command mode, set the Arduino terminal to "No line ending"
    //Enter $$$. Then change the terminal to "Carriage return"
    //wifi.terminal();

    //create an NTP request
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    
    //init values needed to form an NTP request
    //LI, Version, Mode
    packetBuffer[0]     = B11100011;
    //Stratum or type of clock
    packetBuffer[1]     = 0;
    //polling interval
    packetBuffer[2]     = 6;
    //peer clock precision
    packetBuffer[3]     = 0xEC;
    //8 bytes of zero for Root Delay and Root Precision
    packetBuffer[12]    = 49;
    packetBuffer[13]    = 0x4E;
    packetBuffer[14]    = 49;
    packetBuffer[15]    = 52; 

    //now get the IP address of the NTP server
    if (wifi.getHostByName(config->getNtpServer(), ntpIP, sizeof(ntpIP))) {
#ifdef DEBUG
        Serial.print(F("NTP server IP address from hostname: "));
#endif
    }
    //sets the default NTP server IP address
    else {
#ifdef DEBUG
        Serial.println(F("Using default NTP IP: "));
#endif
        strncpy(ntpIP, NTP_SERVER_IP_DEFAULT, sizeof(ntpIP));
    }
#ifdef DEBUG        
    Serial.println(ntpIP);
#endif    
    
    //set the new NTP server IP address
    if (!wifi.setHost(ntpIP, NTP_PORT)) {
#ifdef DEBUG
        Serial.print(F("Error setting IP: "));
        Serial.println(ntpIP);    
#endif        
        return 0;
    }

    //init the UDP protocol
    if (!wifi.setIpProtocol(WIFLY_PROTOCOL_UDP)) {
#ifdef DEBUG
        Serial.println(F("Error setting UDP protocol"));
#endif                
        return 0;
    }
    
    //write the request
    if (wifi.write(packetBuffer, NTP_PACKET_SIZE) < NTP_PACKET_SIZE) {
#ifdef DEBUG
        Serial.println(F("Error sending NTP packet"));
#endif        
        return 0;    
    }
    
    //read the response
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    if (!internet.read(packetBuffer, sizeof(packetBuffer), TIMEOUT)) {
        return 0;
    }

    //the timestamp starts at the byte 40 of the received packet and it is four bytes
    for (int i = 0; i < 4; i++) {
        ntpTime = ntpTime << 8 | packetBuffer[40 + i];
    }

    //now convert the NTP time into the UNIX time
    //subtract seventy years
    currentTime = ntpTime - 2208988800UL;

    //the time range is 2014-01-01 to 2100-01-01
    if ((currentTime < 1388534400UL) || (currentTime > 4102444800UL)) {
        return 0;
    }

#ifdef DEBUG	
    Serial.print(F("NTP response. Seconds since Jan 1900: "));
    Serial.println(ntpTime);
    Serial.print(F("Unix UTC time from NTP: "));
    Serial.println(currentTime);
#endif	
    
    return currentTime;
}

