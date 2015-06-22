# A library for Arduino for Playing MIDI files

The library sends a MIDI file from an SD card to a MIDI device that can play it.

Unlike other simpler libraries, it can play multi-voice multi-track MIDI files.

It requires a MIDI chip with a serial connection. 
VS1053 can play MIDI files, but the musical instruments are not good.
I recommend using Dream's SAM2195 or a newer version SAM2695 http://www.dream.fr/pdf/Serie2000/SAM_Datasheets/SAM2695.pdf
FluxamaSynth is a shield with SAM2195 http://moderndevice.com/product/fluxamasynth-shield/


## More documentation on the projects webpage: http://hobby.farit.ru

## Examples

An example code to setup and play a file:

//inits the MIDI device 
//midiPin is the digital pin number on Arduino
player = new MidiPlayer(midiPin);

//plays the song
//fileName is the full path to the file on the SD card
//songTime is the maximum duration of the song
player->play(fileName, songTime);

delete player;




