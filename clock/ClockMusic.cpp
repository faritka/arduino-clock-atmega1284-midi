#include "ClockMusic.h"

/**
 * Constructor
 */
ClockMusic::ClockMusic(ClockConfig *clockConfig) : config(clockConfig)
{
    //no active song that is being played
    playingSongNumber = -1;

    //disable the audio amplifier
    pinMode(AMPLIFIER_PIN, OUTPUT);
    digitalWrite(AMPLIFIER_PIN, LOW);

    getSongList();

#ifdef DEBUG
    Serial.println(F("Songs read:"));
    for (int i = 0; i < songList.count(); i++) {
        Serial.print(F("Song name: "));
        Serial.println(songList[i]->name);
    }
#endif
}

ClockMusic::~ClockMusic()
{
    //destroy the contents of the queue
    for (int i = 0; i < songList.count(); i++) {
        delete songList[i];
    }
}

void ClockMusic::getSongList()
{
    //destroy the contents of the queue
    for (int i = 0; i < songList.count(); i++) {
        delete songList[i];
    }

    //read the list of the MIDI songs on SD
    ClockSongMidi::readSongNames(songList);

    //read the list of the RTTTL songs on SD
    ClockSongRtttl::readSongNames(songList);
}

const char *ClockMusic::getSongNameByNumber(const int songNumber) const
{
    //songNumber starts from 0
    if (songNumber >= songList.count()) {
        return NULL;
    }

    return (songList[songNumber]->name);
}

int ClockMusic::getSongNumberByName(const char *songName) const
{
    int songNumber = 0;
    if (!strlen(songName)) {
        return songNumber;
    }
    
    for (int i = 0; i < songList.count(); i++) {
        if (strcasecmp(songList[i]->name, songName) == 0) {
            songNumber = i;
            break;
        }    
    }
    
    return songNumber;
}

int ClockMusic::getNumberSongs() const
{
    return songList.count();
}

void ClockMusic::playSong(const char *songName, const int songTime, const byte songRandom)
{
    bool songFound = false;
    int songCount = songList.count();
    int i = 0;

    //a random song
    if (songRandom) {
        //get a random song number
        i = random(songCount);
        songFound = true;
    }
    //the song by the name
    else {
        for (i = 0; i < songCount; i++) {
            if (strcasecmp(songList[i]->name, songName) == 0) {
                songFound = true;
                break;
            }
        }
    }

    if (songFound) {
        playingSongNumber = i;
        songList[playingSongNumber]->play(songTime);
    }
    else {
#ifdef DEBUG
        Serial.println(F("Playing the default song"));
#endif
        //play the default song from the flash memory
        ClockSongRtttl::playDefault();
    }

    playingSongNumber = -1;
}

boolean ClockMusic::stop()
{
    //if the music is being played now
    if (playingSongNumber < 0) {
        return false;
    }

    songList[playingSongNumber]->stop();

    return true;
}