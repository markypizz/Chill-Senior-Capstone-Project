#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const byte ON = 2;
const byte OFF = 1;
byte message;

byte currentSend = 2;
unsigned int localPort = 8888;
IPAddress broadcastIP;

WiFiUDP udp;

void setup()
{
  WiFi.mode(WIFI_AP_STA);;
  Serial.println("Beginning");
  Serial.begin(9600);
  WiFi.softAP("ChillShower", "jamespark");
  broadcastIP = ~WiFi.subnetMask() | WiFi.gatewayIP();
  Serial.println("Done");

  while (! udp.begin(localPort)) {
    Serial.println("Waiting to start UDP");
    yield();
  }
}

void loop()
{
  //Send message to clients
  message = currentSend;

  udp.beginPacket(broadcastIP,localPort);
  udp.write(message);
  udp.endPacket();
  Serial.println("Wrote...");

  //if (client) {
  //  if (client.connected()) {
  //    client.read(); //Clear request
  //    Serial.println("Sending");
//
  //    //Flip send state
   //   if (currentSend == ON) {
   //     currentSend = OFF;
   //   } else {
   //     currentSend = ON;
   //   }
   ///   
   //   //Test line
   //   client.write(message);
   // }
  //}

    if (currentSend == ON) {
      currentSend = OFF;
    } else {
      currentSend = ON;
    }

    delay(2000);
      
}
