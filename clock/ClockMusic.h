/**
 * The class to play music
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_MUSIC_H
#define __CLOCK_MUSIC_H

#include <Arduino.h>
#include <toneAC2.h>
#include <SD.h>
#include <QueueList.h>

#include "config.h"
#include "ClockConfig.h"
#include "ClockFile.h"
#include "ClockSong.h"
#include "ClockSongRtttl.h"
#include "ClockSongMidi.h"

class ClockMusic
{
    public:
        //Constructor
        ClockMusic(ClockConfig *clockConfig);
        ~ClockMusic();

        /**
         * Plays the song
         *
         * @param char *songName The song name
         * @param int songTime The maximum song playing time
         * @param byte songRandom If 1, play a random song from the list
         */
        void playSong(const char *songName, const int songTime = 60, const byte songRandom = 0);

        /**
         * Checks if a song is played now
         *
         * @return boolean True if a song is played, false if not.
         */
        inline boolean isPlaying()
        {
            return ((playingSongNumber >= 0) ? true : false);
        }

        /**
         * Stops playing the current song
         * @returns boolean True if a song played, false if not.
         */
        boolean stop();

        /**
         * Gets the song name by its number in the song list
         * @param int songNumber The song number, starts from 0
         */
        const char *getSongNameByNumber(const int songNumber) const;
        /**
         * Gets the song number in the song list by its name
         */
        int getSongNumberByName(const char *songName) const;
        /**
         * Gets the number of songs in the song list
         */
        int getNumberSongs() const;

        /**
         * Gets the song list from the SD card
         */
        void getSongList();

    private:
        //stores the config object
        ClockConfig *config;

        /**
         * The number of the song that plays
         */
        int playingSongNumber;

        /**
         * The list of the available songs
         */
        QueueList<ClockSong*> songList; 
};

#endif
