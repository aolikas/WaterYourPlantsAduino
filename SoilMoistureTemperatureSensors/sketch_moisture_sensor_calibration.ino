void setup() {
  Serial.begin(9600); // open serial port
}

void loop() {
 int val;
  val = analogRead(0); //connect sensor to Analog 0
  Serial.println(val); //print the value to serial port
  delay(100);
}
