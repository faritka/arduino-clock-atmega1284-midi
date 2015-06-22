#include "ClockSongRtttl.h"

ClockSongRtttl::ClockSongRtttl(const char *songName) : ClockSong(songName), switchOn(false)
{
    pinMode(SPEAKER_1, OUTPUT);
    pinMode(SPEAKER_2, OUTPUT);
    digitalWrite(SPEAKER_1, LOW);
}

const char PROGMEM ClockSongRtttl::defaultSong[] 
    = {"Korobeyniki:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a"};

void ClockSongRtttl::findSongName(String &song, char *songName)
{
    //the song name is divided by ':'
    String songNameString = song.substring(0, song.indexOf(':'));

    //the maximum number of saved characters in the config
    if (songNameString.length() > SONG_NAME_LENGTH) {
        songNameString = songNameString.substring(0, SONG_NAME_LENGTH);
    }

    songNameString.toCharArray(songName, SONG_NAME_LENGTH);

#ifdef DEBUG
    Serial.print(F("Found RTTTL song name: "));
    Serial.println(songName);
#endif
}


bool ClockSongRtttl::play(const int songTime)
{
    bool songFound = false;
    String song;
    char songName[SONG_NAME_LENGTH];

    switchOn = false;

#ifdef DEBUG
    //read the songs from the SD card    
    Serial.println(F("Reading the music for alarms from the sd"));
    Serial.print(F("Config song name: "));
    Serial.println(name);
#endif

    ClockFile file;
    if (!file.open(SD_FILE_MUSIC)) {    
        while (file.readLine(song)) {
#ifdef DEBUG
            Serial.print(F("Song line: "));
            Serial.println(song);
#endif
            findSongName(song, songName);

            if (strcasecmp(songName, name) == 0) {
#ifdef DEBUG
                Serial.print(F("Playing the song: "));
                Serial.println(songName);
#endif
                songFound = true;
                break;
            }
        }
    }

    if (!songFound) {
        return false;
    }

    //the song plays
    switchOn = true;

    playRtttl(song);

    //the song doesn't play
    switchOn = false;

    return true;
}


void ClockSongRtttl::playRtttl(String song)
{
    int notes[] = { 0,
              NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
              NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
              NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
              NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
    };

    unsigned int i = 0;

    byte default_dur = 4;
    byte default_oct = 6;
    int bpm = 63;
    int num;
    long wholenote;
    long duration;
    byte note;
    byte scale;

    // format: d=N,o=N,b=NNN:
    // find the start (skip name, etc)
    i = song.indexOf(':');
    if (i == -1) {
        return;
    }

    //skip ':'
    i++;

    // get default duration
    if(song[i] == 'd') {
        i++; 
        i++;              // skip "d="
        num = 0;
        while(isDigit(song[i])) {
              num = (num * 10) + (song[i] - '0');
              i++;
        }
        if (num > 0) {
            default_dur = num;
        }
        i++;                   // skip comma
    }

#ifdef DEBUG
    Serial.print(F("ddur: ")); 
    Serial.println(default_dur, 10);
#endif

    // get default octave
    if (song[i] == 'o') {
        i++; 
        i++;              // skip "o="
        num = song[i] - '0';
        i++;
        if (num >= 3 && num <=7) {
            default_oct = num;
        }
        i++;                   // skip comma
    }

#ifdef DEBUG
    Serial.print(F("doct: ")); 
    Serial.println(default_oct, 10);
#endif

    // get BPM
    if (song[i] == 'b') {
        i++; 
        i++;              // skip "b="
        num = 0;
        while(isDigit(song[i])) {
            num = (num * 10) + (song[i] - '0');
            i++;
        }
        bpm = num;
        i++;                   // skip colon
    }

#ifdef DEBUG
    Serial.print("bpm: "); 
    Serial.println(bpm, 10);
#endif

    // BPM usually expresses the number of quarter notes per minute
    wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

#ifdef DEBUG
    Serial.print(F("wn: ")); 
    Serial.println(wholenote, 10);
#endif

    // now begin note loop
    while (song[i]) {
        //the song was stopped
        if (!switchOn) {
            break;
        }

        // first, get note duration, if available
        num = 0;
        while(isDigit(song[i])) {
            num = (num * 10) + (song[i] - '0');
            i++;
        }

        if (num) {
            duration = wholenote / num;
        }
        else {
            duration = wholenote / default_dur;  // we will need to check if we are a dotted note after
        }
        // now get the note
        note = 0;

        switch(song[i]) {
            case 'c':
                note = 1;
                break;
            case 'd':
                note = 3;
                break;
            case 'e':
                note = 5;
                break;
            case 'f':
                note = 6;
                break;
            case 'g':
                note = 8;
                break;
            case 'a':
                note = 10;
                break;
            case 'b':
                note = 12;
                break;
            case 'p':
            default:
                note = 0;
        }
        i++;

        // now, get optional '#' sharp
        if (song[i] == '#') {
            note++;
            i++;
        }

        // now, get optional '.' dotted note
        if (song[i] == '.') {
            duration += duration/2;
            i++;
        }

        // now, get scale
        if (isDigit(song[i])) {
            scale = song[i] - '0';
            i++;
        }
        else {
            scale = default_oct;
        }
        scale += OCTAVE_OFFSET;
        if (song[i] == ',') {
            i++;       // skip comma for next note (or we may be at the end)
        }

        // now play the note
        if (note) {
#ifdef DEBUG
            Serial.print(F("Playing: "));
            Serial.print(scale, 10); 
            Serial.print(F(" "));
            Serial.print(note, 10); 
            Serial.print(F(" ("));
            Serial.print(notes[(scale - 4) * 12 + note], 10);
            Serial.print(F(") "));
            Serial.println(duration, 10);
#endif
            toneAC2(SPEAKER_1, SPEAKER_2, notes[(scale - 4) * 12 + note]);
            delay(duration); 
            noToneAC2();
        }
        else {
#ifdef DEBUG
            Serial.print(F("Pausing: "));
            Serial.println(duration, 10);
#endif
            delay(duration);
        }
    }
}

byte ClockSongRtttl::readSongNames(QueueList<ClockSong*> &songList)
{
    String song;
    char songName[SONG_NAME_LENGTH];

#ifdef DEBUG
    //read the songs from the SD card
    Serial.println(F("Reading the song names from the SD card"));
#endif

    ClockFile file;
    if (!file.open(SD_FILE_MUSIC)) {
        while (file.readLine(song)) {
#ifdef DEBUG
            Serial.print(F("Song line: "));
            Serial.println(song);
#endif
            findSongName(song, songName);

            ClockSongRtttl *songObject = new ClockSongRtttl(songName);
            songList.push(songObject);

            //too many songs, no overflow
            if (songList.count() >= NUMBER_SONGS) {
                break;
            }
        }
    }
    else {
        return 1;
    }

    return 0;
}

bool ClockSongRtttl::playDefault()
{
    ClockSongRtttl song("dS");
    String songContent;
    char c;
    unsigned int i = 0;
    while ((c = pgm_read_byte_near(defaultSong + i++))) {
        songContent += c;
    }

    song.playRtttl(songContent);

    return true;
}

