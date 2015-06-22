/**
 * The generic interface for a song
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_SONG_H
#define __CLOCK_SONG_H 
 
#include <Arduino.h> 
 
class ClockSong
{
    public:
        ClockSong(const char *songName)
        {
            strncpy(name, songName, SONG_NAME_LENGTH);
        };

        /**
           * Plays the song  
         */
        virtual bool play(const int songTime) = 0;

        /**
         * Stops playing
         */
        virtual void stop() = 0;

        /**
         * The song name
         */
        char name[SONG_NAME_LENGTH];

};
  
#endif 
