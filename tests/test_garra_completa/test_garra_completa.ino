#include <Servo.h>

#define SERVO_PA_GARRA_PIN 44
#define SERVO_SUBIR_GARRA_PIN 46
#define SERVO_ROTACIONAR_GARRA_PIN 45
#define SERVO_CANCELA_ESQ_PIN 13
#define SERVO_CANCELA_DIR_PIN 12

Servo servoPaGarra;          // 180 fechada, 60 aberta
Servo servoSubirGarra;       // 0 baixo, 160 cima
Servo servoRotacionarGarra;  // 0 esq, 110 dir 55 padrao
Servo servoCancelaEsq;       // 0 fechado, 90 aberto
Servo servoCancelaDir;       // 90 fechado, 0 aberto

uint8_t posicaoServoPaGarra = 180, posicaoServoSubirGarra = 160, posicaoServoRotacionarGarra = 55;
uint8_t posicaoServoCancelaEsq = 0, posicaoServoCancelaDir = 90;

void setup() {
  Serial.begin(9600);
  servoPaGarra.attach(SERVO_PA_GARRA_PIN, 700, 2000);  // testar isso amanha
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

  servoPaGarra.write(posicaoServoPaGarra);
  servoSubirGarra.write(posicaoServoSubirGarra);
  servoRotacionarGarra.write(posicaoServoRotacionarGarra);
  servoCancelaEsq.write(posicaoServoCancelaEsq);
  servoCancelaDir.write(posicaoServoCancelaDir);

  Serial.println("fim do setup");
}

void loop() {
  movimentarServo(&servoSubirGarra, 160, 0, 22);

  // movimentarServo(&servoPaGarra, 180, 60, 5);
  movimentarServoPaGarra(posicaoServoPaGarra, 60, 1);

  movimentarServo(&servoSubirGarra, 0, 160, 25);

  movimentarServo(&servoRotacionarGarra, 55, 110, 40); // 55
  movimentarServo(&servoPaGarra, 60, 180, 5);
  movimentarServo(&servoPaGarra, 180, 60, 5);

  movimentarServo(&servoRotacionarGarra, 110, 0, 40);
  movimentarServo(&servoPaGarra, 60, 180, 5);
  movimentarServo(&servoPaGarra, 180, 60, 5);
  movimentarServo(&servoRotacionarGarra, 0, 55, 30);

  movimentarServo(&servoCancelaEsq, 0, 90, 4);
  movimentarServo(&servoCancelaDir, 100, 0, 4);
  delay(1000);
  movimentarServo(&servoCancelaEsq, 90, 0, 4);
  movimentarServo(&servoCancelaDir, 0, 100, 4);
}

void movimentarServoPaGarra(uint8_t posicaoInicial, uint8_t posicaoFinal, uint8_t velocidade) {
  if (posicaoInicial > posicaoFinal) {
    posicaoServoPaGarra = posicaoInicial;
    while (posicaoServoPaGarra > posicaoFinal) {
      posicaoServoPaGarra--;
      servoPaGarra.write(posicaoServoPaGarra);
      delay(velocidade);
    }
  } else {
    posicaoServoPaGarra = posicaoInicial;
    while (posicaoServoPaGarra < posicaoFinal) {
      posicaoServoPaGarra++;
      servoPaGarra.write(posicaoServoPaGarra);
      delay(velocidade);
    }
  }
}

void movimentarServo(Servo *motor, uint8_t posicaoInicial, uint8_t posicaoFinal, uint8_t velocidade) {
  if (posicaoInicial > posicaoFinal) {
    for (int i = posicaoInicial; i >= posicaoFinal; i--) {
      Serial.println(i);
      motor->write(i);
      delay(velocidade);
    }
  } else {
    for (uint8_t i = posicaoInicial; i <= posicaoFinal; i++) {
      Serial.println(i);
      motor->write(i);
      delay(velocidade);
    }
  }
}