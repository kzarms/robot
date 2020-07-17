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

// BUZZER port
int BUZZER = 12;

// Button port
int KEY = 13;
/*
//Line Walking
const int SensorLeft = A3;     	// Set Left Line Walking Infrared sensor port
const int SensorRight = A2;   	// Set Right Line Walking Infrared sensor port
int SL;                         // State of Left Line Walking Infrared sensor
int SR;                         // State of Right Line Walking Infrared sensor

//follow
const int SensorLeft_2 = A5;    // Left Tracking Infrared sensor
const int SensorRight_2 = A4;   // Right Tracking Infrared sensor
int SL_2;                       // State of Left Tracking Infrared sensor
int SR_2;                       // State of Right Tracking Infrared sensor

//State
int g_carstate = enSTOP;        //  1:front 2:back 3:left 4:right 0:stop // State of vehicle running state
int g_modeSelect = 0;           // 0:remote control mode(default); 1:line walking mode ; 2: obsracle avoidance mode; 3: tracking
int g_modeComunication = 0;     // 0:Infrared remote control 1:Bluetooth remote control
int g_AllState = 0;             // 0: Busying; 1:Mode selection
//int g_IRRealse = 0;           //Remote control buttons loosen detection
*/

//===============================================
//Logical golbal variables

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
  //Make a short impuls OFF - ON 5μs -OFF
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(3);
  digitalWrite(trig, LOW);
  //read infor from echo pin
  duration = pulseIn(echo, HIGH);
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  // duration/29/2 or duration/58
  return duration / 58;
}
// Car control functions
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
  digitalWrite(lf_led_pair, HIGH);
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
  digitalWrite(lf_led_pair, LOW);
}
void car_control(String command){
  int speed = 200;
  if(command == "0#"){
    Serial.println("Stop the car");
    car_stop();
  }
  else if (command == "1#"){
    Serial.println("Start the car");
    car_fw(speed);
  }
  else{
    Serial.println("Wrong command!");
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
  pinMode(KEY, INPUT_PULLUP);

  // Set Bluetooth baud rate 9600
  Serial.begin(9600);

  //Bip sould - I am loaded!
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}
//Main
void loop(){
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();
  String cmd;
  // Check if we have data in the serial
  if (B_SERIAL_STRING) {
    //Print data in the screen:
    cmd = SERIAL_STRING;
    Serial.println("We got:");
    Serial.println(cmd);
    Serial.println("Execute in car_control function");
    car_control(cmd);
    // clear the string:
    SERIAL_STRING = "";
    B_SERIAL_STRING = false;
  }

  //Launch every 500ms
  if (currentMillis - P_MILLIS >= 2000) {
    // save the last time you blinked the LED
    P_MILLIS = currentMillis;
    // Code Execution
    Serial.print(distance_test());
    Serial.print("cm");
    Serial.println();
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
