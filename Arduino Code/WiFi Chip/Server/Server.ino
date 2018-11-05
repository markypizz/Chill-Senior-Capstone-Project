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
  WiFi.disconnect();
  WiFi.softAP("ChillShower", "jamespark");
  //broadcastIP = ~WiFi.subnetMask() | WiFi.gatewayIP();
  Serial.println("Done");

  udp.begin(localPort);

  statuses.targetTemp = 30;
  statuses.currentTemp = 0;
  statuses.hotValveClosed = true;
  statuses.coldValveClosed = false;
  statuses.grayWaterClosed = false;

  //Wait until data arrives
  while (Serial.available() < sizeof(statuses)) {delay(100);}

  Serial.readBytes((char*)&statuses, sizeof(statuses));
  
  delay(500);
}

void loop()
{
  
  //Send message to clients
  //message = currentSend;

  if (Serial.available()) {
    Serial.println(Serial.available());
    Serial.readBytes((char*)&statuses, sizeof(statuses));
    Serial.write(1);
  }

  udp.beginPacket(broadcastIP,localPort);
  udp.write((char *)&statuses,sizeof(statuses));
  udp.endPacket();
  //Serial.println("Wrote...");

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

    delay(10);
      
}
