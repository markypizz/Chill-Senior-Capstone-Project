#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const byte ON = 2;
const byte OFF = 1;
char message[] = "Hello there!";

byte currentSend = 2;
unsigned int localPort = 4211;
IPAddress broadcastIP(255,255,255,255);

WiFiUDP udp;

typedef struct {
  int32_t timer1;
  int32_t timer2;
  int16_t targetTemp;
  int16_t currentTemp;
  uint8_t servoAngle;
  bool mainLineClosed;
  bool showerReady;
} stats;

typedef struct {
  int32_t timer1;
  int32_t timer2;
  int16_t targetTemp;
} response;

stats statuses;
response reStats;

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
  statuses.timer1 = 300000;
  statuses.timer2 = 180000;
  statuses.mainLineClosed = false;
  statuses.servoAngle = 180;
  statuses.showerReady = false;

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
    //Serial.println(Serial.available());
    Serial.readBytes((char*)&statuses, sizeof(statuses));
    //Serial.write(1);
    udp.beginPacket(broadcastIP,4210);
    udp.write((char *)&statuses,sizeof(statuses));
    udp.endPacket();
  }
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

    int packetSize = udp.parsePacket();
    if (packetSize != 0) {
      Serial.write("Made it");
    }
    if (packetSize != sizeof(statuses)) {
      int len = udp.read((char *)&reStats,sizeof(reStats));
      if (len > 0) {
        //Serial.println(statuses.currentTemp);
        //Serial.println("Current temp: " + statuses.currentTemp);
        //Serial.println("Cold valve: " + statuses.coldValveClosed);
        Serial.write((char *)&reStats,sizeof(reStats));
    }
  }

    delay(100);
      
}
