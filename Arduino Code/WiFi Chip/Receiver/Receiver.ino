#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin("ChillShower", "jamespark");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  Serial.write(2);
  delay(10000);
  Serial.write(1);

}   

void loop() {
  delay(1000);
}
