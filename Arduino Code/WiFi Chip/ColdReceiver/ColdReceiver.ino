#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

unsigned int localPort = 4210;             

WiFiUDP udp;

byte udpResponse = 0;
char incomingPacket[255];

typedef struct {
  int32_t timer1;
  int32_t timer2;
  int16_t targetTemp;
  int16_t currentTemp;
  uint8_t servoAngle;
  bool mainLineClosed;
  bool showerReady;
} stats;

stats statuses;

//int message;

void setup() {
  Serial.begin(9600);
  // connect to wifi.
  WiFi.mode(WIFI_STA);
  WiFi.begin("ChillShower", "jamespark");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.println("Not connected");
  }

  //Serial.println("Connected");

  //Now connect to server object
  //if (client.connect(ip, 80) == 1) {
  //  Serial.println("Connected");
  //} else {
  //  Serial.println("Not Connected");
  //}
  
  delay(3000);

  while (!udp.begin(localPort)) {
    Serial.println("Could not start UDP");
  }

  //Serial.println("Listening");

  //irrelevant data
  statuses.targetTemp = 0;
  statuses.currentTemp = 0; 
  statuses.timer1 = 0;
  statuses.timer2 = 0;
  statuses.servoAngle = 90;
  statuses.mainLineClosed = false;
  statuses.showerReady = false;

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
  if (packetSize > 0) {
    //Serial.println("Reading packet");
    int len = udp.read((char *)&statuses,sizeof(statuses));
    if (len > 0) {
      //Serial.println("Length > 0");
      //Serial.println(statuses.currentTemp);
      //Serial.println("Current temp: " + statuses.currentTemp);
      //Serial.println("Cold valve: " + statuses.coldValveClosed);

      //Write desired servo angle
      //Serial.println(statuses.servoAngle);
      Serial.write(statuses.servoAngle);
    }
  }
}
