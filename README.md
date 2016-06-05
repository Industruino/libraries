# Industruino libraries
Arduino libraries to use with Industruino products
* [UC1701](#uc1701)
* [EthernetIndustruino](#ethernetindustruino)
* [Indio](#indio)

# UC1701
For using the Industruino LCD, there are 2 options: 
* the UC1701 library (available above): easy to use, relatively small (it is also used in the Industruino pre-installed demo sketches). it is largely compatible with the popular PCD8544 (Nokia screens). familiar syntax:  
```
lcd.begin();  
lcd.clear();  
lcd.setCursor(1, 1);  
lcd.print("hello Industruino!");
```
* the U8G library: a popular display library with many fonts and graphics, consuming more memory

# EthernetIndustruino
If you are using the Industruino Ethernet module, you will need this library which is based on the standard Arduino Ethernet library.

# Indio
If you are using the Industruino IND.I/O product, you will need this library to access the I/O channels. The pins on the IDC expansion connector, the backlight pin, and the membrane panel buttons pin(s) should still be accessed in the usual way, not using the Indio library; the Indio library is only for the external I/O channels available on the green screw connectors.  
DIGITAL I/O
Configuration:
```
Indio.digitalMode(1,INPUT);  // Set CH1 as an input
Indio.digitalMode(7,OUTPUT);  // Set CH7 as an output
```
Read/write:
```
Indio.digitalRead(1);      // Read CH1
Indio.digitalWrite(7,LOW); // Set CH7 to low (0V)
```
ANALOG INPUT
Configuration of resolution:
```
Indio.setADCResolution(16); // Set the ADC resolution. Choices are 12bit@240SPS, 14bit@60SPS, 16bit@15SPS and 18bit@3.75SPS.
```
Configuration of input mode:
```
Indio.analogReadMode(1, V10);     // Set Analog-In CH1 to 10V mode (0-10V).
Indio.analogReadMode(1, V10_p);   // Set Analog-In CH1 to % 10V mode (0-10V -> 0-100%).
Indio.analogReadMode(1, mA);      // Set Analog-In CH1 to mA mode (0-20mA).
Indio.analogReadMode(1, mA_p);    // Set Analog-In CH1 to % mA mode (4-20mA -> 0-100%)
Indio.analogReadMode(1, V5);      // Set Analog-In CH1 to 5V mode (2x gain enabled on ADC).
Indio.analogReadMode(1, V5_p);    // Set Analog-In CH1 to 5V mode (0-5V -> 0-100%).
Indio.analogReadMode(1, V10_raw); // Set Analog-In CH1 to 10V mode and read raw ADC value (0-10V -> 0-4096).
Indio.analogReadMode(1, mA_raw);  // Set Analog-In CH1 to mA mode and read raw ADC value (0-20mA -> 0-4096).
```  
ANALOG OUTPUT


