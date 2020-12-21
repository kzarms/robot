//
//  The 2WD robot ET.
//  Control via IR and serial port.
//
//===============================================
// Play melody on the buzz
//#include "pitches.h"
#include "songs.h"
// Include IR module from DLLs on PS. Afer import in the code.
#include <IRremote.h>


// IR remote control, get and store results
const int RECV_PIN = 2;

// Motor ports
// After testing, we fount that speed is not relevant. To slow.
// We will always use 1 on the EN
//const int lf_motor_en = 10;
//const int rt_motor_en = 5;

const int lf_motor_fw = 8;
const int lf_motor_bw = 9;
const int rt_motor_fw = 6;
const int rt_motor_bw = 7;

// Led light
// Head lights
int lf_head_led = 3;
int rt_head_led = 4;

// BUZZER port
const int BUZZER = 12;

// Ultrasonic ports
//int trig = A0;
//int echo = A1;

// IR sensors, left and right
// const int lf_ir = A5;
// const int rt_ir = A4;

// ===============================================
// Logical golbal variables

// Robot movement
// Current robot speed (left and right)
int LEFT = 0;
int RIGHT = 0;

// Delta robot sppeed (update)
int  dLEFT = 0;
int  dRIGHT = 0;

// A Serial String to hold incoming data
String SERIAL_STRING = "";
// whether the string is complete
bool B_SERIAL_STRING = false;

// Using interlal timer (avoid delay usage)
unsigned long P_MILLIS = 0;

// Melody notes
int notes = 0;
bool MELODY_PlAY = false;
// ===============================================
// IR init on the global level
IRrecv IrReceiver(RECV_PIN);

// ===============================================
// Main move function.
void move(int left, int right){
  // left and right are -9 .. +9
  // calculate it with map from 0..9 to 100..255
  // Check values for the left side
  if(left == 0){
    // left side is stop
    //digitalWrite(lf_motor_en, LOW);
    digitalWrite(lf_motor_fw, LOW);
    digitalWrite(lf_motor_bw, LOW);
    //digitalWrite(lf_led_pair, LOW);
  } else if (left > 0){
    // moving forward on the left side
    digitalWrite(lf_motor_fw, HIGH);
    digitalWrite(lf_motor_bw, LOW);
    //calculate speed forward
    //analogWrite(lf_motor_en, map(left, 1, 9, 100, 255));
    //digitalWrite(lf_led_pair, HIGH);
  } else {
    digitalWrite(lf_motor_fw, LOW);
    digitalWrite(lf_motor_bw, HIGH);
    //calculate speed forward
    //analogWrite(lf_motor_en, map((-1 * left), 1, 9, 100, 255));
    //digitalWrite(lf_led_pair, HIGH);
  }
  // Check values for the right side
  if(right == 0){
    //Left side is stop
    //digitalWrite(rt_motor_en, LOW);
    digitalWrite(rt_motor_fw, LOW);
    digitalWrite(rt_motor_bw, LOW);
    //digitalWrite(rt_led_pair, LOW);
  } else if (right > 0){
    //moving forward on the left side
    digitalWrite(rt_motor_fw, HIGH);
    digitalWrite(rt_motor_bw, LOW);
    //calculate speed forward
    //analogWrite(rt_motor_en, map(right, 1, 9, 100, 255));
    //digitalWrite(rt_led_pair, HIGH);
  } else {
    digitalWrite(rt_motor_fw, LOW);
    digitalWrite(rt_motor_bw, HIGH);
    //calculate speed and set PWM
    //analogWrite(rt_motor_en, map((-1 * right), 1, 9, 100, 255));
    //digitalWrite(rt_led_pair, HIGH);
  }
  //Serial.print("Left speed is: ");
  //Serial.println(left);
  //Serial.print("Right speed is: ");
  //Serial.println(right);
};
// Function to play the song
void play_melody(int melody[], int notes, int BUZZER){
  int tempo = 140;
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;

  const byte speakerPin=9;
  unsigned long lastPeriodStart;
  const int onDuration=1000;
  const int periodDuration=6000;

  if (millis()-lastPeriodStart>=periodDuration)
  {
    lastPeriodStart+=periodDuration;
    tone(speakerPin,550, onDuration); // play 550 Hz tone in background for 'onDuration'
  }


  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    if(MELODY_PlAY == false){
      return;
    }
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(BUZZER, melody[thisNote], noteDuration * 0.9);
    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    // stop the waveform generation before the next note.
    noTone(BUZZER);
  };
};
// ===============================================
// Initialization
void setup(){
  // Set serial. Use for direct command set and for BLE
  Serial.begin(9600);
  Serial.println("Starting...");
  // Init IR module
  IrReceiver.enableIRIn();  // Start the receiver
  IrReceiver.blink13(true); // Enable feedback LED

  // Initialize motor drive for output mode
  //pinMode(lf_motor_en, OUTPUT);
  //pinMode(rt_motor_en, OUTPUT);

  pinMode(lf_motor_fw, OUTPUT);
  pinMode(lf_motor_bw, OUTPUT);
  pinMode(rt_motor_fw, OUTPUT);
  pinMode(rt_motor_bw, OUTPUT);

  // Set led as output
  pinMode(lf_head_led, OUTPUT);
  pinMode(lf_head_led, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Set buzzer as output
  pinMode(BUZZER, OUTPUT);

  Serial.println("Rady to go!");
}
// Main
void loop(){
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();
  // Check if we have data in the serial
  if(B_SERIAL_STRING){
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
  if(IrReceiver.decode()){
    // Get the value from the IR
    uint32_t tCode = IrReceiver.results.value;
    //Serial.print(tCode, HEX);
    switch(tCode){
      //case 0x00FDB04F:  move(0,0); Serial.println("oo"); break; //   0  beep  OFF/ON
      case 0x00FD8877:  dLEFT = 1; dRIGHT = 1; break;   // up  Advance
      case 0x00FD28D7:  dLEFT = -1; dRIGHT = 1; break;  // <   Turn left
      case 0x00FDA857:  dLEFT = 0; dRIGHT = 0; MELODY_PlAY = false; break;   // ok   Stop
      case 0x00FD6897:  dLEFT = 1; dRIGHT = -1; break;  // >   Turn right
      case 0x00FD9867:  dLEFT = -1; dRIGHT = -1; break; // dw  Back
      // Melody play functions
      // remote control 1
      case 0x00FD00FF:
        notes = sizeof(happy_birthday_to_you) / sizeof(happy_birthday_to_you[0]) / 2;
        MELODY_PlAY = true;
        break;
      //case 0x00FD00FF: melody_to_play = 1; break;
      // remote control 2
      //case 0x00FD807F: melody_to_play = 2; break;
      //case 0x00FD807F: notes = sizeof(we_wish_you_a_merry_christmas) / sizeof(we_wish_you_a_merry_christmas[0]) / 2; play_melody(we_wish_you_a_merry_christmas, notes, BUZZER); break;
      //case 0x00FD40BF: g_AllState = 1; g_modeSelect = 1;  ModeBEEP(g_modeSelect); break; //3   line walking mode  3
      //case 0x00FD20DF: g_AllState = 1; g_modeSelect = 3;  ModeBEEP(g_modeSelect); break; //4   tacking mode  4
      default: Serial.println(tCode, HEX); break;
    }
    // Receive the next value
    IrReceiver.resume();
  }

  // move or not
  if(LEFT != dLEFT || RIGHT != dRIGHT){
    // Move motion values are not same. Correction.
    Serial.println("Execute the move function");
    move(dLEFT, dRIGHT);
    // Set current values in global
    LEFT = dLEFT;
    RIGHT = dRIGHT;
  }

  if(MELODY_PlAY){
    play_melody(happy_birthday_to_you, notes, BUZZER);
  }
  // only if robot moving
  if(LEFT != 0 || RIGHT != 0){
    //calculate duration in miliseconds
    //find max of abs values form current speed
    int duration = map((max(abs(LEFT), abs(LEFT))), 1, 9, 200, 20);
    int distance = map((max(abs(LEFT), abs(LEFT))), 1, 9, 10, 35);
    //Launch every 100ms
    if((currentMillis - P_MILLIS) >= duration){
      //Serial.println("Duration is ");
      //Serial.println(duration);
      //Serial.println("Max distance is ");
      //Serial.println(distance);
      P_MILLIS = currentMillis;
      // Code Execution
      //int dst = distance_test();
      int dst = 1000;
      //Less than 15cm
      if(dst < distance){
        dLEFT = 0;
        dRIGHT = 0;
        move(dLEFT, dRIGHT);
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
// Run each time after loop
// If loop has delay - it mpact on the responce
void serialEvent(){
  while(Serial.available()){
    // get the new byte:
    char inChar = (char)Serial.read();
    // Add it to the global SERIAL_STRING:
    SERIAL_STRING += inChar;
    //If it is the last symbol - set true. Clean in the next cycle
    if(inChar == '#'){
      B_SERIAL_STRING = true;
    }
  }
}
