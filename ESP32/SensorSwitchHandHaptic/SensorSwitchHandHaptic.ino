void setup() {
  // put your setup code here, to run once:
  pinMode(32, INPUT); 
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(32) == HIGH) {
    digitalWrite(34, HIGH);
    digitalWrite(35, HIGH);
    delay(200);

    digitalWrite(34, LOW);
    digitalWrite(35, LOW);
    delay(500);

    digitalWrite(34, HIGH);
    digitalWrite(35, HIGH);
    delay(400);

    digitalWrite(34, LOW);
    digitalWrite(35, LOW);
    delay(500);

    digitalWrite(34, HIGH);
    digitalWrite(35, HIGH);
    delay(800);

    digitalWrite(34, LOW);
    digitalWrite(35, LOW);
  }
}
