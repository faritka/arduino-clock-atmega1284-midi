# Arduino Clock Atmega1284p

## Documentation is on the project's webpage: http://hobby.farit.ru

![The front view of the Arduino clock](http://hobby.farit.ru/wp-content/uploads/2015/06/clock_arduino_front.jpg)

## The Arduino Clock:

* uses the C++ object-oriented code written for the Arduino framework
* is powered by the 8-bit Atmel's microcontroller ATmega1284P http://www.atmel.com/devices/ATMEGA1284P.aspx
* is installed on the PCB from the Wise Clock 4 http://timewitharduino.blogspot.com/search/label/Wise%20Clock%204
* displays information on the Sure Electronic's 32X16 RG Dual Color LED Dot Matrix Unit Board http://store.sure-electronics.com/led/de-dp14116 (display colors: red, green, orange)</li>
* adjusts time from the Internet using the NTP protocol via the Wi-Fi RN-XV WiFly Module http://www.microchip.com/wwwproducts/Devices.aspx?product=RN171XV
* shows the weather forecast from the Internet
* has alarms that can be configured to activate every day or once on a specific date or tomorrow or on different week days
* plays music in the RTTTL format that was popular in mobile phones
* plays music in the MIDI format via the Fluxamasynth Shield http://moderndevice.com/product/fluxamasynth-shield/ that uses the Atmel's and Dream's ATSAM2195 MIDI chip
* changes the brightness of the display depending on the light level in a room detected by the light-dependent resistor
