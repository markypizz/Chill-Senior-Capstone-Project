#include <Servo.h>
Servo myservo;

void setup() {
    Serial.begin(9600);
      myservo.attach(9); //pin number

  myservo.writeMicroseconds(1500); // from 1000~2000, 1500 is the middle. some motors run from 700~2300
}

void loop() {

  myservo.writeMicroseconds(2300); // 2000 or 2300 to turn fully clockwise
  delay(2000);
    myservo.writeMicroseconds(700); // 700 or 1000 to turn fully counter clockwise
  delay(2000);
}
