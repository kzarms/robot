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

// ESP8266 GPIO pin to use.
const uint16_t kRecvPin = 5; // D1
IRrecv irrecv(kRecvPin);
decode_results results;

// Head lights
const uint16_t lf_head_led = 4; // D2
const uint16_t rt_head_led = 0; // D4


void setup() {
  Serial.begin(115200);
  // Set led as output
  pinMode(lf_head_led, OUTPUT);
  pinMode(lf_head_led, OUTPUT);

  // Start the IR receiver
  irrecv.enableIRIn();

  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);

  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);
}

void loop() {
  if (irrecv.decode(&results)) {
    uint32_t tCode = results.value;
    //Serial.print(tCode, HEX);
    switch(tCode){
        //case 0x00FDB04F:  move(0,0); Serial.println("oo"); break; //   0  beep  OFF/ON
        //case 0x00FD8877:  dLEFT = 1; dRIGHT = 1; break;   // up  Advance
        //case 0x00FD28D7:  dLEFT = -1; dRIGHT = 1; break;  // <   Turn left
        //case 0x00FDA857:  dLEFT = 0; dRIGHT = 0; MELODY_PlAY = false; break;   // ok   Stop
        //case 0x00FD6897:  dLEFT = 1; dRIGHT = -1; break;  // >   Turn right
        //case 0x00FD9867:  dLEFT = -1; dRIGHT = -1; break; // dw  Back
        // Melody play functions
        // remote control 1
        case 0x00FD00FF:
            digitalWrite(lf_head_led, HIGH); break;
        break;
        //case 0x00FD00FF: melody_to_play = 1; break;
        // remote control 2
        //case 0x00FD807F: melody_to_play = 2; break;
        //case 0x00FD807F: notes = sizeof(we_wish_you_a_merry_christmas) / sizeof(we_wish_you_a_merry_christmas[0]) / 2; play_melody(we_wish_you_a_merry_christmas, notes, BUZZER); break;
        //case 0x00FD40BF: g_AllState = 1; g_modeSelect = 1;  ModeBEEP(g_modeSelect); break; //3   line walking mode  3
        //case 0x00FD20DF: g_AllState = 1; g_modeSelect = 3;  ModeBEEP(g_modeSelect); break; //4   tacking mode  4
        default: Serial.println(tCode, HEX); digitalWrite(lf_head_led, LOW); break;
    }

    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    Serial.println("");
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}
