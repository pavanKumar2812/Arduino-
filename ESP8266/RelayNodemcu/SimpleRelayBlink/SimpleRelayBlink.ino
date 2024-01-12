int relayInput = D0;
void setup() {
  Serial.begin(115200);
  pinMode(relayInput, OUTPUT);
}

void loop() {
  digitalWrite(relayInput, HIGH);
  Serial.println("High");
  delay(1000);
  digitalWrite(relayInput, LOW);
  Serial.println("Low");
  delay(1000);
}