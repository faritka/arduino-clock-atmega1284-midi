#include "ClockFile.h"

/**
 * Constructor
 */
ClockFile::ClockFile() 
{
}

/**
 * Destructor
 */
ClockFile::~ClockFile()
{
    close();
}


int ClockFile::open(char *filePath, uint8_t mode)
{ 
    if ((mode == FILE_READ) && !SD.exists(filePath)) {
#ifdef DEBUG
        Serial.print(F("The file doesn't exist on SD or unreadable: "));
        Serial.println(filePath);
#endif        
        return 1;
    }    
    
    //remove the file in the write mode
    //by default, files are opened in the append mode
    if ((mode == FILE_WRITE) && SD.exists(filePath)) {
        SD.remove(filePath);
#ifdef DEBUG
        Serial.print(F("The file was deleted: "));
        Serial.println(filePath);
#endif        
    }

    file = SD.open(filePath, mode);
    if (!file) {
#ifdef DEBUG
        Serial.print(F("The file is unreadable on SD: "));
        Serial.println(filePath);
#endif        
        
        return 1;    
    }
    
    return 0;
}

bool ClockFile::readLine(String &str)
{
    char c;
    str = "";
 
    if (!file.available()) {
        return false;  
    }
  
    while (file.available()) {
        c = file.read();
        str += c;
        //the line break
        if ((c == '\n') || (c == '\r')) {
              //skip comments
              if (str.startsWith("#") || str.startsWith("//")) {
                str = "";
                continue;
              }  
      
              //trim carriage returns
              //str.replace("\n", "");
              //str.replace("\r", "");
              //str.trim();
      
              if (str.length() > 2) {
                    break;
              }
        }
    }  
      
    return (str.length() ? true : false);
}



