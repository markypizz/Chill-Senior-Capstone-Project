int message; 

const byte ON = 2;
const byte OFF = 1;

const int LED = 4; //Can easily be replaced with the valve

void setup() {
  //Serial Begin at 9600 Baud 
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  message = Serial.read();

  if (message == ON) {
    digitalWrite(LED,HIGH);
  } else if (message == OFF) {
    digitalWrite(LED,LOW);
  }
  
  delay(1000);
}
