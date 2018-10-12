#include <ESP8266WiFi.h>

const byte ON = 2;
const byte OFF = 1;
int message;

WiFiServer server(80);

void setup()
{
  Serial.println("Beginning");
  Serial.begin(9600);
  WiFi.softAP("ChillShower", "jamespark");
  server.begin();
  Serial.println("Done");
}

void loop()
{
  //Send message to clients
  //message = Serial.read();

  //Write to clients
  //if (message == ON) {
  //  server.write(ON);
  //} else if (message == OFF) {
  //  server.write(OFF);
  //}

  if (server.available()) {
    Serial.println("Looping send, receive");
    //Test line
    server.print(ON + "\r");
  }
}
