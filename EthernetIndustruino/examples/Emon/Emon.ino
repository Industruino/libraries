//This sketch will read analog input A0 and A11 and post its value to EmonCMS.org every 5 seconds. To use this sketch, first make a free user account at emoncms.org.

char foo; //without a simple variable declaration, use of #ifdef at the top of your code raises an error!

typedef uint8_t SOCKET;
#include <SPI.h>
#include <EthernetIndustruino.h>
#define W5500_ETHERNET_SHIELD
// Include Emon Library


//network configuration, WIRED or WIFI

  //MAC address of your Ethernet module
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  
  // fill in an available IP address on your network here,
  IPAddress ip(192, 168, 1, 130);  
 
EthernetClient client; //Ethernet client mode
// The analog sensor pins to be read
const int lightSensorPin = A0;
int tempSensorPin = A11;

float tempValue = 0;
int lightValue = 0;

// Create an Emon instance
//Emoncms configurations
char server[] = "emoncms.org"; // name address for emoncms.org
//IPAddress server(213, 138, 101, 177); // numeric IP for emoncms.org (no DNS)

String apikey = "01278441cb103fca172410a5e1061a29"; //change this to your api key (can be found on the API help page of your EmonCMS.org account).
int node = 0; //if 0, not used

unsigned long lastConnectionTime = 0; // last time you connected to the server, in milliseconds
boolean lastConnected = false; // state of the connection last time through the main loop
const unsigned long postingInterval = 5000; // delay between updates, in milliseconds

void setup() {
//Setup of Ethernet module chip select lines
pinMode(10, OUTPUT);    
pinMode(6, OUTPUT);     
pinMode(4, OUTPUT);     
digitalWrite(10, LOW);
digitalWrite(6, HIGH);
digitalWrite(4, HIGH);
      
delay(5000); //wait 5 seconds to make sure Ethernet is running.
 // Open serial communications
Serial.begin(9600);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {

    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);

  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("starting..");

}



void loop() {

  // if you're not connected, and at least <postingInterval> milliseconds have
  // passed sinceyour last connection, then connect again and
  // send data:
  if ((millis() - lastConnectionTime > postingInterval)) {
   
    //read sensors
    lightValue = analogRead(lightSensorPin);
    tempValue = analogRead(tempSensorPin);

    //Print values (debug)
    Serial.println();
    Serial.print("Temp : ");
    Serial.print(tempValue);
    Serial.print(" ; Light : ");
    Serial.print(lightValue);
    //send values
    sendData();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void sendData() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    // send the HTTP GET request:
   client.print("GET /api/post?apikey=");
    client.print(apikey);
    if (node > 0) {
      client.print("&node=");
      client.print(node);
    }
    client.print("&json={temp");
    client.print(":");
    client.print(tempValue);
    client.print(",light:");
    client.print(lightValue);
    client.println("} HTTP/1.1");
    client.println("Host:emoncms.org");
    client.println("User-Agent: Arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
    Serial.println(freeRam());
    Serial.println("Disconnecting...");
    client.stop();
  }
  else {
    // if you couldn't make a connection:
    Serial.println("Connection failed");
  }
}


void printStatus() {
  #ifdef WIFI
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.print(" dBm");
  #else
    // print your local IP address:
    Serial.print("IP address: ");
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      // print the value of each byte of the IP address:
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print(".");
    }
  #endif
  Serial.println();
}

float getCelsius(int sensorValue) {
/*
created by Federico Vanzati for TinkerKit Thermistor Library
*/
  const static float ADCres = 1023.0;
  const static int Beta = 3950;	// Beta parameter
  const static float Kelvin = 273.15;	// 0°C = 273.15 K
  const static int Rb = 10000;	// 10 kOhm
  const static float Ginf = 120.6685;	// Ginf = 1/Rinf
  
  float Rthermistor = Rb * (ADCres / sensorValue - 1);
  float _temperatureC = Beta / (log( Rthermistor * Ginf )) ;
  return _temperatureC - Kelvin;
}


int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
