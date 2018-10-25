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

typedef struct {
  int targetTemp;
  int currentTemp;
  bool hotValveClosed;
  bool coldValveClosed;
  bool grayWaterClosed;
} stats;

stats statuses;

void setup(void) {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  pinMode(VALVE,OUTPUT);
 
  Serial.begin(9600);
  sensors.begin();

  statuses.targetTemp = 30;
  statuses.currentTemp = 0;
  statuses.hotValveClosed = true;
  statuses.coldValveClosed = false;
  statuses.grayWaterClosed = false;

}

void loop(void) {
  sensors.requestTemperatures();

  if (sensors.getTempCByIndex(0) > 30) {
    statuses.hotValveClosed = true;
  } else {
    statuses.hotValveClosed = false;
  }
  Serial.write((byte*)&statuses,sizeof(statuses));
  delay(2000);
}
