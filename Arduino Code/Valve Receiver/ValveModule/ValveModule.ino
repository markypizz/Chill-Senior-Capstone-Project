bool message; 

const int VALVE = 2;

void setup() {
  //Serial Begin at 9600 Baud 
  pinMode(VALVE,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    message = Serial.read();
  
    //message -1 if no data available
    
    if (message == false) {
      digitalWrite(VALVE,LOW);
    } else if (message == true) {
      //Close valve
      digitalWrite(VALVE,HIGH);
    }
  }
}
