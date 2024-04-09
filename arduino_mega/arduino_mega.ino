/*
  Nome do Projeto: Seguidor de Linha
  Descrição: Sketch para controlar um robô seguidor de linha.
  Autor: Seu Nome
  Data: 10/01/2024
  Versão: 1.0
*/

#include "mega_pins.h"
#include <QTRSensors.h>

#define LUZ 900
#define LUZ_F 500

QTRSensors qtrc;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];
uint16_t sFE3, sFE2, sFE1, sFD1, sFD2, sFD3;
uint16_t sE3, sE2, sE1, sE0, sD0, sD1, sD2, sD3;

void setup() {
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
  // ligarLed(AMBOS, DESLIGADO, 1000);
  // ligarLed(AMBOS, VERDE, 1000);
  // ligarLed(AMBOS, DESLIGADO, 1000);
  // ligarLed(AMBOS, AZUL, 1000);
  // ligarLed(AMBOS, DESLIGADO, 1000);

  sE2 = analogRead(SE2_PIN);
  sE1 = analogRead(SE1_PIN);
  sD1 = analogRead(SD1_PIN);
  sD2 = analogRead(SD2_PIN);

  

  if (sE1 >= LUZ || sE2 >= LUZ) {
    segueLinhaEsquerda();
  }

  if (sD1 >= LUZ || sD2 >= LUZ) {
    segueLinhaDireita();
  }
}