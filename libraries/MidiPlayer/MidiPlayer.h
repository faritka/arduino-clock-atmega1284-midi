/**
 * The class for Arduino to play a MIDI file from an SD card
 *
 * It requires a MIDI chip with a serial connection. 
 * VS1053 can play MIDI files, but the musical instruments are not good.
 * I recommend using Dream's SAM2195 
 * or a newer version SAM2695 http://www.dream.fr/pdf/Serie2000/SAM_Datasheets/SAM2695.pdf
 * FluxamaSynth is a shield with SAM2195 http://moderndevice.com/product/fluxamasynth-shield/
 *
 * As Arduino typically doesn't have enough memory to load and parse the whole MIDI file
 * into RAM, this library reads from an SD card and plays each MIDI event sequentially.
 * It can play MIDI files with multiple tracks.
 *
 * @author     Farit Nabiullin
 * @link       http://hobby.farit.ru
 */
#ifndef __MIDIPLAYER_H_
#define __MIDIPLAYER_H_

#include <Arduino.h>
#include <SendOnlySoftwareSerial.h>
#include <SD.h>
#include <QueueList.h>

#define DEBUG

//the maximum number of MIDI tracks in the file
#define MAXIMUM_NUM_TRACKS 32

class MidiPlayer 
{
  public:
    /**
     * Constructor
     *
     * @param const int transmitPin The processor pin to which FluxamaSynth is connected
     */
    MidiPlayer(byte transmitPin);

    /**
     * Plays the MIDI file
     *
     * @params char *filename The filename on SD
     * @params unsigned long maxTime Stop playing after this time in seconds
     */
    boolean play(char *filename, unsigned long maxTime);

    /**
     * Stops playing
     */
    inline void stop() 
    {
        switchOn = false;
    }

  private:  
    /**
     * Midi file header structure
     */
    typedef struct {
        unsigned long headerLength;
        unsigned int formatType;
        unsigned int numTracks;
        int timeDivision;
        unsigned long smpteTiming; 
    } MidiHeader;

    /**
     * Midi track header structure
     */
     typedef struct {
        //the pointer to the current position in the file
        unsigned long trackPointer;
        //the length of the track
        unsigned long trackLength;
        //if the track has been played completely
        boolean trackFinished;

        //when the event starts
        unsigned long eventTime;
        //the last command for running status events
        unsigned char eventLastCommand;
        //the event type
        unsigned int eventType;
        //the event data
        union {
            unsigned char data[3];
            unsigned long tempo;
        } eventData;
    } MidiTrack;

    /**
     * The MIDI file
     */
    File file;

    /**
     * The microprocessor pin, which acts as the transmit-only serial port to the FluxamaSynth
     */
    SendOnlySoftwareSerial port;

    /**
     * The MIDI file header
     */
    MidiHeader header;

    /**
     * When it is On, music can play
     */
    boolean switchOn;

    /**
     * The current MIDI track number
     */
    int currentTrackNumber;

    /**
     * The array of tracks
     */
    QueueList<MidiTrack> tracks; 

    /**
     * The tempo
     */
    unsigned long tempo;

    /**
     * The number of finished tracks
     */
    unsigned int numFinishedTracks;

    /**
     * Creates the header ID
     */
    inline long makeHeaderId(int c1, int c2, int c3, int c4)
    {
        return (c1 | (c2 << 8) | (c3 << 16) | c4 << 24);
    } 

    /**
     * Reades the header ID
     */
    long readHeaderId();

    /**
     * Reads a byte from the file
     */
    int readByte();
    /**
     * Reads a byte from the file for the track
     */
    int readByte(MidiTrack &track);

    /**
     * Read a byte from the file without advancing to the next one.
     */
    int peekByte();

    /**
     * Reads a variable-length number from the file for the track
     */
    long readVar(MidiTrack &track);

    /**
     * Reads a fixed-size big-endian number
     */
    int readInt(int numBytes);

    /**
     * Skips the number of bytes
     */
    void skipBytes(const int bytes);

    /**
     * Skips the number of bytes for a track
     */
    void skipBytes(const int bytes, MidiTrack &track);

    /**
     * Parses the MIDI header
     */
    boolean parseMidiHeader();    

    /**
     * Parses the MIDI track header
     */
    boolean parseTrackHeader(const int trackNumber);

    /**
     * Parses the next event from the track
     *
     * @param MidiTrack track The track object
     * @param unsigned long currentMillis The current time in milliseconds
     */
    boolean parseNextEvent(MidiTrack &track, unsigned long currentMillis);   

    /**
     * Processes the current event from the track
     *
     * @param MidiTrack track The track object
     * @param unsigned long currentMillis The current time in milliseconds
     */
    boolean processEvent(MidiTrack &track, unsigned long currentMillis);

};

// __MIDIPLAYER_H_
#endif
