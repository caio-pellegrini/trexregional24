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

#define SERVO_PA_GARRA_FEC 80
#define SERVO_PA_GARRA_ABE 0

#define SERVO_SUBIR_GARRA_CIM 160
#define SERVO_SUBIR_GARRA_BAI 0

#define SERVO_ROTACIONAR_GARRA_ESQ 20
#define SERVO_ROTACIONAR_GARRA_MEIO 55
#define SERVO_ROTACIONAR_GARRA_DIR 90

#define SERVO_CANCELA_ESQ_FEC 3
#define SERVO_CANCELA_ESQ_ABE 95

#define SERVO_CANCELA_DIR_FEC 99
#define SERVO_CANCELA_DIR_ABE 0

uint8_t posicaoServoPaGarra = SERVO_PA_GARRA_FEC;
uint8_t posicaoServoSubirGarra = SERVO_SUBIR_GARRA_CIM;
uint8_t posicaoServoRotacionarGarra = SERVO_ROTACIONAR_GARRA_MEIO;
uint8_t posicaoServoCancelaEsq = SERVO_CANCELA_ESQ_FEC;
uint8_t posicaoServoCancelaDir = SERVO_CANCELA_DIR_FEC;

// Declare dois objetos sensor
VL53L0X laserFrente;
VL53L0X laserGarra;
int distanciaLaserGarra;

// Define os pinos XSHUT para cada sensor
#define LASER_FRENTE_XSHUT 14
#define LASER_GARRA_XSHUT 19

#define BTN_VITIMA_PIN      33

bool vitimaViva, btnVitima;

void setup()
{
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

    servoPaGarra.write(posicaoServoPaGarra);
    servoSubirGarra.write(posicaoServoSubirGarra);
    servoRotacionarGarra.write(posicaoServoRotacionarGarra);
    servoCancelaEsq.write(posicaoServoCancelaEsq);
    servoCancelaDir.write(posicaoServoCancelaDir);
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
    subirGarra();

    if (btnVitima) {
      rotacionarGarraDir();
    } else {
      rotacionarGarraEsq();
    }

    abrirPas();
    delay(750);
    fecharPas();
    rotacionarGarraMeio();
    delay(500);

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

void lerLaserGarra()
{
  distanciaLaserGarra = laserGarra.readRangeSingleMillimeters();
}

void lerBtnVitima()
{
  btnVitima = !digitalRead(BTN_VITIMA_PIN);
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
    // aqui a garra está subindo
    vitimaViva = false;
    while (posicaoServoSubirGarra < posicaoFinal) {
      posicaoServoSubirGarra++;
      servoSubirGarra.write(posicaoServoSubirGarra);
      delay(velocidade);
      lerBtnVitima();
      if (btnVitima) {
        vitimaViva = true;
      }
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
  movimentarServoSubirGarra(SERVO_SUBIR_GARRA_CIM, 10);
}

void descerGarra() {
  movimentarServoSubirGarra(SERVO_SUBIR_GARRA_BAI, 10);
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