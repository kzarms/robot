//Motor ports
int motor_en=9;
int motor_fw=6;
int motor_bw=7;

int car_speed = 0;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(motor_en, OUTPUT);
  pinMode(motor_fw, OUTPUT);
  pinMode(motor_bw, OUTPUT);
}


// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  int speedTmp = map(sensorValue,0,1023,0,255);
  if(car_speed != speedTmp){
    car_speed = speedTmp;
    if(car_speed <= 1){
      digitalWrite(motor_fw, LOW);
      Serial.println("Sotop");
    } else {      
      Serial.println(car_speed);
      digitalWrite(motor_fw, HIGH);
      //analogWrite(motor_fw, car_speed);
      digitalWrite(motor_bw, LOW);
      //Set speed
      analogWrite(motor_en, car_speed);
      //digitalWrite(motor_en, HIGH);
    }
    delay(100);
  } 
  
}
