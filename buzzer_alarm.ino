#include <SimpleTimer.h>

const int buzzer1=9;
const int buzzer2=8;
SimpleTimer timer;

// print current time
void TimerPrint() {
  int hrs,mins,sec;
  sec = millis() / 1000;
  mins = sec / 60;
  hrs = sec / 3600;
  sec = sec - mins * 60;
  mins = mins - hrs * 60;
  Serial.print(hrs);
  semicolonPrint(mins);
  semicolonPrint(sec);
  Serial.println();
}
//print semicolon and zero in between hrs, mins, and sec
void semicolonPrint(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(buzzer1, OUTPUT);
pinMode(buzzer2, OUTPUT);
SystemStartAlert();

    timer.setInterval(300000, firstwarning); // calls function after 5 min, numbers in milisec
    timer.setInterval(1000, TimerPrint); //prints time every 1 sec
    

}
void firstwarning() {
tone(buzzer1,500);            //((buzzer1,500) to send 500Hz sound signal
delay(200);                     // for 0.5 sec
noTone(buzzer1);
tone(buzzer2,500);              //same
delay(500);
noTone(buzzer2);

    timer.setInterval(180000, secondwarning); // calls function after 3min
}
void secondwarning() {
  timer.setTimer(5000, buzzerfunc1, 3);

    timer.setInterval(60000, Repeat); // repeats the function every 1min
}
void Repeat() {                  //Calling functions to make the buzzer beep alternatively.
  timer.setTimer(3000, buzzerfunc2, 5);

}

void loop() {
  timer.run();
}


void SystemStartAlert(){
  tone(buzzer1,1000);
tone(buzzer2,1000);
delay(1000);
noTone(buzzer1);
noTone(buzzer2);
}

void buzzerfunc1(){             //function for buzzer1
  tone(buzzer1,700);            //((buzzer1,500) to send 500Hz sound signal
delay(500);                     // for 0.5 sec
noTone(buzzer1);
  tone(buzzer2,700);            //((buzzer1,500) to send 500Hz sound signal
delay(700);                     // for 0.5 sec
noTone(buzzer2);
}
void buzzerfunc2(){             //function for buzzer1
  tone(buzzer1,1000);            //((buzzer1,500) to send 500Hz sound signal
delay(300);                     // for 0.5 sec
noTone(buzzer1);
  tone(buzzer2,1000);            //((buzzer1,500) to send 500Hz sound signal
delay(500);                     // for 0.5 sec
noTone(buzzer2);
}


