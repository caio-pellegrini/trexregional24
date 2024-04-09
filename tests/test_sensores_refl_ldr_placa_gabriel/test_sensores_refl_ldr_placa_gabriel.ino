#define SD3 A0
#define SD2 A1
#define SD1 A2
#define SM A3
#define SE1 A4
#define SE2 A5
#define SE3 A6
#define LDRD A8
#define LDRE A9

int sd3, sd2, sd1, sm, se1, se2, se3, ldrd, ldre;

void setup() {
  Serial.begin(9600);
  pinMode(SD3, INPUT);
  pinMode(SD2, INPUT);
  pinMode(SD1, INPUT);
  pinMode(SM, INPUT);
  pinMode(SE1, INPUT);
  pinMode(SE2, INPUT);
  pinMode(SE3, INPUT);
  pinMode(LDRD, INPUT);
  pinMode(LDRE, INPUT);
}

void loop() {
  sd3 = analogRead(SD3);
  sd2 = analogRead(SD2);
  sd1 = analogRead(SD1);
  sm = analogRead(SM);
  se1 = analogRead(SE1);
  se2 = analogRead(SE2);
  se3 = analogRead(SE3);
  ldrd = analogRead(LDRD);
  ldre = analogRead(LDRE);
  
  Serial.print("SD3: ");
  Serial.print(sd3);
  Serial.print(" SD2: ");
  Serial.print(sd2);
  Serial.print(" SD1: ");
  Serial.print(sd1);
  Serial.print(" SM: ");
  Serial.print(sm);
  Serial.print(" SE1: ");
  Serial.print(se1);
  Serial.print(" SE2: ");
  Serial.print(se2);
  Serial.print(" SE3: ");
  Serial.print(se3);
  // Serial.print(" LDRD: ");
  // Serial.print(ldrd);
  // Serial.print(" LDRE: ");
  // Serial.print(ldre);

  Serial.println();
  delay(100);
}
