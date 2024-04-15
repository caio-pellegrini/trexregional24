/*
  Nome do Projeto: Seguidor de Linha
  Descrição: Sketch para controlar um robô seguidor de linha.
  Autor: Seu Nome
  Data: 10/01/2024
  Versão: 1.0
*/

#include "mega_def.h"
#include <QTRSensors.h>

#define CALIBRACAO             1

#define CALIBRAR_QTRA          0
#define CALIBRAR_QTRRC         0
#define CALIBRAR_TCS_VERDE     0
#define CALIBRAR_ULTRA_FRENTE  0
#define CALIBRAR_GIROSCOPIO    0
#define CALIBRAR_LASER_FRENTE  0
#define CALIBRAR_VISAO_GARRA   0
#define CALIBRAR_BOTOES        0
#define CALIBRAR_TCS_AREA      0

#define LUZ 900
#define LUZ_F 500

QTRSensors qtrc;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];
uint16_t sFE3, sFE2, sFE1, sFD1, sFD2, sFD3;
uint16_t se3, se2, se1, se0, sd0, sd1, sd2, sd3;

void setup() {
  ligarLed(AMBOS, VERMELHO, 0);
  
  for (byte i = 0; i < 3; i++) {
    pinMode(rgbD[i], OUTPUT);
    pinMode(rgbE[i], OUTPUT);
  }
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
}

void loop() {
  // ligarLed(AMBOS, VERMELHO, 1000);
  #if CALIBRACAO
    calibrar();
    return; // Comente essa linha se quiser que o robo ANDE com o SERIAL LIGADO (não recomendado)
  #endif

  se2 = analogRead(SE2_PIN);
  se1 = analogRead(SE1_PIN);
  sd1 = analogRead(SD1_PIN);
  sd2 = analogRead(SD2_PIN);

  

  if (se1 >= LUZ || se2 >= LUZ) {
    segueLinhaEsquerda();
  }

  if (sd1 >= LUZ || sd2 >= LUZ) {
    segueLinhaDireita();
  }
}