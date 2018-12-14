/*References:
Code is based on https://www.instructables.com/id/Use-a-DIY-Pressure-Plate-Switch-to-Automate-Your-H/
//Except I am doing a pull down
Combined code with parts of the magnet_sensor_MQTT.ino code and sonic.ino code from IoTLab4
  https://techtutorialsx.com/2017/04/09/esp8266-connecting-to-mqtt-broker/
  https://www.learnrobotics.org/blog/how-to-configure-node-red-and-wemos-d1-mini-using-mqtt/
  https://www.baldengineer.com/mqtt-tutorial.html
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h> //allows to connect to MQTT broker

//WiFi
const char* ssid = "IOTCC"; //WiFi Name
const char* password = "IOTCC515"; //WiFi Password
const char* mqtt_server = "192.168.3.1"; //Raspberry Pi's (MQTT Broker) IP
const int mqttPort = 1883; //default port

//pressure plate setup
//numbers are according to the GPIO layout: https://wiki.wemos.cc/products:d1:d1_mini
const int buttonPin=0; //D3 -- this is the input for the pressure plate
const int ledPin =2; //D4 -- this is just the light on the arduino.

//inputs for pushbutton in Ground and D3

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

//MQTT setup
WiFiClient espClient;
PubSubClient client(espClient);

//void callback(char* topic, byte* payload, unsigned int length) {
// 
//  Serial.print("Message arrived in topic: "); //Which channel
//  Serial.println(topic);
// 
//  Serial.print("Message:"); //prints the message from the channel
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//  }
// 
//  Serial.println();
//  Serial.println("-----------------------"); //Seperates the messages
// 
//}


void setup() {
//Pressure Sensor 
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);     //LED on the arduino
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);    //need to do a pullup because of the pin using. Give me fixed levels.

//MQTT Stuff
 Serial.begin(115200); //console

 WiFi.begin(ssid, password); //Connect to WiFi
 Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500); //0.5 second wait
    Serial.print(".");
  }

 //Show connection details
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 client.setServer(mqtt_server, mqttPort);
// client.setCallback(callback);

 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("Pressure")){ //If connected to MQTT Server
  Serial.println("connected");
 } 
 else { //if not connected to MQTT Server
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }

}


void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266Magnet")) {
  Serial.println("connected");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

//Variables for comparing when there's a change: when there's pressure and when there's no pressure.
  char *CurrentStatus="n"; //current status
  char *PriorStatus="n"; //Past status

void loop(){
   if (!client.connected()) {
    reconnect();
  }

  //Code for pressure sensor
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  // check if the pushbutton is pressed.
  
  if (buttonState == LOW) {    //pushbutton is pressed
    // turn LED on:
    digitalWrite(ledPin,LOW);
    CurrentStatus = "y";
  }
  else {                      //pushbutton is not pressed
    // turn LED off
    digitalWrite(ledPin,HIGH);
    CurrentStatus = "n"; //
  }

//Only publishing to MQTT when there's a change.
  if (CurrentStatus != PriorStatus){
    client.publish("pressure/sensor", CurrentStatus);
    PriorStatus = CurrentStatus;
    Serial.println("Pushed update");
  }

    delay(100); //Putting a delay for 100ms

  client.loop(); //Keep going through the code.
}
