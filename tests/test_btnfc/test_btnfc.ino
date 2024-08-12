void setup() {
  // put your setup code here, to run once:
  pinMode(A7, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("btnFcEsq: ");
  Serial.println(!digitalRead(A7));
}
