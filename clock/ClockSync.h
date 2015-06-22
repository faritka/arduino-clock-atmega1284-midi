/**
 * The class to update the files on SD from the Internet
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_FILE_UPDATE_H
#define __CLOCK_FILE_UPDATE_H

#include <Arduino.h>
#include <WiFlyHQ.h>
#include <aJSON.h>
#include <SD.h>
#include <Time.h>

#include "config.h"
#include "ClockConfig.h"
#include "ClockFile.h"
#include "ClockInternet.h"
#include "ClockMusic.h"
#include "ClockScreen.h"


class ClockFileInfo
{
    public:
        unsigned long size;
        String name;
        String url;
};

class ClockSync
{
    public:
        //Constructor
        ClockSync(ClockConfig *clockConfig, ClockMusic *clockMusic, ClockScreen *screen);

        /**
         * Updates the files
         */
        void update();

    private:
        //stores the config object
        ClockConfig *config;

        //stores the music object
        ClockMusic *music;

        //stores the screen object
        ClockScreen *screen;

        /**
         * Connects to the server and gets the files
         */
        boolean getFromServer();

        /**
         * Updates the file
         */
        boolean updateFile(ClockFileInfo &fileInfo, ClockInternet &internet, WiFly &wifi, const char *site);

        /**
         * Deletes old files that are not on the server anymore
         */
        void deleteOldFiles(File &dir, ClockFileInfo *files, int numberOfFiles);

        /**
         * The number of unsuccessful connection tries
         */
        byte updateCounter;

};

#endif
