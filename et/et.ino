//
//  The 2WD robot ET.
//  Control via IR
//

//===============================================
#include "pitches.h"
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

// BUZZER port
int BUZZER = 12;

// A Serial String to hold incoming data
String SERIAL_STRING = "";
// whether the string is complete
bool B_SERIAL_STRING = false;

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

const int threshold = 10;

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

  // Set Ultrasonic echo port as input and trig as output
  //pinMode(echo, INPUT);
  //pinMode(trig, OUTPUT);
  // Set buzzer as output
  pinMode(BUZZER, OUTPUT);

  // Set button as input and internal pull-up mode
  //pinMode(button, INPUT_PULLUP);

  // Set Bluetooth baud rate 9600
  Serial.begin(9600);
  Serial.println("Melody!");
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER);
  }

  Serial.println("Rady to go!");
}
//Main

void loop(){
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();
  // Check if we have data in the serial
  if(B_SERIAL_STRING){
    //Print data in the screen:
    Serial.print("We got: ");
    Serial.println(SERIAL_STRING);
    // Execute command
    if(SERIAL_STRING == "f#"){
      // Move forward
      Serial.println("Moving forward");
      digitalWrite(rt_motor_fw, HIGH);
      digitalWrite(rt_motor_bw, LOW);
      analogWrite(rt_motor_en, 150);
    } else if (SERIAL_STRING == "b#"){
      Serial.println("Moving backward");
      digitalWrite(rt_motor_fw, LOW);
      digitalWrite(rt_motor_bw, HIGH);
      //digitalWrite(rt_motor_en, HIGH);
      //analogWrite(rt_motor_en, 100);
    } else if (SERIAL_STRING == "s#"){
      Serial.println("Stop");
      digitalWrite(rt_motor_fw, LOW);
      digitalWrite(rt_motor_bw, LOW);
      digitalWrite(rt_motor_en, LOW);
    } else {
      Serial.println("Command not found. Use f#, b# or s#");
    }

    // clear the string:
    SERIAL_STRING = "";
    B_SERIAL_STRING = false;
  }
}
//Run each time after loop
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
