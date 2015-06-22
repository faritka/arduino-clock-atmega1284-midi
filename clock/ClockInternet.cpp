#include "ClockInternet.h"

/**
 * Constructor
 */
ClockInternet::ClockInternet(ClockConfig *clockConfig) : config(clockConfig), wifi()
{
    /**
     * The Wi-Fi Xbee module is connected to Serial1 of the microprocessor
     */
    Serial1.begin(SERIAL1_BAUD_RATE);
}

/**
 * Destructor
 */
ClockInternet::~ClockInternet()
{
    if (wifi.isConnected()) {
        wifi.close();
    }
} 

void ClockInternet::reset(ClockConfig *clockConfig)
{
    if (!clockConfig->getInternetEnabled()) {
        return;
    }

#ifdef DEBUG
    Serial.println(F("Resetting the Wi-Fi device"));
#endif

    //sets the Reset pin on the XBee device low and then high
    pinMode(XBEE_RESET_PIN, OUTPUT);
    digitalWrite(XBEE_RESET_PIN, LOW);
    delay(50);
    digitalWrite(XBEE_RESET_PIN, HIGH);

    //it may take seconds for XBee to reset
    delay(1000);
}

/**
 * Connects to the Wireless Internet
 */
WiFly &ClockInternet::connect()
{
#ifdef DEBUG    
    Serial.println(F("Starting Wi-Fi Internet connection"));
#endif    


    if (!wifi.begin(&Serial1, &Serial)) {
#ifdef DEBUG    
        Serial.println(F("Failed to start WiFly"));
#endif        
        return wifi;
    }

    // Join wifi network if not already associated 
    if (!wifi.isAssociated()) {
        // Setup the WiFly to connect to a wifi network
        wifi.setSSID(config->getWifiSsid());
        if (config->getWifiEncryption() == 0) {        
            //WPA encryption
            wifi.setPassphrase(config->getWifiPassword());
        }
        else {
            //WEP encryption
            wifi.setKey(config->getWifiPassword());
        }
    
        //WPA encryption
        //wifi.setPassphrase(config->getWifiPassword());
        wifi.enableDHCP();
        wifi.setDeviceID(WIFI_DEVICE_ID);
        //auto channel
        wifi.setChannel(0);
        //the transmission rate
        //wifi.setRate(200000);
        wifi.save();


        if (wifi.join()) {
#ifdef DEBUG        
            Serial.println(F("Joined the wifi network"));
#endif            
        } 
        else {
            //will try to connect again after the reboot
            wifi.reboot();
#ifdef DEBUG        
            Serial.println(F("Failed to join the wifi network"));
#endif                        
            return wifi;
        }
    } 
    else {
#ifdef DEBUG    
        Serial.println(F("Already joined network"));
#endif        
    }

#ifdef DEBUG
    char buf[32];

    Serial.print(F("MAC: "));
    Serial.println(wifi.getMAC(buf, sizeof(buf)));
    Serial.print(F("IP: "));
    Serial.println(wifi.getIP(buf, sizeof(buf)));
    Serial.print(F("Netmask: "));
    Serial.println(wifi.getNetmask(buf, sizeof(buf)));
    Serial.print(F("Gateway: "));
    Serial.println(wifi.getGateway(buf, sizeof(buf)));

    Serial.print(F("DeviceID: "));
    Serial.println(wifi.getDeviceID(buf, sizeof(buf)));
#endif
    
    return wifi;
}

/*
bool ClockInternet::readResponse(WiFly &wifi, byte *buf, const int size, const unsigned long timeout)
{
    unsigned long startTime = millis();
    byte receivedByte = 0;

    while ((wifi.available() < size) && ((millis() - startTime) < timeout)) {
        //hang in this loop until we either get size bytes of data or the timeout has gone by
    }

    if (wifi.available() < size) {
        // the data didn't come in - handle that problem here
#ifdef DEBUG        
        Serial.println(F("Wi-Fi reading error. Didn't receive data from the server"));
#endif        

        return false;
    }
    else {
        //read each byte
        for (int i = 0; i < size; i++) {
            receivedByte = wifi.read();
            //an error while reading
            if (receivedByte == -1) {
                return false;
            }
            buf[i] = receivedByte;
            //Serial.print("Char [");
            //Serial.print(i);
            //Serial.print("]: ");
            //Serial.println(receivedByte);
        }
    }

    wifi.flushRx(timeout);

    return true;
}
*/

/**
 * Encodes the URL for the GET parameter
 */
String ClockInternet::urlencode(const String &url)
{   
    String escaped = "";
    int len = url.length();
    for(int i = 0; i < len; i++) {
        //0-9
        if (((48 <= url[i]) && (url[i] <= 57))
            //abc...xyz
             || ((65 <= url[i]) && (url[i] <= 90))
             //ABC...XYZ
             || ((97 <= url[i]) && (url[i] <= 122)) 
             || (url[i] == '~') || (url[i] == '!') || (url[i] == '*') 
             || (url[i] == '(') || (url[i] == ')') || (url[i] == '\'')) {
            escaped += url[i];
        }
        else {
            escaped += "%" + char2hex(url[i]);
        }
    }
    return escaped;
}

String ClockInternet::char2hex(char dec)
{
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);
    if ((0 <= dig1) && (dig1 <= 9)) {
        //0,48inascii
        dig1 += 48;
    }
    if ((10 <= dig1) && (dig1 <= 15)) {
        //a,97inascii
        dig1 += 97 - 10; 
    } 
    if ((0 <= dig2) && (dig2 <= 9)) {
        dig2 += 48;
    }
    if ((10 <= dig2) && (dig2 <= 15)) {
        dig2 += 97-10;
    }

    return (String(dig1) + String(dig2));
}

boolean ClockInternet::readByte(byte *buf, unsigned long timeout)
{
    unsigned long start = millis();
    int readBuf = 0;

    while ((millis() - start) < timeout) {
        if (wifi.available() > 0) {
            readBuf = wifi.read();
            if (readBuf == -1) {
                continue;
            }
            *buf = (byte)readBuf;
                        
            return true;
        }
    }

#ifdef DEBUG
    Serial.println(F("Timeout reading a byte"));
#endif

    return false;
}

int ClockInternet::read(byte *buf, const unsigned int bufSize, const unsigned long timeout)
{
    int readBytes = 0;
    byte readBuf = 0;

    while (readByte(&readBuf, timeout)) {
        buf[readBytes++] = readBuf;
        
        //the buffer is full
        if (readBytes >= bufSize) {
            break;
        }
    }

    return readBytes;
}
