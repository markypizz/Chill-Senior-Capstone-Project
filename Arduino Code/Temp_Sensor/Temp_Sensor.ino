#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <SimpleTimer.h>
#include <Servo.h>

const int TSensor=4;
const int ONLED=7;
const int READYLED=2;
const int BUTTON=13;

const byte ON = 2;
const byte OFF = 1;

const int MAX_HOT = 35;

bool waitingCycles = false;
int counterForHotWater = 0;
bool timerStarted = false;
bool extraDelay = false;

OneWire oneWire(TSensor);

DallasTemperature sensors(&oneWire);

SimpleTimer timer;
const int buzzer1=9;
const int buzzer2=8;

typedef struct {
  int32_t timer1;
  int32_t timer2;
  int16_t targetTemp;
  int16_t currentTemp;
  uint8_t servoAngle; //cold valve
  bool mainLineClosed;
  bool showerReady;
} stats;

typedef struct {
  int32_t timer1;
  int32_t timer2;
  int16_t targetTemp;
} returnStats;

stats statuses;
returnStats returnStatuses;

void setup(void) {
  // put your setup code here, to run once:
  pinMode(ONLED,OUTPUT);
  pinMode(READYLED,OUTPUT);
  pinMode(BUTTON, INPUT);

  delay(1000);
  Serial.begin(9600);

  delay(1000);
  sensors.begin();

  int temp = EEPROMReadInt(0);
  long timer1 = EEPROMReadlong(2);
  long timer2 = EEPROMReadlong(6);
  
  if ((temp < 10) || (temp > 60)) { //default value
    temp = 30; //default temp
  }

  if ((timer1 < 1) || (timer1 > 10)) { //default value
    timer1 = 300000; //5 minutes
  }

  if ((timer2 < 1) || (timer2 > 10)) { //default value
    timer2 = 180000; //5 minutes
  }
  
  //Write it back
  EEPROMWriteInt(0, temp);
  EEPROMWritelong(2, timer1);
  EEPROMWritelong(6, timer2);

  memset(&statuses, 0, sizeof(statuses));
  
  statuses.targetTemp = temp;
  returnStatuses.targetTemp = temp;
  statuses.currentTemp = 0; 
  returnStatuses.timer1 = timer1;
  returnStatuses.timer2 = timer2;
  statuses.timer1 = timer1;
  statuses.timer2 = timer2;

  //Bizarre bug ???
  statuses.servoAngle = 180; //VALVE CLOSED
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
  timer.setTimeout(statuses.timer2, secondwarning); // calls function after 3min
}

void secondwarning() {
  timer.setTimer(5000, buzzerfunc1, 3);
  timer.setInterval(60000, repeat); // repeats the function every 1min
}
void repeat() {                  //Calling functions to make the buzzer beep alternatively.
  timer.setTimer(3000, buzzerfunc2, 5); // buzzerfunc2 will run 5 times every 3sec when function repeat is ran
  statuses.servoAngle = 180;
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
  
  if (waitingCycles) {
    counterForHotWater += 1;
    
    if (counterForHotWater == 3) {
      waitingCycles = false;
    }
    
  } else {
    sensors.requestTemperatures();
    int currentTemp = sensors.getTempCByIndex(0);
    
    if (counterForHotWater == 3) {
      if (!(statuses.showerReady)) {
        
        
        //Shower not ready
        if (currentTemp > statuses.targetTemp) {
          //Alert with app
          statuses.showerReady = true;
          
          //Alert with Buzzers and LED
          showerReadyAlert();
          digitalWrite(READYLED,HIGH);
          
        } else {
          statuses.servoAngle += 20;
          if (statuses.servoAngle > 180) {
            statuses.servoAngle = 180;
          }
        }
      } else {
        // Shower begun, start timer
        if (timerStarted == false) {
          timer.setTimeout(statuses.timer1, firstwarning);
          timerStarted = true;
        }

        if (!statuses.mainLineClosed) {
          if (currentTemp > statuses.targetTemp) {
            if (statuses.servoAngle < 20) {
              statuses.servoAngle = 20;
            }
            statuses.servoAngle -= 20;
          } else if (currentTemp < statuses.targetTemp){
            statuses.servoAngle += 20;
            if (statuses.servoAngle > 180) {
              statuses.servoAngle = 180;
            }
          }
        }

        //Main shower on code
        timer.run();

        int buttonState = digitalRead(BUTTON);

        //Briefly press and hold button to activate
        if (buttonState == HIGH) {
          //flip state of master valve
          statuses.mainLineClosed = !(statuses.mainLineClosed);
          extraDelay = true;
        }
      }
    } else {
      
      if (Serial.available()) {
          //Serial.println(Serial.available());
          Serial.readBytes((char*)&returnStatuses, sizeof(returnStatuses));

          statuses.targetTemp = returnStatuses.targetTemp;
          statuses.timer1 = returnStatuses.timer1;
          statuses.timer2 = returnStatuses.timer2;
        }
      //Waiting to enable cold valve
      if (currentTemp > MAX_HOT) {
        //Open cold valve
        statuses.servoAngle = 90;
        waitingCycles = true;
        counterForHotWater = 1;
      } else {
        //Serial.println(statuses.coldValveClosed);
        //statuses.servoAngle = 90; //half open cold water
        //Serial.println(statuses.coldValveClosed);
      }
    }
  }
  
  
  //Update current temperature
  statuses.currentTemp = (int)sensors.getTempCByIndex(0);
  
  Serial.write((char *)&statuses,sizeof(statuses));
  delay(2000);

  if (extraDelay) {
    delay(3000);
    extraDelay = false;
  }
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

long EEPROMReadlong(long address)
{
    //Read the 4 bytes from the eeprom memory.
    long four = EEPROM.read(address);
    long three = EEPROM.read(address + 1);
    long two = EEPROM.read(address + 2);
    long one = EEPROM.read(address + 3);

    //Return the recomposed long by using bitshift.
    return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
