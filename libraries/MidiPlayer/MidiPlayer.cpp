#include "MidiPlayer.h"

MidiPlayer::MidiPlayer(byte transmitPin) : port(transmitPin) 
{
    //the standard MIDI speed
    port.begin(31250);

}

boolean MidiPlayer::play(char *filename, unsigned long maxTime)
{
    //turn on the music
    switchOn = true;

    //the current time
    unsigned long currentTime = millis();
    //the song end time in milliseconds if it's too long
    unsigned long endTime = currentTime + maxTime * 1000;

    int i = 0;

    //set the default values
    currentTrackNumber = -1;
    //120 bpm
    tempo = 500000;
    numFinishedTracks = 0;

    if (!SD.exists(filename)) {
#ifdef DEBUG
        Serial.println(F("The file doesn't exist"));
#endif
        return false;
    }

    file = SD.open(filename, FILE_READ);
    if (!file) {
#ifdef DEBUG
        Serial.println(F("Can't open the file"));
#endif
        return false;
    }

    if (!parseMidiHeader()) {
        file.close();
        return false;
    } 

    for (i = 0; i < header.numTracks; i++) {
        if (!parseTrackHeader(i)) {
            file.close();
            return false;
        }
    }

    //parse and play all events
    file.seek(0);

    while (switchOn && (numFinishedTracks < header.numTracks) && (currentTime <= endTime)) {
        for (i = 0; i < header.numTracks; i++) {
            //the track has finished playing
            if (tracks[i].trackFinished) {
                continue;
            }

            currentTime = millis();
            //process the next event if the time has come for it
            processEvent(tracks[i], currentTime);
        }
    }

    file.close();

    //clean the list of the tracks
    while (!tracks.isEmpty()) {
        tracks.pop();
    }

    //reset
    port.write(0xff);
    delay(100);

    //Serial.println("Finished playing");

    return true;
}

long MidiPlayer::readHeaderId()
{
    long value;

    //not enough bytes
    if (file.available() < 4) {
        return -1;
    }

    value = readByte();
    value |= readByte() << 8;
    value |= readByte() << 16;
    value |= readByte() << 24; 

    return value;
} 


int MidiPlayer::readByte()
{
    return (file.read());    
}

int MidiPlayer::readByte(MidiTrack &track)
{
    int value = readByte();
    
    track.trackPointer++;
    track.trackLength--;
    
    return value;    
}

int MidiPlayer::peekByte()
{
    return (file.peek());
}


long MidiPlayer::readVar(MidiTrack &track)
{
    long value;
    int c;
    
    c = readByte(track);
    value = c & 0x7f;
    if (c & 0x80) {
        c = readByte(track);
        value = (value << 7) | (c & 0x7f);
        if (c & 0x80) {
            c = readByte(track);
            value = (value << 7) | ( c & 0x7f);
            if (c & 0x80) {
                c = readByte(track);
                value = (value << 7) | c;
                if (c & 0x80) {
                    return -1;
                }
            }
        }        
    }
    
    return value;
}

int MidiPlayer::readInt(int numBytes)
{
    int c, value = 0;
        
    if (file.available() < numBytes) {
        return -1;
    } 
    
    do {
        c = readByte();
        value = (value << 8) | c;
    } while (--numBytes);
        
    return value;
} 

void MidiPlayer::skipBytes(const int numBytes)
{
    for (int i = 0; i < numBytes; i++) {
        readByte();
    }
}

void MidiPlayer::skipBytes(const int numBytes, MidiTrack &track)
{
    skipBytes(numBytes);

    track.trackPointer += numBytes;
    track.trackLength -= numBytes;
}

boolean MidiPlayer::parseMidiHeader()
{
    //go to the beginning of the file
    file.seek(0);

    //a MIDI file starts from MThd
    if (readHeaderId() != makeHeaderId('M', 'T', 'h', 'd')) {
#ifdef DEBUG    
        Serial.println(F("Not a MIDI file"));
#endif        
        return false;    
    }
        
    header.headerLength = readInt(4);
    if (header.headerLength < 6) {
        return false;
    }
        
    header.formatType = readInt(2);
    if ((header.formatType != 0) && (header.formatType != 1)) {
#ifdef DEBUG    
        Serial.print(F("This MIDI format is not supported"));
#endif        
        return false;
    }
        
    header.numTracks = readInt(2);
    if ((header.numTracks < 1) || (header.numTracks > MAXIMUM_NUM_TRACKS)) {
#ifdef DEBUG    
        Serial.print(F("Invalid number of tracks"));
#endif        
        return false;
    }
        
    header.timeDivision = readInt(2);
    if (header.timeDivision < 0) {
        return false;
    }
    
    header.smpteTiming = !!(header.timeDivision & 0x8000);
    
    if (header.smpteTiming) {
#ifdef DEBUG
        Serial.println(F("No SMPTE support"));
#endif        
        return false;    
    }
    
    return true;
}

boolean MidiPlayer::parseTrackHeader(const int trackNumber)
{
    long trackLength = 0;
    
    //search for the MTrk chunk
    while (1) {
        long id = readHeaderId();
        trackLength = readInt(4);
        
        if ((id < 0) || (trackLength < 0) || (trackLength >= 0x10000000)) {
            return false;
        }
        
        //check if it's a MIDI track
        if (id == makeHeaderId('M', 'T', 'r', 'k')) {
            break;
        }
        
        skipBytes(trackLength);          
    }        
    
    //create a new track object and add it to the list of tracks
    MidiTrack track;
    track.trackPointer = file.position();
    track.trackLength = trackLength;
    track.trackFinished = false;
    track.eventTime = 0;
    track.eventType = 0;
    
    tracks.push(track);    
    
    //move the file pointer to the end of the track
    file.seek(track.trackPointer + track.trackLength);          

    return true;
}

boolean MidiPlayer::parseNextEvent(MidiTrack &track, unsigned long currentMillis)
{   
    unsigned char command;
    long deltaTicks, deltaMs;
    int c;
    int len;
     
    if (track.trackLength <= 0) {
        track.trackFinished = true;
        return true;
    } 
     
    file.seek(track.trackPointer);

    deltaTicks = readVar(track);
    if (deltaTicks < 0) {
        return true;
    }
    
    //convert ticks into milliseconds
    deltaMs = ((tempo / 1000) * deltaTicks)  / header.timeDivision;            
    
    //set the time when the event must be run
    track.eventTime = currentMillis + deltaMs;

    c = peekByte();    
    if (c < 0) {
        track.trackFinished = true;
        return true;    
    }

    //has a command
    if (c & 0x80) {
        command = c;    
        if (command < 0xf0) {
            track.eventLastCommand = command;
        }
        
        //advance one byte
        readByte(track);
    }
    //running status
    else {
        command = track.eventLastCommand;                
        if (!command) {
            return false;
        }    
    }
        
    switch (command >> 4) {
        //a channel message with 2 parameter bytes
        //note off
        case 0x8:
        //note on
        case 0x9:
        //note aftertouch
        case 0xa:
        //controller
        case 0xb:
        //pitch bend
        case 0xe:
            track.eventType = command >> 4;
            
            //MIDI channel
            track.eventData.data[0] = command & 0x0f;
            track.eventData.data[1] = readByte(track) & 0x7f;
            track.eventData.data[2] = readByte(track) & 0x7f;            
            break;
        
        //a channel message with 1 parameter byte
        //program change
        case 0xc:
        //channel aftertouch
        case 0xd:
            track.eventType = command >> 4;
            
            track.eventData.data[0] = command & 0x0f;
            track.eventData.data[1] = readByte(track) & 0x7f;
            break;    
        
        //meta
        case 0xf:
            switch (command) {
                //a meta event
                case 0xff:
                    c = readByte(track);                
                    len = readVar(track);
                    if (len < 0) {
                        return false;
                    }
            
                    switch (c) {
                        //end of track
                        case 0x2f:
                            track.eventType = c;
                            break;
                        //tempo    
                        case 0x51:
                            if (len < 3) {
                                return false;
                            }
                            track.eventType = c;

                            unsigned long b1, b2, b3;
                            b1 = readByte(track);
                            b2 = readByte(track);
                            b3 = readByte(track);
                            track.eventData.tempo = (b1 << 16) | (b2 << 8) | b3;
                            skipBytes(len - 3, track);
                            break;                
                        default:
                            //ignore all other meta events
                            skipBytes(len, track);
                            break;                        
                    }
                    break;
                default:
                    break;                                        
            }
        default:
            break;                    
    }
    
/*    
    Serial.print("trackPointer: ");
    Serial.print(track.trackPointer);
    Serial.print(", trackLength: ");
    Serial.print(track.trackLength);
    Serial.print(", tempo: ");
    Serial.print(tempo);
    Serial.print(", Event. DeltaTicks: ");
    Serial.print(deltaTicks);
    Serial.print(", deltaMs: ");
    Serial.print(deltaMs);
    Serial.print(", eventTime: ");
    Serial.print(track.eventTime);
    Serial.print(", Type: ");
    Serial.print(track.eventType, HEX);
    Serial.print(", Channel: ");
    Serial.println(track.eventData.data[0]);
*/  
    
    return true;
}


boolean MidiPlayer::processEvent(MidiTrack &track, unsigned long currentMillis)
{
    //if there are no events, try to find the next one    
    if (track.eventType == 0) {
        parseNextEvent(track, currentMillis);    
    }
    
    if (track.trackLength <= 0) {
        track.trackFinished = true;
        //the counter of the finished tracks
        numFinishedTracks++;
        
        return true;                    
    }
    
    //the time to process the current event has not come yet
    if (currentMillis < track.eventTime) {
        return true;
    }    
    
    //now send the event
    switch (track.eventType) {
        //note off
        case 0x8:
            //MIDI channel
            port.write(0x80 | track.eventData.data[0]);
            //note numer
            port.write(track.eventData.data[1]);
            //velocity
            port.write(track.eventData.data[2]);
            break;
        //note on
        case 0x9:
            //channel
            port.write(0x90 | track.eventData.data[0]);
            //note number
            port.write(track.eventData.data[1]);
            //note velocity
            port.write(track.eventData.data[2]);
            break;
        //note aftertouch
        case 0xa:
            //channel
            port.write(0xa0 | track.eventData.data[0]);
            //note number
            port.write(track.eventData.data[1]);
            //pressure amount
            port.write(track.eventData.data[2]);
            break;
        //controller
        case 0xb:
            //channel
            port.write(0xb0 | track.eventData.data[0]);
            //controller type
            port.write(track.eventData.data[1]);
            //value
            port.write(track.eventData.data[2]);
            break;
        //program change
        case 0xc:
            //channel
            port.write(0xc0 | track.eventData.data[0]);
            //instrument number: guitar, piano
            port.write(track.eventData.data[1]);
            break;                                                
        //channel after touch
        case 0xd:
            //channel
            port.write(0xd0 | track.eventData.data[0]);
            //pressure amount
            port.write(track.eventData.data[1]);
            break;                                                
        //pitch bend
        case 0xe:
            //channel
            port.write(0xe0 | track.eventData.data[0]);
            //the pitch value
            unsigned int pitchValue;
            pitchValue = track.eventData.data[1] | (track.eventData.data[2] << 7);            
            port.write(pitchValue);
            break;                                                
        //track end
        case 0x2f:
            track.trackFinished = true;
            //the counter of the finished tracks
            numFinishedTracks++;
            break;    
        //tempo
        case 0x51:
            tempo = track.eventData.tempo;    
            break;                        
        default:
            break;        
    }    
    
    //parse the next event
    if (!track.trackFinished) {
        parseNextEvent(track, currentMillis);
    }
    
    return true;
}

