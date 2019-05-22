/* Learn how to use a potentiometer to fade an LED with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/arduino-fade-led
   Dev: Michalis Vasilakis / Date: 25/10/2014                                                   */
   

/*

  WiFi UDP device responding to buttons on a web page 
  sent from a web page to node.js client to node.js server
  and finally to here

  created April 1, 2019
  by Henok Guluma

  Change log:
  14 April 2019 - ms -  better comments and cleaned up code
                      Temperature sensor configured properly

*/

#include <WiFiNINA.h>
#include <WiFiUdp.h>

//int sensePin = A0;  //This is the Arduino Pin that will read the sensor output
int sensorInput;    //The variable we will use to store the sensor input

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int myPort = 5000;      // local port to listen on
unsigned int remoteServerPort = 7000;  // remote port to talk to
//IPAddress remoteServerAddress(192, 168, 1, 27);
IPAddress remoteServerAddress(10, 225, 161, 128);

char packetBuffer[255]; //buffer to hold incoming packet

WiFiUDP Udp;

// Sensors
const int BUTTON_PIN1 = 13;
const int BUTTON_PIN2 = 14;

// Actuators
const int NUMBER_OF_ACTUATORS = 2;
int actuators[NUMBER_OF_ACTUATORS] = {
  3, // pin numbers
  4
};

String LED_ON_MESSAGE = "ledON";
String LED_OFF_MESSAGE = "ledOFF";

// remember the button state so we only send
// when the state changes
boolean buttonState1;
boolean lastbuttonState1=LOW;
boolean buttonState2;
boolean lastbuttonState2=LOW;

int lastvalueState1;
int lastvalueState2;

int value1; //save analog value
int value2;
void setup() {

  // Set all actuator pins as outputs
  for (int i = 0; i < NUMBER_OF_ACTUATORS; i++ ) {
    pinMode(actuators[i], OUTPUT);
  }
 // pinMode(sensePin, INPUT);
  pinMode(BUTTON_PIN1, INPUT);
  pinMode(BUTTON_PIN2, INPUT);
  
  //Initialize serial
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  // Remove this if running without a serial monitor
 

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.print("Initializing WiFiUDP library and listening on port ");
  Serial.println(myPort);
  Udp.begin(myPort);
}

void loop() {

  value1 = analogRead(A5);    //read the analog sensor and store it
  value2 = analogRead(A6);
  buttonState1 = digitalRead(BUTTON_PIN1);
  buttonState2 = digitalRead(BUTTON_PIN2);
 
  value1 = map(value1, 0, 1023, 0, 11); //Map value 0-1023 to 0-255 (PWM)
  if (value1<1)
    value1=0;
  else if (value1 < 2)
    value1 = 1;
  else if (value1 < 3)
    value1 = 2;
  else if (value1 < 4)
    value1 = 3;
  else if (value1 < 5)
    value1 = 4;
  else if (value1 < 6)
    value1 = 5;
    else if (value1 < 7)
    value1 = 6;
  else if (value1 < 8)
    value1 = 7;
  else if (value1 < 9)
    value1 = 8;
  else if (value1 < 10)
    value1 = 9;
  else if (value1 < 11)
    value1 = 10;

   value2 = map(value2, 0, 1023, 0, 6); //Map value 0-1023 to 0-255 (PWM)
   if (value2<1)
    value2=0;
   else if (value2 < 2)
    value2 = 1;
   else if (value2 < 3)
    value2 = 2;
   else if (value2 < 4)
    value2 = 3;
   else if (value2 < 5)
    value2 = 4;
   else if (value2 < 6)
    value2 = 5;
  
  
  // It's polite to listen first
  // Did the server send us anything?
  int packetSize = Udp.parsePacket(); 
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.print(packetSize);
    Serial.print(" from address ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);

    // Activate the actuators as requested
    digitalWrite( 
      actuators[(int)packetBuffer[0]],  // first byte is actuator number
      (int)packetBuffer[1]);            // second byte is value
  }
  
  if ((value1 != lastvalueState1) || (value2 != lastvalueState2) || (buttonState1 != lastbuttonState1) || (buttonState2 != lastbuttonState2) ) {
    Serial.println("Pot rotation changed; sending new state");
    Serial.print(value1);Serial.print(value2); Serial.print(buttonState1); Serial.print(buttonState2);
    Udp.beginPacket(remoteServerAddress, remoteServerPort);
    Udp.write(value1);
    Udp.write(value2);
    Udp.write(buttonState1);
    Udp.write(buttonState2);


    
    Udp.endPacket();
    

    lastvalueState1 = value1;
    lastvalueState2 = value2;
    lastbuttonState1 = buttonState1;
    lastbuttonState2 = buttonState2;
    
  }
 
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("My IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
