#include <OneWire.h>
#include <DallasTemperature.h>

const int TSensor=4;
const int LED=7;
const int VALVE=2;

const byte ON = 2;
const byte OFF = 1;

OneWire
oneWire(TSensor);

DallasTemperature
sensors(&oneWire);



void setup(void) {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  pinMode(VALVE,OUTPUT);
 
  Serial.begin(9600);
  sensors.begin();

}

void loop(void) {
  sensors.requestTemperatures();
  //Serial.print("Celsius temperature\n");
  //Serial.print(sensors.getTempCByIndex(0));
  //Serial.print("\n");

  if (sensors.getTempCByIndex(0) > 30) {
    //Serial.print("\nHot enough!\n\n");
    //digitalWrite(LED,HIGH);
  
    //Close valve
    //Serial.print("\nValve On");
    //digitalWrite(VALVE,HIGH);
    Serial.write(ON);
  } else {
    //digitalWrite(LED,LOW);
    //digitalWrite(VALVE,LOW);
    Serial.write(OFF);
  }
  delay(1000);
  
}
