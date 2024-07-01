#define S1 A9
#define S2 A10
#define S3 A11
#define S4 A12
#define S5 A13
#define S6 A14
#define S7 A15

void setup() {
    Serial.begin(9600);
    pinMode(S1, INPUT);
    pinMode(S2, INPUT);
    pinMode(S3, INPUT);
    pinMode(S4, INPUT);
    pinMode(S5, INPUT);
    pinMode(S6, INPUT);
    pinMode(S7, INPUT);
}

void loop() {
    Serial.print("S1: ");
    Serial.print(analogRead(S1));
    Serial.print(" S2: ");
    Serial.print(analogRead(S2));
    Serial.print(" S3: ");
    Serial.print(analogRead(S3));
    Serial.print(" S4: ");
    Serial.print(analogRead(S4));
    Serial.print(" S5: ");
    Serial.print(analogRead(S5));
    Serial.print(" S6: ");
    Serial.print(analogRead(S6));
    Serial.print(" S7: ");
    Serial.println(analogRead(S7));
    delay(100);
}