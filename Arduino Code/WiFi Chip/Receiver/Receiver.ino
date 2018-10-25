#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const byte ON = 2;
const byte OFF = 1;

unsigned int localPort = 4210;             

WiFiUDP udp;

byte udpResponse = 0;
char incomingPacket[255];

//int message;

void setup() {
  Serial.begin(9600);
  // connect to wifi.
  WiFi.mode(WIFI_STA);
  WiFi.begin("ChillShower", "jamespark");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Now connect to server object
  //if (client.connect(ip, 80) == 1) {
  //  Serial.println("Connected");
  //} else {
  //  Serial.println("Not Connected");
  //}
  
  Serial.println(2);
  delay(3000);
  Serial.println(1);

  while (!udp.begin(localPort)) {
    Serial.println("Could not start UDP");
  }

}   

void loop() {
  //if (client.connected()) {
  //  client.write(1);
  //  Serial.println("Connected\n");
  //  while(!client.available()){}
  //  Serial.println("Available");
  //  byte message = client.read();
  //  Serial.println(message);

    //Send message to arduino
    //if (message == ON || message == OFF) {
    //  Serial.write(message);
    //}
  
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
    int len = udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
}
