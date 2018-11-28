#include <Servo.h>
Servo myservo;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
      myservo.attach(9); //pin number
      
  myservo.writeMicroseconds(1500); 
}

void loop() {
  // put your main code here, to run repeatedly:

  myservo.writeMicroseconds(2300); // 2000 or 2300 clockwise
  delay(2000);
    myservo.writeMicroseconds(700); // 700 or 1000 counter clockwise
  delay(2000);
}
