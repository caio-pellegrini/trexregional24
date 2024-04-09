
#define LUZ 900
#define LUZ_F 500

// MOTORES
#define MOTOR_DF 8   // azul claro
#define MOTOR_DT 9   // azul escuro
#define MOTOR_EF 11  // verde escuro
#define MOTOR_ET 10  // verde claro

// SENSORES DE REFLETÂNCIA
#define SE3_PIN A8
#define SE2_PIN A9
#define SE1_PIN A15
#define SE0_PIN A14
#define SD0_PIN A13
#define SD1_PIN A12
#define SD2_PIN A10
#define SD3_PIN A11

// Adafruit_TCS34725 tcsEsq = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

QTRSensors qtrc;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];
uint16_t sFE3, sFE2, sFE1, sFD1, sFD2, sFD3;
uint16_t sE3, sE2, sE1, sE0, sD0, sD1, sD2, sD3;

void segueLinhaEsquerda(int tempo) {
  analogWrite(MOTOR_DF, 220);
  analogWrite(MOTOR_DT, 0);
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, 195);
  delay(tempo);
}

void segueLinhaDireita(int tempo) {
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, 195);
  analogWrite(MOTOR_EF, 220);
  analogWrite(MOTOR_ET, 0);
  delay(tempo);
}

void setup() {
  Serial.begin(9600);
  Serial.println();

  // PORTA MOTORES
  pinMode(MOTOR_DF, OUTPUT);
  pinMode(MOTOR_DT, OUTPUT);
  pinMode(MOTOR_EF, OUTPUT);
  pinMode(MOTOR_ET, OUTPUT);

  //PORTA SENSORES REFLETANCIA
  pinMode(SE2_PIN, INPUT);
  pinMode(SE1_PIN, INPUT);
  pinMode(SD1_PIN, INPUT);
  pinMode(SD2_PIN, INPUT);
  pinMode(SE3_PIN, INPUT);
  pinMode(SE0_PIN, INPUT);
  pinMode(SD3_PIN, INPUT);
  pinMode(SD0_PIN, INPUT);

  qtrc.setTypeRC();
  qtrc.setSensorPins((const uint8_t[]){32, 34, 36, 38, 40, 42}, SensorCount);


  // Serial.println(tcsEsq.begin() ? "TCS34725 #2 conectado :)" : "TCS34725 #2 conexão falhou :(");
}

void loop() {

  sE2 = analogRead(SE2_PIN);
  sE1 = analogRead(SE1_PIN);
  sD1 = analogRead(SD1_PIN);
  sD2 = analogRead(SD2_PIN);

  analogWrite(MOTOR_DF, 85);
  analogWrite(MOTOR_DT, 0);
  analogWrite(MOTOR_EF, 85);
  analogWrite(MOTOR_ET, 0);
  delay(1);

  if (sE1 >= LUZ || sE2 >= LUZ) {
    segueLinhaEsquerda(1);
  }

  if (sD1 >= LUZ || sD2 >= LUZ) {
    segueLinhaDireita(1);
  }
  
  // sE3 = analogRead(SE3_PIN);
  // sE2 = analogRead(SE2_PIN);
  // sE1 = analogRead(SE1_PIN);
  // sE0 = analogRead(SE0_PIN);
  // sD0 = analogRead(SD0_PIN);
  // sD1 = analogRead(SD1_PIN);
  // sD2 = analogRead(SD2_PIN);
  // sD3 = analogRead(SD3_PIN);

  // SENSOR REFLETÂNCIA FRENTE
  // qtrc.read(sensorValues);
  // sFE3 = map(sensorValues[0], 0, 2500, 0, 1023);
  // sFE2 = map(sensorValues[1], 0, 2500, 0, 1023);
  // sFE1 = map(sensorValues[2], 0, 2500, 0, 1023);
  // sFD1 = map(sensorValues[3], 0, 2500, 0, 1023);
  // sFD2 = map(sensorValues[4], 0, 2500, 0, 1023);
  // sFD3 = map(sensorValues[5], 0, 2500, 0, 1023);
  


  // if (sD1 >= LUZ && sD2 >= LUZ && sE1 >= LUZ && sE2 >= LUZ && sE0 >= LUZ && sD0 >= LUZ && sE3 >= LUZ && sD3 >= LUZ) {

  //   analogWrite(MOTOR_DF, 0);
  //   analogWrite(MOTOR_DT, 0);
  //   analogWrite(MOTOR_EF, 0);
  //   analogWrite(MOTOR_ET, 0);
  //   delay(200);

  //   uint16_t r, g, b, c;
  //   tcsEsq.getRawData(&r, &g, &b, &c);
  //   if (map(g, 0, 6200, 0, 1023) < 100) {

  //     analogWrite(MOTOR_DF, 100);
  //     analogWrite(MOTOR_DT, 0);
  //     analogWrite(MOTOR_EF, 100);
  //     analogWrite(MOTOR_ET, 0);
  //     delay(450);


  //     analogWrite(MOTOR_DF, 150);
  //     analogWrite(MOTOR_DT, 0);
  //     analogWrite(MOTOR_EF, 0);
  //     analogWrite(MOTOR_ET, 150);
  //     delay(1500);

  //     analogWrite(MOTOR_DF, 100);
  //     analogWrite(MOTOR_DT, 0);
  //     analogWrite(MOTOR_EF, 100);
  //     analogWrite(MOTOR_ET, 0);
  //     delay(400);

  //     analogWrite(MOTOR_DF, 0);
  //     analogWrite(MOTOR_DT, 0);
  //     analogWrite(MOTOR_EF, 0);
  //     analogWrite(MOTOR_ET, 0);
  //     delay(1000);
  //   }


    // analogWrite(MOTOR_DF, 0);
    // analogWrite(MOTOR_DT, 0);
    // analogWrite(MOTOR_EF, 0);
    // analogWrite(MOTOR_ET, 0);
    // delay(200);
    // analogWrite(MOTOR_DF, 100);
    // analogWrite(MOTOR_DT, 0);
    // analogWrite(MOTOR_EF, 100);
    // analogWrite(MOTOR_ET, 0);
    // delay(50);
    // analogWrite(MOTOR_DF, 0);
    // analogWrite(MOTOR_DT, 0);
    // analogWrite(MOTOR_EF, 0);
    // analogWrite(MOTOR_ET, 0);
    // delay(200);
    // sE3 = analogRead(SE3_PIN);
    // sE2 = analogRead(SE2_PIN);
    // sE1 = analogRead(SE1_PIN);
    // sE0 = analogRead(SE0_PIN);
    // sD0 = analogRead(SD0_PIN);
    // sD1 = analogRead(SD1_PIN);
    // sD2 = analogRead(SD2_PIN);
    // sD3 = analogRead(SD3_PIN);

    // if (sD1 >= LUZ && sD2 >= LUZ && sE1 >= LUZ && sE2 >= LUZ && sE0 >= LUZ && sD0 >= LUZ && sE3 >= LUZ && sD3 >= LUZ) {
    // analogWrite(MOTOR_DF, 0);
    // analogWrite(MOTOR_DT, 0);
    // analogWrite(MOTOR_EF, 0);
    // analogWrite(MOTOR_ET, 0);
    // delay(5000);
    // }

  //}

}
