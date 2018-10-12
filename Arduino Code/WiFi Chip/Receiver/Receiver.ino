#include <ESP8266WiFi.h>

const byte ON = 2;
const byte OFF = 1;

byte ip[] = {192, 168, 4, 1};
WiFiClient client;

int message;

void setup() {
  Serial.begin(9600);
  // connect to wifi.
  WiFi.mode(WIFI_STA);
  WiFi.begin("ChillShower", "jamespark");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Now connect to server object
  if (client.connect(ip, 80) == 1) {
    Serial.println("Connected");
  } else {
    Serial.println("Not Connected");
  }
  
  //Serial.write(2);
  delay(10000);
  //Serial.write(1);

}   

void loop() {
  client.connect(ip, 80);
  if (client.connected()) {
    client.write(1);
    Serial.println("Connected\n");
    while(!client.available()){}
    Serial.println("Available");
    String message = client.readStringUntil('\r');
    Serial.println(message);

    //Send message to arduino
    //if (message == ON || message == OFF) {
    //  Serial.write(message);
    //}
  }
  client.flush();
  client.stop();
}
