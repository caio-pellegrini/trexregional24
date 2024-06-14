#include <Servo.h>

#define SERVO_PA_GARRA_PIN 44
#define SERVO_SUBIR_GARRA_PIN 46
#define SERVO_ROTACIONAR_GARRA_PIN 45
#define SERVO_CANCELA_ESQ_PIN 13
#define SERVO_CANCELA_DIR_PIN 12

#define SERVO_PA_GARRA_FEC   80
#define SERVO_PA_GARRA_ABE   0

#define SERVO_SUBIR_GARRA_CIM 160
#define SERVO_SUBIR_GARRA_BAI 0

#define SERVO_ROTACIONAR_GARRA_ESQ 10
#define SERVO_ROTACIONAR_GARRA_MEIO 55
#define SERVO_ROTACIONAR_GARRA_DIR 90

#define SERVO_CANCELA_ESQ_FEC 3
#define SERVO_CANCELA_ESQ_ABE 95

#define SERVO_CANCELA_DIR_FEC 99
#define SERVO_CANCELA_DIR_ABE 0

Servo servoPaGarra;
Servo servoSubirGarra;
Servo servoRotacionarGarra;
Servo servoCancelaEsq;
Servo servoCancelaDir;  

uint8_t posicaoServoPaGarra = SERVO_PA_GARRA_FEC;
uint8_t posicaoServoSubirGarra = SERVO_SUBIR_GARRA_CIM;
uint8_t posicaoServoRotacionarGarra = SERVO_ROTACIONAR_GARRA_MEIO;
uint8_t posicaoServoCancelaEsq = SERVO_CANCELA_ESQ_FEC;
uint8_t posicaoServoCancelaDir = SERVO_CANCELA_DIR_FEC;

void setup() {
  Serial.begin(9600);
  servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

  servoPaGarra.write(posicaoServoPaGarra);
  servoSubirGarra.write(posicaoServoSubirGarra);
  servoRotacionarGarra.write(posicaoServoRotacionarGarra);
  servoCancelaEsq.write(posicaoServoCancelaEsq);
  servoCancelaDir.write(posicaoServoCancelaDir);
}

void loop() {
  descerGarra();

  abrirPas();
  delay(1000);
  fecharPas();

  subirGarra();

  rotacionarGarraDir();

  abrirPas();
  delay(1000);
  fecharPas();

  rotacionarGarraEsq();

  abrirPas();
  delay(1000);
  fecharPas();

  rotacionarGarraMeio();

  abrirCancelaEsq();  
  abrirCancelaDir();
  delay(1000);
  fecharCancelaEsq();
  fecharCancelaDir();
}

void movimentarServo(Servo &servo, uint8_t &posicaoAtual, uint8_t posicaoFinal, uint8_t velocidade) {
  if (posicaoAtual > posicaoFinal) {
    while (posicaoAtual > posicaoFinal) {
      posicaoAtual--;
      servo.write(posicaoAtual);
      delay(velocidade);
    }
  } else {
    while (posicaoAtual < posicaoFinal) {
      posicaoAtual++;
      servo.write(posicaoAtual);
      delay(velocidade);
    }
  }
}

void movimentarServoPaGarra(uint8_t posicaoFinal, uint8_t velocidade) {
  if (posicaoServoPaGarra > posicaoFinal) {
    while (posicaoServoPaGarra > posicaoFinal) {
      posicaoServoPaGarra--;
      servoPaGarra.write(posicaoServoPaGarra);
      delay(velocidade);
    }
  } else {
    while (posicaoServoPaGarra < posicaoFinal) {
      posicaoServoPaGarra++;
      servoPaGarra.write(posicaoServoPaGarra);
      delay(velocidade);
    }
  }
}

void movimentarServoRotacionarGarra(uint8_t posicaoFinal, uint8_t velocidade) {
  if (posicaoServoRotacionarGarra > posicaoFinal) {
    while (posicaoServoRotacionarGarra > posicaoFinal) {
      posicaoServoRotacionarGarra--;
      servoRotacionarGarra.write(posicaoServoRotacionarGarra);
      delay(velocidade);
    }
  } else {
    while (posicaoServoRotacionarGarra < posicaoFinal) {
      posicaoServoRotacionarGarra++;
      servoRotacionarGarra.write(posicaoServoRotacionarGarra);
      delay(velocidade);
    }
  }
}

void movimentarServoSubirGarra(uint8_t posicaoFinal, uint8_t velocidade) {
  if (posicaoServoSubirGarra > posicaoFinal) {
    while (posicaoServoSubirGarra > posicaoFinal) {
      posicaoServoSubirGarra--;
      servoSubirGarra.write(posicaoServoSubirGarra);
      delay(velocidade);
    }
  } else {
    while (posicaoServoSubirGarra < posicaoFinal) {
      posicaoServoSubirGarra++;
      servoSubirGarra.write(posicaoServoSubirGarra);
      delay(velocidade);
    }
  }
}

void movimentarServoCancelaEsq(uint8_t posicaoFinal, uint8_t velocidade) {
  if (posicaoServoCancelaEsq > posicaoFinal) {
    while (posicaoServoCancelaEsq > posicaoFinal) {
      posicaoServoCancelaEsq--;
      servoCancelaEsq.write(posicaoServoCancelaEsq);
      delay(velocidade);
    }
  } else {
    while (posicaoServoCancelaEsq < posicaoFinal) {
      posicaoServoCancelaEsq++;
      servoCancelaEsq.write(posicaoServoCancelaEsq);
      delay(velocidade);
    }
  }
}

void movimentarServoCancelaDir(uint8_t posicaoFinal, uint8_t velocidade) {
  if (posicaoServoCancelaDir > posicaoFinal) {
    while (posicaoServoCancelaDir > posicaoFinal) {
      posicaoServoCancelaDir--;
      servoCancelaDir.write(posicaoServoCancelaDir);
      delay(velocidade);
    }
  } else {
    while (posicaoServoCancelaDir < posicaoFinal) {
      posicaoServoCancelaDir++;
      servoCancelaDir.write(posicaoServoCancelaDir);
      delay(velocidade);
    }
  }
}

void fecharPas() {
  movimentarServoPaGarra(SERVO_PA_GARRA_FEC, 3);
}

void abrirPas() {
  movimentarServoPaGarra(SERVO_PA_GARRA_ABE, 3);
}

void subirGarra() {
  movimentarServoSubirGarra(SERVO_SUBIR_GARRA_CIM, 15);
}

void descerGarra() {
  movimentarServoSubirGarra(SERVO_SUBIR_GARRA_BAI, 15);
}

void rotacionarGarraDir() {
  movimentarServoRotacionarGarra(SERVO_ROTACIONAR_GARRA_DIR, 10);
}

void rotacionarGarraEsq() {
  movimentarServoRotacionarGarra(SERVO_ROTACIONAR_GARRA_ESQ, 10);
}

void rotacionarGarraMeio() {
  movimentarServoRotacionarGarra(SERVO_ROTACIONAR_GARRA_MEIO, 10);
}

void abrirCancelaEsq() {
  movimentarServoCancelaEsq(SERVO_CANCELA_ESQ_ABE, 4);
}

void fecharCancelaEsq() {
  movimentarServoCancelaEsq(SERVO_CANCELA_ESQ_FEC, 4);
}

void abrirCancelaDir() {
  movimentarServoCancelaDir(SERVO_CANCELA_DIR_ABE, 4);
}

void fecharCancelaDir() {
  movimentarServoCancelaDir(SERVO_CANCELA_DIR_FEC, 4);
}

// enquanto a garra sobe já verificar se é viva ou morta durante o while
// é uma boa prática ficar dando attach e detach nos servos após cada movimento? nao, nao é.