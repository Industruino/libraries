# Industruino libraries
Arduino libraries to use with Industruino products
* [UC1701](#uc1701)
* [EthernetIndustruino](#ethernetindustruino)
* [Indio](#indio)
  * [digital I/O](#digital-io) 
  * [analog input](#analog-input)
  * [analog output](#analog-output)
  * [interrupts](#interrupts)
  * [calibration](#calibration)

# UC1701
The Industruino LCD is connected over SPI to the pins D19,20,21,22 (and the backlight to D13 on 32u4 boards and D26 on 1286 boards). We suggest you use either of these 2 libraries:
* our customised UC1701 library (available in this repository): easy to use, relatively small (it is also used in the Industruino pre-installed demo sketches). it is largely compatible with the popular PCD8544 (Nokia screens). familiar syntax:  
```
lcd.begin();  
lcd.clear();  
lcd.setCursor(1, 1);  
lcd.print("hello Industruino!");
```
 
* the [U8G](https://github.com/olikraus/u8glib) library: a popular display library with many fonts and graphics, consuming more memory. you can use this constructor:
```
U8GLIB_MINI12864 u8g(21, 20, 19, 22);	// SPI Com: SCK = 21, MOSI = 20, CS = 19, A0 = 22
```

# EthernetIndustruino
If you are using the Industruino Ethernet module, you will need this library which is based on the standard Arduino Ethernet library. The Ethernet module is connected over SPI, so we also need the SPI library.
```
#include <SPI.h>
#include <EthernetIndustruino.h>
```
The Ethernet module also includes FRAM; see the example in the library on how to use this.

# Indio
If you are using the Industruino IND.I/O product, you will need this library to access the I/O channels. The pins on the IDC expansion connector, the backlight pin, and the membrane panel buttons pin(s) should still be accessed in the usual way, not using the Indio library; the Indio library is only for the external I/O channels available on the green screw connectors.

The Indio board uses an I2C expander for the I/O channels so we also need the Wire library.
```
#include <Indio.h>
#include <Wire.h>
```

### DIGITAL IO 

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

### ANALOG INPUT

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

### ANALOG OUTPUT

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

### INTERRUPTS

For using interrupts on the digital channels, the method depends on whether you're using topboard 32u4 or 1286. 

#### INTERRUPTS on the 32u4 topboard

The interrupt pin of the expander on the 12/24V digital side is connected to pin D8 (PCINT4) of the 32u4 topboard. This pin will trigger when a change on any of the 8 input or output channels occurs. If more than 1 channel needs to be detected by the interrupt, a small comparison routine can be run inside the interrupt service routine, which compares the status of the channels pre-inerrupt to the current status.

This code example (for 32u4 topboard) shows a counter on the LCD for each rising edge on CH1 (without debounce).
```
#include <Indio.h>
#include <Wire.h>

#include <UC1701.h>
static UC1701 lcd;

volatile int risingEdge = 1;
volatile int counter = 0;

void setup() {

  Serial.begin(9600);
  lcd.begin();
  
  Indio.digitalWrite(1, LOW);  // Clear CH1 to LOW
  Indio.digitalMode(1, INPUT); // Set CH1 as an input

  // Enable Pin Change Interrupt
  PCMSK0 = (1 << PCINT4);
  PCICR = (1 << PCIE0);

  // Global Interrupt Enable
  sei();
}

ISR (PCINT0_vect)
{
  if (risingEdge == 1) {
    risingEdge = 0;
    Serial.println("trigger");
    counter++;
  }

  else {
    risingEdge = 1;
  }
}

void loop() {
  lcd.setCursor(1,3);
  lcd.print(counter);
  delay(100);
}
```

#### INTERRUPTS on the 1286 topboard

The interrupt pin of the expander on the 12/24V digital side is connected to the INT7 pin of the 1286 topboard. This pin will trigger when a change on any of the 8 input or output channels occurs, and we can specify `CHANGE`, `RISING`, `FALLING`, `LOW` (note this pin is inverted: a change from LOW to HIGH on the digital channel triggers `FALLING`). If more than 1 channel needs to be detected by the interrupt, a small comparison routine can be run inside the interrupt service routine, which compares the status of the channels pre-inerrupt to the current status.

This code example (for 1286 topboard) shows a counter on the LCD for each rising edge on CH1 (without debounce).
```
#include <Indio.h>
#include <Wire.h>

#include <UC1701.h>
static UC1701 lcd;

volatile int counter = 0;

void setup() {

  Serial.begin(9600);
  lcd.begin();

  Indio.digitalWrite(1, LOW);  // Clear CH1 to LOW
  Indio.digitalMode(1, INPUT); // Set CH1 as an input

  attachInterrupt(7, count, CHANGE);       // INT7 attached to the interrupt of the expander
                                           // this is not D7
}

void count() {
  Serial.println("trigger");
  counter++;
}

void loop() {
  lcd.setCursor(1, 3);
  lcd.print(counter);
  delay(100);
}
```

The 1286 topboard also allows us to attach an interrupt to the membrane panel buttons: its button inputs are connected to pin change interrupts PCINT 4, 5, and 6 for buttons Down, Enter, and Up respectively.

Below demo sketch will show "waiting" on the LCD; when you press the "Enter" button an interrupt will be triggered and "Enter pressed" will show on the LCD for one second. To attach the interrupt to the "Up" or "Down" button change "PCINT5" in line `PCMSK0 = (1 << PCINT6);` to PCINT4 or PCINT6.

```
#include <UC1701.h>
static UC1701 lcd;

volatile int modeFlag = 0;

void setup() {
  
    lcd.begin(); //enable LCD
    // Enable Pin Change Interrupt 5 = Enter button
    PCMSK0 = (1 << PCINT5);
    PCICR = (1 << PCIE0);
 
    // Global Interrupt Enable
   sei();    
}

ISR (PCINT0_vect)
{    
    modeFlag = 1;    
}

void loop() { 
  
  lcd.setCursor(0, 0);
  lcd.print("waiting       ");
  
  if (modeFlag == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Enter pressed");
    delay(1000); 
    modeFlag = 0;
  }
}
```

### CALIBRATION

Please find the calibration data array inside the Indio.cpp library file, together with an explanation on how to perform the calibration. The library is preloaded with calibration data but characteristics are board specific thus reading with standard cal. data might be off.



