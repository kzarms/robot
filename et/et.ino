//
//  The 2WD robot ET.
//  Control via IR and serial port.
//
//===============================================
// WiFi


// Include IR module from DLLs on PS. Afer import in the code.
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// ESP8266 GPIO pins to use.
const uint16_t RECV_PIN = 5; // D1
IRrecv irrecv(RECV_PIN);
decode_results results;

// Speaker
const uint16_t BUZZER = 4;   // D2

// Head lights
const uint16_t lf_head_led = 0;   // D3
const uint16_t rt_head_led = 2;   // D4

// Motor ports
const uint16_t lf_motor_fw = 13;  // D7
const uint16_t lf_motor_bw = 15;  // D8
const uint16_t rt_motor_fw = 14;  // D5
const uint16_t rt_motor_bw = 12;  // D6

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

// ===============================================
// Functions
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
void move(int left, int right){
    // Main move function.
    // Check values for the left side
    if (left == 0) {
        // left side is stop
        digitalWrite(lf_motor_fw, LOW);
        digitalWrite(lf_motor_bw, LOW);
    } else if (left > 0){
        // moving forward on the left side
        digitalWrite(lf_motor_fw, HIGH);
        digitalWrite(lf_motor_bw, LOW);
    } else {
        digitalWrite(lf_motor_fw, LOW);
        digitalWrite(lf_motor_bw, HIGH);
    }
    // Check values for the right side
    if(right == 0){
        // Left side is stop
        digitalWrite(rt_motor_fw, LOW);
        digitalWrite(rt_motor_bw, LOW);
    } else if (right > 0){
        // moving forward on the left side
        digitalWrite(rt_motor_fw, HIGH);
        digitalWrite(rt_motor_bw, LOW);
    } else {
        digitalWrite(rt_motor_fw, LOW);
        digitalWrite(rt_motor_bw, HIGH);
    }
};
// ===============================================
// Execution
void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");
    // Set led as output
    pinMode(lf_head_led, OUTPUT);
    pinMode(lf_head_led, OUTPUT);

    // Initialize motor drive for output mode
    pinMode(lf_motor_fw, OUTPUT);
    pinMode(lf_motor_bw, OUTPUT);
    pinMode(rt_motor_fw, OUTPUT);
    pinMode(rt_motor_bw, OUTPUT);

    // Start the IR receiver
    irrecv.enableIRIn();

    while (!Serial)  // Wait for the serial connection to be establised.
        delay(50);
    tone(BUZZER, 523);
    delay (1000);
    noTone(BUZZER);
    Serial.println();
    Serial.print("Rady to go!");
}

void loop() {
    // Commands from the IR
    if (irrecv.decode(&results)) {
        uint32_t tCode = results.value;
        //Serial.print(tCode, HEX);
        switch(tCode){
            //case 0x00FDB04F:  move(0,0); Serial.println("oo"); break; //   0  beep  OFF/ON
            case 0x00FD8877:  dLEFT = 1; dRIGHT = 1; break;   // up  Advance
            case 0x00FD28D7:  dLEFT = -1; dRIGHT = 1; break;  // <   Turn left
            case 0x00FDA857:  dLEFT = 0; dRIGHT = 0; break;   // ok   Stop
            case 0x00FD6897:  dLEFT = 1; dRIGHT = -1; break;  // >   Turn right
            case 0x00FD9867:  dLEFT = -1; dRIGHT = -1; break; // dw  Back
            // Melody play functions
            // remote control 1
            case 0x00FD00FF:
                digitalWrite(lf_head_led, HIGH); break;
            //case 0x00FD00FF: melody_to_play = 1; break;
            // remote control 2
            case 0x00FD807F: digitalWrite(lf_head_led, LOW); break;
            //case 0x00FD807F: notes = sizeof(we_wish_you_a_merry_christmas) / sizeof(we_wish_you_a_merry_christmas[0]) / 2; play_melody(we_wish_you_a_merry_christmas, notes, BUZZER); break;
            //case 0x00FD40BF: g_AllState = 1; g_modeSelect = 1;  ModeBEEP(g_modeSelect); break; //3   line walking mode  3
            //case 0x00FD20DF: g_AllState = 1; g_modeSelect = 3;  ModeBEEP(g_modeSelect); break; //4   tacking mode  4
            default: Serial.println(tCode, HEX); break;
        }

        // print() & println() can't handle printing long longs. (uint64_t)
        //serialPrintUint64(results.value, HEX);
        //Serial.println("");
        irrecv.resume();  // Receive the next value
    }

    // Commands from the Serial
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

    if (LEFT != dLEFT || RIGHT != dRIGHT) {
        // Move motion values are not same. Correction.
        Serial.println("Execute the move function");
        move(dLEFT, dRIGHT);
        // Set current values in global
        LEFT = dLEFT;
        RIGHT = dRIGHT;
    }
}

