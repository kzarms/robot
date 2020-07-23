//
//  The 4WD robot essun.
//  Control only via BT
//

//===============================================
//Physical pinout connection
// IR remote control
int ir = 2;
//Motor ports
int lf_motor_en=10;
int rt_motor_en=5;

int lf_motor_fw=8;
int lf_motor_bw=9;
int rt_motor_fw=6;
int rt_motor_bw=7;

// Led light pair
int lf_led_pair=3;
int rt_led_pair=4;

// Ultrasonic ports
int trig = A0;
int echo = A1;

// IR sensors, left and right
const int lf_ir = A5;
const int rt_ir = A4;

// BUZZER port
int BUZZER = 12;

// Button port
int button = 13;
/*
//Line Walking
const int SensorLeft = A3;     	// Set Left Line Walking Infrared sensor port
const int SensorRight = A2;   	// Set Right Line Walking Infrared sensor port
int SL;                         // State of Left Line Walking Infrared sensor
int SR;                         // State of Right Line Walking Infrared sensor

//State
int g_carstate = enSTOP;        //  1:front 2:back 3:left 4:right 0:stop // State of vehicle running state
int g_modeSelect = 0;           // 0:remote control mode(default); 1:line walking mode ; 2: obsracle avoidance mode; 3: tracking
int g_modeComunication = 0;     // 0:Infrared remote control 1:Bluetooth remote control
int g_AllState = 0;             // 0: Busying; 1:Mode selection
//int g_IRRealse = 0;           //Remote control buttons loosen detection
*/

//===============================================
//Logical golbal variables

// Car movement
//Current car speed (left and right)
int LEFT = 0;
int RIGHT = 0;
//New car sppeed received
int  dLEFT = 0;
int  dRIGHT = 0;

// A Serial String to hold incoming data
String SERIAL_STRING = "";
// whether the string is complete
bool B_SERIAL_STRING = false;

// Using interlal timer (avoid delay usage)
unsigned long P_MILLIS = 0;
//===============================================
//region functions

// Measuring front distance
long distance_test(){
  //Define internal vars
  long duration, distance;
  //Make a short impuls OFF - ON  -OFF
  digitalWrite(trig, LOW);
  //delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(3);
  digitalWrite(trig, LOW);
  //read in from echo pin
  duration = pulseIn(echo, HIGH);
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  // duration/29/2 or duration/58
  return duration / 58;
}
// Car control functions
/*
void car_fw(int car_speed){
  // Both motor enable
  digitalWrite(lf_motor_en, HIGH);
  digitalWrite(rt_motor_en, HIGH);
  // Start engings forward
  digitalWrite(lf_motor_fw, HIGH);
  digitalWrite(lf_motor_bw, LOW);
  digitalWrite(rt_motor_fw, HIGH);
  digitalWrite(rt_motor_bw, LOW);
  // Turn ON the lights
  digitalWrite(lf_led_pair, HIGH);
  digitalWrite(rt_led_pair, HIGH);
  //Set speed
  analogWrite(lf_motor_en, car_speed);
  analogWrite(rt_motor_en, car_speed);
}
void car_lf(int car_speed){
  // Left disable, right enable
  digitalWrite(lf_motor_en, LOW);
  digitalWrite(rt_motor_en, HIGH);
  // Start only right engings forward
  digitalWrite(lf_motor_fw, LOW);
  digitalWrite(lf_motor_bw, LOW);
  digitalWrite(rt_motor_fw, HIGH);
  digitalWrite(rt_motor_bw, LOW);
  // Turn ON the lights
  digitalWrite(lf_led_pair, LOW);
  digitalWrite(rt_led_pair, HIGH);
  //Set speed
  //analogWrite(lf_motor_en, car_speed);
  analogWrite(rt_motor_en, car_speed);
}
void car_lf_r(int car_speed){
  // Both engings are on
  digitalWrite(lf_motor_en, HIGH);
  digitalWrite(rt_motor_en, HIGH);
  // Both engins in reverse
  digitalWrite(lf_motor_fw, LOW);
  digitalWrite(lf_motor_bw, HIGH);
  digitalWrite(rt_motor_fw, HIGH);
  digitalWrite(rt_motor_bw, LOW);
  // Turn ON the lights on the side we turn on
  digitalWrite(lf_led_pair, LOW);
  digitalWrite(rt_led_pair, HIGH);
  //Set speed
  analogWrite(lf_motor_en, car_speed);
  analogWrite(rt_motor_en, car_speed);
}
void car_rt(int car_speed){
  // Left disable, right enable
  digitalWrite(lf_motor_en, HIGH);
  digitalWrite(rt_motor_en, LOW);
  // Start only right engings forward
  digitalWrite(lf_motor_fw, HIGH);
  digitalWrite(lf_motor_bw, LOW);
  digitalWrite(rt_motor_fw, LOW);
  digitalWrite(rt_motor_bw, LOW);
  // Turn ON the lights
  digitalWrite(lf_led_pair, HIGH);
  digitalWrite(rt_led_pair, LOW);
  //Set speed
  analogWrite(lf_motor_en, car_speed);
  //analogWrite(rt_motor_en, car_speed);
}
void car_rt_r(int car_speed){
  // Both engings are on
  digitalWrite(lf_motor_en, HIGH);
  digitalWrite(rt_motor_en, HIGH);
  // Both engins in reverse
  digitalWrite(lf_motor_fw, HIGH);
  digitalWrite(lf_motor_bw, LOW);
  digitalWrite(rt_motor_fw, LOW);
  digitalWrite(rt_motor_bw, HIGH);
  // Turn ON the lights on the side we turn on
  digitalWrite(lf_led_pair, HIGH);
  digitalWrite(rt_led_pair, LOW);
  //Set speed
  analogWrite(lf_motor_en, car_speed);
  analogWrite(rt_motor_en, car_speed);
}
void car_bw(int car_speed){
  // Both motor enable
  digitalWrite(lf_motor_en, HIGH);
  digitalWrite(rt_motor_en, HIGH);
  // Start engings forward
  digitalWrite(lf_motor_fw, LOW);
  digitalWrite(lf_motor_bw, HIGH);
  digitalWrite(rt_motor_fw, LOW);
  digitalWrite(rt_motor_bw, HIGH);
  // Turn ON the lights
  digitalWrite(lf_led_pair, HIGH);
  digitalWrite(rt_led_pair, HIGH);
  //Set speed
  analogWrite(lf_motor_en, car_speed);
  analogWrite(rt_motor_en, car_speed);
}
void car_stop(){
  // Both motors stop
  digitalWrite(lf_motor_en, LOW);
  digitalWrite(rt_motor_en, LOW);
  // All engings stop
  digitalWrite(lf_motor_fw, LOW);
  digitalWrite(lf_motor_bw, LOW);
  digitalWrite(rt_motor_fw, LOW);
  digitalWrite(rt_motor_bw, LOW);
  // Turn OFF the lights
  digitalWrite(lf_led_pair, LOW);
  digitalWrite(rt_led_pair, LOW);
}
void car_control(String cmd){
  if(cmd.length() == 0){
    Serial.println("Empty command");
    return;
  }
  if(cmd.startsWith("0")){
    //Stop the car command.
    Serial.println("Stop the car");
    car_stop();
  }
  else if (cmd.startsWith("1")){
    int speed = cmd.substring(1,2).toInt();
    Serial.println(speed);
    switch(speed){
      case 1:
        Serial.println("Start the car with speed 1");
        car_fw(120);
        break;
      case 2:
        Serial.println("Start the car with speed 2");
        car_fw(185);
        break;
      case 3:
        Serial.println("Start the car with speed 3");
        car_fw(255);
        break;
      default:
        Serial.println("Can not crecognice speed. Abort.");
        break;
    }
  } else if (cmd.startsWith("2")){
    //Left the car command.
    Serial.println("Left the car");
    car_lf_r(150);
  } else if (cmd.startsWith("3")){
    //Left the car command.
    Serial.println("Right the car");
    car_rt_r(150);
  } else if (cmd.startsWith("4")){
    //Left the car command.
    Serial.println("Back the car");
    car_bw(150);
  }
  else {
    Serial.println("Wrong command!");
  }
}
*/
void car_move(int left, int right){
  // left and right are -9 .. +9
  int leftSpeed = 0;
  int rightSpeed = 0;
  //Check values for the left side
  if(left == 0){
    //Left side is stop
    digitalWrite(lf_motor_en, LOW);
    digitalWrite(lf_motor_fw, LOW);
    digitalWrite(lf_motor_bw, LOW);
    digitalWrite(lf_led_pair, LOW);
  } else if (left > 0){
    //moving forward on the left side
    digitalWrite(lf_motor_fw, HIGH);
    digitalWrite(lf_motor_bw, LOW);
    //calculate speed forward
    leftSpeed = map(left, 1, 9, 100, 255);
    analogWrite(lf_motor_en, leftSpeed);
    digitalWrite(lf_led_pair, HIGH);
  } else {
    digitalWrite(lf_motor_fw, LOW);
    digitalWrite(lf_motor_bw, HIGH);
    //calculate speed forward
    leftSpeed = map((-1 * left), 1, 9, 100, 255);
    analogWrite(lf_motor_en, leftSpeed);
    digitalWrite(lf_led_pair, HIGH);
  }
  // Check values for the right side
  if(right == 0){
    //Left side is stop
    digitalWrite(rt_motor_en, LOW);
    digitalWrite(rt_motor_fw, LOW);
    digitalWrite(rt_motor_bw, LOW);
    digitalWrite(rt_led_pair, LOW);
  } else if (right > 0){
    //moving forward on the left side
    digitalWrite(rt_motor_fw, HIGH);
    digitalWrite(rt_motor_bw, LOW);
    //calculate speed forward
    rightSpeed = map(right, 1, 9, 100, 255);
    analogWrite(rt_motor_en, rightSpeed);
    digitalWrite(rt_led_pair, HIGH);
  } else {
    digitalWrite(rt_motor_fw, LOW);
    digitalWrite(rt_motor_bw, HIGH);
    //calculate speed and set PWM
    rightSpeed = map((-1 * right), 1, 9, 100, 255);
    analogWrite(rt_motor_en, rightSpeed);
    digitalWrite(rt_led_pair, HIGH);
  }
  Serial.print("Left speed is: ");
  Serial.println(left);
  Serial.print("Right speed is: ");
  Serial.println(right);
}
void beep(int beep_delay){
  digitalWrite(BUZZER, HIGH);
  delay(beep_delay);
  digitalWrite(BUZZER, LOW);
}
void chk_button(void){
  if(!digitalRead(button)){
      Serial.println("Manual start the car");
      beep(100);
      //car_fw(185);
  }
}
//endregion
//Initialization
void setup(){
  //IR
  pinMode(ir, INPUT_PULLUP);
  //Initialize motor drive for output mode
  pinMode(lf_motor_en, OUTPUT);
  pinMode(rt_motor_en, OUTPUT);

  pinMode(lf_motor_fw, OUTPUT);
  pinMode(lf_motor_bw, OUTPUT);
  pinMode(rt_motor_fw, OUTPUT);
  pinMode(rt_motor_bw, OUTPUT);

  //Set led as output
  pinMode(lf_led_pair, OUTPUT);
  pinMode(rt_led_pair, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);


  // Set Ultrasonic echo port as input and trig as output
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  // Set buzzer as output
  pinMode(BUZZER, OUTPUT);

  // Set button as input and internal pull-up mode
  pinMode(button, INPUT_PULLUP);

  // Set Bluetooth baud rate 9600
  Serial.begin(9600);

  beep(100);

  Serial.println("Rady to go!");
}
//Main
void loop(){
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();
  // Check if we have data in the serial
  if (B_SERIAL_STRING) {
    //Print data in the screen:
    Serial.println("We got:");
    Serial.println(SERIAL_STRING);
    int sep = SERIAL_STRING.indexOf(',');
    // Save values into temp
    dLEFT = SERIAL_STRING.substring(0, sep).toInt();
    dRIGHT = SERIAL_STRING.substring((sep + 1), (SERIAL_STRING.length() - 1)).toInt();
    // clear the string:
    SERIAL_STRING = "";
    B_SERIAL_STRING = false;
  }
  if(LEFT != dLEFT || RIGHT != dRIGHT){
    //Move the car with new values
    Serial.println("Execute in car_move function");
    car_move(dLEFT, dRIGHT);
    LEFT = dLEFT;
    RIGHT = dRIGHT;
  }
  //
  chk_button();

  //sensivity = max(abs(LEFT), abs(LEFT));
  //only if car moving
  if(LEFT != 0 || RIGHT != 0){
    //calculate duration in miliseconds
    //find max of abs values form current speed
    int duration = map((max(abs(LEFT), abs(LEFT))), 1, 9, 200, 20);
    int distance = map((max(abs(LEFT), abs(LEFT))), 1, 9, 10, 35);
    //Launch every 100ms
    if ((currentMillis - P_MILLIS) >= duration) {
      Serial.println("Duration is ");
      Serial.println(duration);
      Serial.println("Max distance is ");
      Serial.println(distance);
      P_MILLIS = currentMillis;
      // Code Execution
      int dst = distance_test();
      //Less than 15cm
      if(dst < distance){
        dLEFT = 0;
        dRIGHT = 0;
        car_move(dLEFT, dRIGHT);
        LEFT = dLEFT;
        RIGHT = dRIGHT;
        Serial.println("Emergency stop at ");
        Serial.println(dst);
      } else {
        //Serial.print(dst);
        //Serial.println("cm");
      }
    }
  }
}
//Run each time after loop
//If loop has delay - it mpact on the responce
void serialEvent(){
  while (Serial.available()){
    // get the new byte:
    char inChar = (char)Serial.read();
    // Add it to the global SERIAL_STRING:
    SERIAL_STRING += inChar;
    //If it is the last symbol - set true. Clean in the next cycle
    if (inChar == '#'){
      B_SERIAL_STRING = true;
    }
  }
}
