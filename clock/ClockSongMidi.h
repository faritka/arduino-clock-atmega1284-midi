/**
 * The class to play the MIDI song on FluxamaSynth
 *
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 * @license    GPL 2
 */
#ifndef __CLOCK_SONG_MIDI_H
#define __CLOCK_SONG_MIDI_H
 
#include <Arduino.h> 
#include <MidiPlayer.h>
#include <SD.h>
#include <QueueList.h> 
 
#include "config.h"
#include "ClockSong.h"

class ClockSongMidi : public ClockSong
{
    public:
        ClockSongMidi(const char *songName);

        /**
         * Plays the song
         */
        bool play(const int songTime);

        /**
         * Stops playing
         */
        void stop();

        /**
         * Reads the list of the songs in the MIDI directory on the SD
         * @param QueueList<ClockSong*> songList The song queue
         */
        static byte readSongNames(QueueList<ClockSong*> &songList);

        /**
         * Reads the songs in the directory
         */
        static void readSongDirectory(File dir, QueueList<ClockSong*> &songList);

    private:

        /**
         * The MIDI player object
         */
        MidiPlayer *player;
};
  
#endif 