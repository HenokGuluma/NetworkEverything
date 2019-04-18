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

int sensePin = A0;  //This is the Arduino Pin that will read the sensor output
int sensorInput;    //The variable we will use to store the sensor input
double temp;        //The variable we will use to store temperature in degrees. 
const int tempmax = 72;

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int myPort = 5000;      // local port to listen on
unsigned int remoteServerPort = 7000;  // remote port to talk to
IPAddress remoteServerAddress(192, 168, 1, 14);

char packetBuffer[255]; //buffer to hold incoming packet

WiFiUDP Udp;

// Sensors
const int BUTTON_PIN = 0;

// Actuators
const int NUMBER_OF_ACTUATORS = 3;
int actuators[NUMBER_OF_ACTUATORS] = {
  3, // pin numbers
  4,
  5
};

String LED_ON_MESSAGE = "ledON";
String LED_OFF_MESSAGE = "ledOFF";

// remember the button state so we only send
// when the state changes
boolean buttonState;
boolean lastButtonState = LOW; // arbitrary
boolean tempState = LOW;
boolean lastTempState=LOW;
boolean c = HIGH;
float previoustime = 0;
void setup() {

  // Set all actuator pins as outputs
  for (int i = 0; i < NUMBER_OF_ACTUATORS; i++ ) {
    pinMode(actuators[i], OUTPUT);
  }
  
  //Initialize serial
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  // Remove this if running without a serial monitor
  while (!Serial) {
    ; 
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
  Serial.println(myPort);
  Udp.begin(myPort);
}

void loop() {

  sensorInput = analogRead(A0);    //read the analog sensor and store it
  temp = (double)sensorInput / 1024;       //find percentage of input reading
  temp = temp * 5;                 //multiply by 5V to get voltage
  temp = temp - 0.5;               //Subtract the offset 
  temp = temp * 100;               //Convert to degrees 
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

  buttonState = digitalRead(BUTTON_PIN);
  if (temp > tempmax && c == HIGH){
    tempState = HIGH;
    lastTempState = HIGH;
    previoustime=millis();
    c = LOW;
  }
  else if (temp<tempmax)
    tempState = LOW;
  if ((buttonState != lastButtonState) || (tempState!=lastTempState)) {
    if (buttonState != lastButtonState)
      Serial.println("button state changed; sending new state");
    else if ( tempState!= lastTempState)
      Serial.println("temperature has risen above the maximum");
    Udp.beginPacket(remoteServerAddress, remoteServerPort);
    Udp.write(buttonState);
    Udp.write(lastTempState);
    Udp.endPacket();
    

    lastButtonState = buttonState;
    lastTempState = tempState;
    
  }
  float lasttime= millis();
  float timediff = lasttime-previoustime;
  if (timediff >=4000)
  {
    lastTempState = LOW;  
    c == HIGH;
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
