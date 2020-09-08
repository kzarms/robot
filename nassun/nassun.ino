int tr1=8;
int tr2=9;
int tr3=6;
int tr4=7;

void setup(){
  // Set motor type
  pinMode(tr1, OUTPUT);
  pinMode(tr2, OUTPUT);
  pinMode(tr3, OUTPUT);
  pinMode(tr4, OUTPUT);

  // Turn off all
  digitalWrite(tr1, LOW);
  digitalWrite(tr2, LOW);
  digitalWrite(tr3, HIGH);
  digitalWrite(tr4, HIGH);
  // Serial
  Serial.begin(9600);
  Serial.println("Rady to go!");
}
void loop(){
    // Start workning on
    // SET 1 and 4 open, 2,3 - close
    digitalWrite(tr1, HIGH);
    digitalWrite(tr2, LOW);
    digitalWrite(tr3, HIGH);
    digitalWrite(tr4, LOW);
    delay(1000);
    // SET 2 and 3 open, 1,4 - close
    digitalWrite(tr1, LOW);
    digitalWrite(tr2, HIGH);
    digitalWrite(tr3, LOW);
    digitalWrite(tr4, HIGH);
    delay(1000);
}