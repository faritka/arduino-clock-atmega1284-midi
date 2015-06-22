/**
 * The class to read and write a file from an SD card
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_FILE_H
#define __CLOCK_FILE_H

#include <Arduino.h>
#include <SD.h>

#include "config.h"


class ClockFile
{
    public:
            
        //Constructor
        ClockFile();        
        //Destructor
        ~ClockFile();

        //opens the file
        int open(char *filePath, uint8_t mode = FILE_READ);        

        //reads a line from a file
        //all comment lines that start with '#' are skipped
        bool readLine(String &str);
        
        /**
         * Writes a character to the file
         */
        byte write(char ch)
        {
            file.write(ch);
        }
        
        /**
         * Closes the file
         */
        void close()
        {
            if (file) {
                file.close();
#ifdef DEBUG    
                Serial.println(F("The file on SD was closed"));
#endif                        
            }
        }         
                
    private:
        File file;        
};

#endif
