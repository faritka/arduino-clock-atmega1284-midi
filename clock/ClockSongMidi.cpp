#include "ClockSongMidi.h"

ClockSongMidi::ClockSongMidi(const char *songName) : ClockSong(songName)
{
}

bool ClockSongMidi::play(const int songTime)
{
    //init the MIDI device 
    player = new MidiPlayer(MIDI_PIN);

    //turn on the sound amplifier
    pinMode(AMPLIFIER_PIN, OUTPUT);
    digitalWrite(AMPLIFIER_PIN, HIGH);

    //create the full path to the song
    char fileName[sizeof(SD_DIRECTORY_MIDI) + 1 + sizeof(name) + sizeof(MIDI_FILE_EXTENSION) + 1];
    strcpy(fileName, SD_DIRECTORY_MIDI);
    strcat(fileName, "/");
    strcat(fileName, name);
    strcat(fileName, MIDI_FILE_EXTENSION);

#ifdef DEBUG  
    Serial.print(F("Playing MIDI: "));
    Serial.println(fileName); 
#endif

    player->play(fileName, songTime);

    //turn off the amplifier
    digitalWrite(AMPLIFIER_PIN, LOW);

    delete player;
    player = NULL;

    return true;
}

void ClockSongMidi::stop()
{
    if (player) {
        player->stop();
    }
}

byte ClockSongMidi::readSongNames(QueueList<ClockSong*> &songList)
{
#ifdef DEBUG
    //read the songs from the SD card    
    Serial.println(F("Reading the MIDI song names from the SD card"));
#endif

    File midiDir = SD.open(SD_DIRECTORY_MIDI);
    if (!midiDir) {
#ifdef DEBUG
        Serial.print(F("Cannot open the dir: "));
        Serial.println(SD_DIRECTORY_MIDI);
#endif
        return 1;
    }

    readSongDirectory(midiDir, songList);
    midiDir.close();

    return 0;
}

void ClockSongMidi::readSongDirectory(File dir, QueueList<ClockSong*> &songList)
{
    char songName[SONG_NAME_LENGTH];

    //in case this is not the first time we opened the directory, rewind it
    dir.rewindDirectory();

    while (true) {
        //open the next file
        File entry = dir.openNextFile();

        //there are no more files in the dir
        if (!entry) {
            break;
        }

        if (!entry.isDirectory()) {
            strncpy(songName, entry.name(), SONG_NAME_LENGTH);

            //allow only MIDI songs
            char *extensionPosition = strstr(songName, MIDI_FILE_EXTENSION);
            if (extensionPosition == NULL) {
                continue;
            }

            //cut out the extension
            *extensionPosition = '\0';
#ifdef DEBUG
            Serial.print(F("MIDI song: "));
            Serial.println(songName);
#endif

            ClockSongMidi *songObject = new ClockSongMidi(songName);
            songList.push(songObject);

            //too many songs, no overflow
            if (songList.count() >= NUMBER_SONGS) {
                break;
            }
        }

        entry.close();
    }

}
