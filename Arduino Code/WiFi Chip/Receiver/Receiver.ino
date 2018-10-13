#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const byte ON = 2;
const byte OFF = 1;

unsigned int localPort = 8888;             

WiFiUDP udp;

byte udpResponse = 0;

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

  while (! udp.begin(localPort)) {
    Serial.println("Waiting to start UDP");
    yield();
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
  
  //client.flush();

  int size = udp.read(&udpResponse,1);

  if (size > 0) {
    Serial.println("Made it");
    Serial.write(udpResponse);
  }
  //delay(2000);
}
