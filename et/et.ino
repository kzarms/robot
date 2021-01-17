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

// ESP8266 GPIO pin to use. Recommended: 4 (D1).
const uint16_t kRecvPin = 5;
IRrecv irrecv(kRecvPin);

decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);
}

void loop() {
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    Serial.println("");
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}