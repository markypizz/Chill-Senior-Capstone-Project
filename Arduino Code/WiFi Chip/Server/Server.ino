#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const byte ON = 2;
const byte OFF = 1;
char message[] = "Hello there!";

byte currentSend = 2;
unsigned int localPort = 4210;
IPAddress broadcastIP(255,255,255,255);

WiFiUDP udp;

typedef struct {
  int targetTemp;
  int currentTemp;
  bool hotValveClosed;
  bool coldValveClosed;
  bool grayWaterClosed;
} stats;

stats statuses;

void setup()
{
  Serial.println("Beginning");
  Serial.begin(9600);
  WiFi.softAP("ChillShower", "jamespark");
  //broadcastIP = ~WiFi.subnetMask() | WiFi.gatewayIP();
  Serial.println("Done");

  udp.begin(localPort);

  statuses.targetTemp = 30;
  statuses.currentTemp = 0;
  statuses.hotValveClosed = true;
  statuses.coldValveClosed = false;
  statuses.grayWaterClosed = false;
}

void loop()
{
  //Send message to clients
  //message = currentSend;

  if (Serial.available()) {
    int i = 0;

    while (i < sizeof(statuses)) {
      *(((byte *)&statuses) + i) = Serial.read();
    }
  }

  udp.beginPacket(broadcastIP,localPort);
  udp.write(statuses);
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

    //if (currentSend == ON) {
    //  currentSend = OFF;
    //} else {
    //  currentSend = ON;
    //}

    delay(2000);
      
}
