#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>

Servo servoPaGarra;
Servo servoSubirGarra;
Servo servoRotacionarGarra;
Servo servoCancelaEsq;
Servo servoCancelaDir;

#define SERVO_PA_GARRA_PIN 44
#define SERVO_SUBIR_GARRA_PIN 46
#define SERVO_ROTACIONAR_GARRA_PIN 45
#define SERVO_CANCELA_ESQ_PIN 13
#define SERVO_CANCELA_DIR_PIN 12

#define SERVO_PA_GARRA_POS_INICIAL 80
#define SERVO_SUBIR_GARRA_POS_INICIAL 160
#define SERVO_ROTACIONAR_GARRA_POS_INICIAL 55
#define SERVO_CANCELA_ESQ_POS_INICIAL 3
#define SERVO_CANCELA_DIR_POS_INICIAL 99

// Declare dois objetos sensor
VL53L0X laserFrente;
VL53L0X laserGarra;
int distanciaLaserGarra;

// Define os pinos XSHUT para cada sensor
#define LASER_FRENTE_XSHUT 14
#define LASER_GARRA_XSHUT 19

#define BTN_VITIMA_PIN 33

bool vitimaViva, btnVitima;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(LASER_FRENTE_XSHUT, OUTPUT);
  pinMode(LASER_GARRA_XSHUT, OUTPUT);

  digitalWrite(LASER_FRENTE_XSHUT, LOW);
  digitalWrite(LASER_GARRA_XSHUT, LOW);
  delay(50);

  digitalWrite(LASER_FRENTE_XSHUT, HIGH);
  delay(50);
  laserFrente.setTimeout(500);
  laserFrente.init();
  laserFrente.setAddress(0x30);

  digitalWrite(LASER_GARRA_XSHUT, HIGH);
  delay(50);
  laserGarra.setTimeout(500);
  laserGarra.init();
  laserGarra.setAddress(0x31);

  pinMode(BTN_VITIMA_PIN, INPUT_PULLUP);

  servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

  servoPaGarra.write(SERVO_PA_GARRA_POS_INICIAL);
  servoSubirGarra.write(SERVO_SUBIR_GARRA_POS_INICIAL);
  servoRotacionarGarra.write(SERVO_ROTACIONAR_GARRA_POS_INICIAL);
  servoCancelaEsq.write(SERVO_CANCELA_ESQ_POS_INICIAL);
  servoCancelaDir.write(SERVO_CANCELA_DIR_POS_INICIAL);
}

void loop() {
  descerGarra();
  abrirPas();
  while (true) {
    lerLaserGarra();
    if (distanciaLaserGarra < 50) {
      break;
    }
  }
  fecharPas();
  subirGarraVerificaVitima();

  if (btnVitima) {
    rotacionarGarraDir();
  } else {
    rotacionarGarraEsq();
  }

  abrirPas();
  delay(400);
  fecharPas();
  rotacionarGarraMeio();
  delay(400);

  // if (btnVitima) {
  //   abrirCancelaDir();
  //   delay(500);
  //   fecharCancelaDir();
  // } else {
  //   abrirCancelaEsq();
  //   delay(500);
  //   fecharCancelaEsq();
  // }
  // delay(500);
}

void lerLaserGarra() {
  distanciaLaserGarra = laserGarra.readRangeSingleMillimeters();
}

void lerBtnVitima() {
  btnVitima = !digitalRead(BTN_VITIMA_PIN);
}

void movimentarServo(Servo *servo, uint8_t posicaoFinal, uint8_t velocidade) {
  uint8_t posicaoAtual = servo->read();
  uint8_t passo = posicaoAtual > posicaoFinal ? -1 : 1;

  while (posicaoAtual != posicaoFinal) {
    posicaoAtual += passo;
    servo->write(posicaoAtual);
    delay(velocidade);
  }
}

void fecharPas() {
  movimentarServo(&servoPaGarra, 80, 3);
}

void abrirPas() {
  movimentarServo(&servoPaGarra, 0, 3);
}

void subirGarra() {
  movimentarServo(&servoSubirGarra, 160, 10);
}

void subirGarraVerificaVitima() {
  vitimaViva = false;
  uint8_t posicaoAtual = servoSubirGarra.read();
  while (posicaoAtual < 160) {
    posicaoAtual++;
    servoSubirGarra.write(posicaoAtual);
    delay(10);
    lerBtnVitima();
    if (btnVitima) {
      vitimaViva = true;
    }
  }
}
  
void descerGarra() {
  movimentarServo(&servoSubirGarra, 0, 10);
}

void rotacionarGarraDir() {
  movimentarServo(&servoRotacionarGarra, 90, 10);
}

void rotacionarGarraEsq() {
  movimentarServo(&servoRotacionarGarra, 20, 10);
}

void rotacionarGarraMeio() {
  movimentarServo(&servoRotacionarGarra, 55, 10);
}

void abrirCancelaEsq() {
  movimentarServo(&servoCancelaEsq, 95, 4);
}

void fecharCancelaEsq() {
  movimentarServo(&servoCancelaEsq, 3, 4);
}

void abrirCancelaDir() {
  movimentarServo(&servoCancelaDir, 0, 4);
}

void fecharCancelaDir() {
  movimentarServo(&servoCancelaDir, 99, 4);
}