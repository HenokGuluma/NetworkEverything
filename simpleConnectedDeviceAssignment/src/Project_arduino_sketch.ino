/*

  WiFi UDP Send Byte on Button Press
  Based on UDP Send and Receive String

  created 3 February 2019
  by Michael Shiloh
  
  Modified by:
  Henok Guluma
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 5000;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet

WiFiUDP Udp;

const int DOORBELL_PIN = 3; // transmitting
const int ThermistorPin = 0;

const int blue = 6; // receiving
const int yellow = 7;
const int green= 8;


int Vo;
int Tmax = 40;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


// remember the button state so we only send
// when the state changes
boolean buttonStatebell;
boolean buttonStatetemp
boolean lastButtonStatebell;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

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
  Serial.println(localPort);
  Udp.begin(localPort);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {

  // IP address of the receiving device
  IPAddress receivingDeviceAddress(192, 168, 1, 23);
  unsigned int receivingDevicePort = 2390;

  receive(); //checks if there is an incoming signal from transmitter
  
//  localPort = 5000;
  buttonStatebell = digitalRead(DOORBELL_PIN);
  lastButtonStatebell = LOW;
 
  if (buttonStatebell != lastButtonStatebell) {

    Serial.println("button state changed; sending new state");
    Udp.beginPacket(receivingDeviceAddress, receivingDevicePort);
    Udp.write(buttonStatebell);
    
    lastButtonStatebell = buttonStatebell;
    
  }
  
  
   Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  if(T > Tmax)
  {
    buttonStatetemp = 1;
    Udp.write(buttonStatetemp);
  }
  
  //Serial.print("Temperature: "); 
  //Serial.print(T);
 

  
  // IN THIS BLOCK, IF THE TEMPERATURE SENSOR DETECTS A TEMPERATURE ABOVE A CERTAIN VALUE, IT WILL CHANGE THE STATE OF A VARIABLE. AFTER
  // THAT IT WILL BE WRITTEN IN THE UDP AND SENT. 
  Udp.endPacket();

  // once we send a packet to the server, it might
  // respond, so read it

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
//    if (len > 0) packetBuffer[len] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);


  }
}

void receive()
{
//  localPort = 5000;
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();

  // if nobody sent us anything, packetSize
  // would be zero, so a non-zero packetSize
  // means we have something to work on
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);


     int blueSwitchState = packetBuffer[0];
     int redSwitchState = packetBuffer[1];
     int greenSwitchState = packetBuffer[2];
     digitalWrite(blue, blueSwitchState);
     digitalWrite(yellow, redSwitchState);
     digitalWrite(green, greenSwitchState);


 
    // if we wanted to send anything back e.g. to
    // acknowledge the packet
    // this would be the place

  
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





 
