#include <Servo.h>
Servo myservo;

uint8_t angle = 90;
uint8_t newAngle = 90;
const int servo = 9;

int pos = 0;
void setup() {
    Serial.begin(9600);
      myservo.attach(servo); //pin number
}

void loop() {

  if (Serial.available() > 0) {
    newAngle = Serial.read();

    if (angle != newAngle) {
      if (newAngle < angle) {
        while (newAngle != angle) {
          myservo.write(angle);
          angle -= 1;
          delay(50);
        }
      } else {
        while (newAngle != angle) {
          myservo.write(angle);
          angle += 1;
          delay(50);
        }
      }
      angle = newAngle;
    }
  }
}
