#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

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

  int temp = EEPROMReadInt(0);

  if ((temp < 10) || (temp > 60)) { //default value
    temp = 30; //default temp
  }
  //Write it back
  EEPROMWriteInt(0, temp);  
  
  statuses.targetTemp = temp;
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

  //Update current temperature
  statuses.currentTemp = (int)sensors.getTempCByIndex(0);
  
  Serial.write((char *)&statuses,sizeof(statuses));
  delay(2000);
}

void EEPROMWriteInt(int address, int value)
{
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  
  EEPROM.update(address, two);
  EEPROM.update(address + 1, one);
}
 
int EEPROMReadInt(int address)
{
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);
 
  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}
