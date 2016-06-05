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
Indio.digitalMode(1,INPUT);       // Set CH1 as an input
Indio.digitalMode(7,OUTPUT);      // Set CH7 as an output
```
Read/write:
```
Indio.digitalRead(1);             // Read CH1
Indio.digitalWrite(7,LOW);        // Set CH7 to low (0V)
```

ANALOG INPUT

Configuration of resolution:
```
Indio.setADCResolution(16);       // Set the ADC resolution
                                  // Choices are 12bit@240SPS, 14bit@60SPS, 16bit@15SPS and 18bit@3.75SPS.
```
Configuration of input mode:
```
Indio.analogReadMode(1, V10);     // Set Analog-In CH1 to 10V mode (0-10V).
Indio.analogReadMode(1, V10_p);   // Set Analog-In CH1 to % 10V mode (0-10V -> 0-100%).
Indio.analogReadMode(1, V5);      // Set Analog-In CH1 to 5V mode (2x gain enabled on ADC).
Indio.analogReadMode(1, V5_p);    // Set Analog-In CH1 to 5V mode (0-5V -> 0-100%).
Indio.analogReadMode(1, V10_raw); // Set Analog-In CH1 to 10V mode and read raw ADC value (0-10V -> 0-4096).

Indio.analogReadMode(1, mA);      // Set Analog-In CH1 to mA mode (0-20mA).
Indio.analogReadMode(1, mA_p);    // Set Analog-In CH1 to % mA mode (4-20mA -> 0-100%)
Indio.analogReadMode(1, mA_raw);  // Set Analog-In CH1 to mA mode and read raw ADC value (0-20mA -> 0-4096).
```  
Read:
```
Indio.analogRead(1);              //Read Analog-In CH1 (output depending on selected mode as above)
```

ANALOG OUTPUT

Configuration of output mode:
```
Indio.analogWriteMode(1, V10);      // Set Analog-Out CH1 to 10V mode (0-10V).
Indio.analogWriteMode(1, V10_p);    // Set Analog-Out CH1 to % 10V mode ( 0-100% -> 0-10V).
Indio.analogWriteMode(1, V10_raw);  // Set Analog-Out CH1 to 10V mode and take raw DAC value (0-4096 -> 0-10V).

Indio.analogWriteMode(1, mA);       // Set Analog-Out CH1 to mA mode (0-20mA).
Indio.analogWriteMode(1, mA_p);     // Set Analog-Out CH1 to % mA mode (0-100% -> 4-20mA).
Indio.analogWriteMode(1, ma_raw);   // Set Analog-Out CH1 to mA mode and take raw DAC value (0-4096 -> 0-20mA).   
```
Write (examples corresponding to above configuration):
```
Indio.analogWrite(1, 2.67, true);   //Set CH1 to 2.67V ("true" will write value to EEPROM of DAC, restoring it after power cycling).
Indio.AnalogWrite(1, 33.5, true);   //Set CH1 to 33.5% (approx 3.685V)
Indio.AnalogWrite(1, 1000, true);   //Set CH1 DAC to integer value 1000 (approx 2.685V).

Indio.analogWrite(1, 10.50, false); //Set CH1 to 10.5mA ("false" will not write value to EEPROM of DAC).
Indio.AnalogWrite(1, 75, true);     //Set CH1 to 75% (approx 16mA).
Indio.AnalogWrite(1, 2048, true);   //Set CH1 DAC to integer value 2048 (approx 10.5mA).
```



