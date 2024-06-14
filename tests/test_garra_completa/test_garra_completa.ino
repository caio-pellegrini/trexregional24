#include <Servo.h>

#define SERVO_PA_GARRA_PIN 44
#define SERVO_SUBIR_GARRA_PIN 46
#define SERVO_ROTACIONAR_GARRA_PIN 45
#define SERVO_CANCELA_ESQ_PIN 13
#define SERVO_CANCELA_DIR_PIN 12

Servo servoPaGarra;          // 80 fechada, 0 aberta
Servo servoSubirGarra;       // 160 cima, 0 baixo
Servo servoRotacionarGarra;  // 55 meio, 10 esq, 100 dir
Servo servoCancelaEsq;       // 0 fechado, 90 aberto
Servo servoCancelaDir;       // 95 fechado, 0 aberto

uint8_t posicaoServoPaGarra = 80;
uint8_t posicaoServoSubirGarra = 160;
uint8_t posicaoServoRotacionarGarra = 55;
uint8_t posicaoServoCancelaEsq = 0;
uint8_t posicaoServoCancelaDir = 95;

void setup() {
  // Serial.begin(9600);
  // Não precisamos fazer attach aqui, pois faremos nas funções de movimento
  fecharPas();
  rotacionarGarraMeio();
  subirGarra();
  fecharCancelaEsq();
  fecharCancelaDir();
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
  movimentarServoPaGarra(80, 3);
}

void abrirPas() {
  movimentarServoPaGarra(0, 3);
}

void subirGarra() {
  movimentarServoSubirGarra(160, 15);
}

void descerGarra() {
  movimentarServoSubirGarra(0, 15);
}

void rotacionarGarraDir() {
  movimentarServoRotacionarGarra(100, 10);
}

void rotacionarGarraEsq() {
  movimentarServoRotacionarGarra(10, 10);
}

void rotacionarGarraMeio() {
  movimentarServoRotacionarGarra(55, 10);
}

void abrirCancelaEsq() {
  if (!servoCancelaEsq.attached()) {
    servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  }
  movimentarServoCancelaEsq(90, 4);
}

void fecharCancelaEsq() {
  movimentarServoCancelaEsq(0, 4);
}

void abrirCancelaDir() {
  movimentarServoCancelaDir(0, 4);
}

void fecharCancelaDir() {
  movimentarServoCancelaDir(95, 4);
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

// enquanto a garra sobe já verificar se é viva ou morta durante o while
// é uma boa prática ficar dando attach e detach nos servos após cada movimento?