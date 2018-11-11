#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <SimpleTimer.h>

const int TSensor=4;
const int ONLED=7;
const int READYLED=2;

const byte ON = 2;
const byte OFF = 1;

const int MAX_HOT = 35;

bool waitingCycles = false;
int counterForHotWater = 0;
bool timerStarted = false;

OneWire oneWire(TSensor);

DallasTemperature sensors(&oneWire);

SimpleTimer timer;
const int buzzer1=9;
const int buzzer2=8;

typedef struct {
  int targetTemp;
  int currentTemp;
  bool coldValveClosed;
  bool mainLineClosed;
  bool showerReady;
} stats;

stats statuses;

void setup(void) {
  // put your setup code here, to run once:
  pinMode(ONLED,OUTPUT);
  pinMode(READYLED,OUTPUT);
 
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
  statuses.coldValveClosed = true;
  statuses.mainLineClosed = false;
  statuses.showerReady = false;

  digitalWrite(ONLED,HIGH);
}

void firstwarning() {
  tone(buzzer1,500);            //((buzzer1,500) to send 500Hz sound signal
  delay(200);                     // for 0.5 sec
  noTone(buzzer1);
  tone(buzzer2,500);              //same
  delay(500);
  noTone(buzzer2);
  timer.setTimeout(180000, secondwarning); // calls function after 3min
}

void secondwarning() {
  timer.setTimer(5000, buzzerfunc1, 3);
  timer.setInterval(60000, repeat); // repeats the function every 1min
}
void repeat() {                  //Calling functions to make the buzzer beep alternatively.
  timer.setTimer(3000, buzzerfunc2, 5); // buzzerfunc2 will run 5 times every 3sec when function repeat is ran
}

void buzzerfunc1(){             //function for buzzer1
  tone(buzzer1,700);            //((buzzer1,500) to send 700Hz sound signal
  delay(500);                     // for 0.5 sec
  noTone(buzzer1);
  tone(buzzer2,700);            //((buzzer1,500) to send 700Hz sound signal
  delay(700);                     // for 0.7 sec
  noTone(buzzer2);
}

void buzzerfunc2(){             //function for buzzer1
  tone(buzzer1,1000);            //((buzzer1,500) to send 1000Hz sound signal
  delay(300);                     // for 0.3 sec
  noTone(buzzer1);
  tone(buzzer2,1000);            //((buzzer1,500) to send 1000Hz sound signal
  delay(500);                     // for 0.5 sec
  noTone(buzzer2);
}

void showerReadyAlert(){
  tone(buzzer1,1000);         //((buzzer1,1000) to send 1000Hz sound signal
  tone(buzzer2,1000);           //((buzzer2,1000) to send 1000Hz sound signal
  delay(1000);                     // for 1 sec
  noTone(buzzer1);
  noTone(buzzer2);
}

void loop(void) {
  //
  // Look here for incoming iphone messages
  // from serial port
  //
  //
  
  if (waitingCycles) {
    counterForHotWater += 1;
    
    if (counterForHotWater == 5) {
      //Open cold valve
      
      
      waitingCycles = false;
    }
    
  } else {
    sensors.requestTemperatures();
    int currentTemp = sensors.getTempCByIndex(0);
    
    if (counterForHotWater == 5) {
      if (!(statuses.showerReady)) {
        //Shower not ready
        if (currentTemp > statuses.targetTemp) {
          //Alert with app
          statuses.showerReady = true;
          
          //Alert with Buzzers and LED
          showerReadyAlert();
          digitalWrite(READYLED,HIGH);
          
        }
      } else {
        // Shower begun, start timer
        if (timerStarted == false) {
          timer.setTimeout(300000, firstwarning);
          timerStarted = true;
        }

        //Poll timer
        timer.run();
      }
    } else {
      //Waiting to enable cold valve
      if (currentTemp > MAX_HOT) {
        //Open cold valve
        statuses.coldValveClosed = false;
        waitingCycles = true;
        counterForHotWater = 1;
      }
    }
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
