/*
  Nome do Projeto: Seguidor de Linha
  Descrição: Sketch para controlar um robô seguidor de linha.
  Autor: Seu Nome
  Data: 10/01/2024
  Versão: 1.0
*/

#include "mega_def.h"
#include <QTRSensors.h>
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include "MPU6050_6Axis_MotionApps612.h"
#include "Adafruit_VL53L0X.h"

#define DEBUG 1
#define DEBUG_CALIBRACAO 1
#define DEBUG_EM_CURSO 0 // 1 para o robo ANDAR com o SERIAL LIGADO (não recomendado)
#define DEBUG_QTRA 1
#define DEBUG_REFL_FRENTE 1
#define DEBUG_TCS_VERDE 0
#define DEBUG_TCS_AREA 0
#define DEBUG_ULTRA 0
#define DEBUG_GIROSCOPIO 0
#define DEBUG_LASER_FRENTE 0
#define DEBUG_VISAO_GARRA 0
#define DEBUG_BOTOES 0

#define LUZ 225                // 900 quando range = 0-1023
#define LUZ_FRENTE 30           // 6 no branco e 110 no preto
#define TCS_SATURACAO_MAX 1500 // 4000 PARA 614ms
#define CORTE_VERDE_ESQ 80   // abaixo disso é verde
#define CORTE_VERDE_DIR 70
#define CORTE_VERMELHO_CRUZ 100

#define CONVERT_8B_DEC(vel) ((vel * 255) / 100)
#define VEL_MOTOR_FRENTE  CONVERT_8B_DEC(52)
#define VEL_MOTOR_CURVA   CONVERT_8B_DEC(43)
#define VEL_MOTOR_TRAS    CONVERT_8B_DEC(39)
#define VEL_MOTOR_SEG_MAX CONVERT_8B_DEC(78)
#define VEL_MOTOR_SEG_MIN CONVERT_8B_DEC(71)

#define TEMPO_MOVER_ANTES_CRUZ 350

uint8_t se3, se2, se1, se0, sd0, sd1, sd2, sd3;
uint8_t sf;

Adafruit_TCS34725 tcsFrente = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcsEsq = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);

uint8_t rgbEsq[3]; // lista de valores RGB do sensor TCS esquerdo
uint8_t rgbDir[3]; // lista de valores RGB do sensor TCS direito
uint16_t rgbFrente[3];

// Variáveis e definições para o MPU-6050 com DMP
MPU6050 mpu;
uint8_t mpuIntStatus;
uint16_t fifoCount;
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
bool dmpReady = false;  // set true if DMP init was successful
uint8_t fifoBuffer[64]; // FIFO storage buffer
// orientation/motion vars
Quaternion q;        // [w, x, y, z]         quaternion container
VectorFloat gravity; // [x, y, z]            gravity vector
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float yaw, pitch, roll;
float initialYaw;
volatile bool mpuInterrupt = false;
// mpu antigo
int16_t ax, ay, az;
int16_t gx, gy, gz;
void dmpDataReady()
{
  mpuInterrupt = true;
}

Adafruit_VL53L0X laserFrente = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t medidaLaserFrente;

bool er;

void setup()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    pinMode(rgbD[i], OUTPUT);
    pinMode(rgbE[i], OUTPUT);
  }
  ligarLed(AMBOS, VERMELHO, 0);

  Serial2.begin(9600);
  Serial.begin(9600);

  if (!tcsEsq.begin())
  {
    Serial.println("TCS34725 Esq não encontrado. Verifique as conexões.");
  }

  tcaSelecionar(0);
  Serial.println(tcsFrente.begin() ? "TCS34725 area conectado :)" : "TCS34725 area conexão falhou :(");

  pinMode(MOTOR_DF, OUTPUT);
  pinMode(MOTOR_DT, OUTPUT);
  pinMode(MOTOR_EF, OUTPUT);
  pinMode(MOTOR_ET, OUTPUT);

  // PORTA SENSORES REFLETANCIA
  pinMode(SE2_PIN, INPUT);
  pinMode(SE1_PIN, INPUT);
  pinMode(SD1_PIN, INPUT);
  pinMode(SD2_PIN, INPUT);
  pinMode(SE3_PIN, INPUT);
  pinMode(SE0_PIN, INPUT);
  pinMode(SD3_PIN, INPUT);
  pinMode(SD0_PIN, INPUT);

  // PORTA SENSOR DA FRENTE
  pinMode(SF_PIN, INPUT);

  // EMISSOR RECEPTOR
  pinMode(3, INPUT);

  tcaDesliga();
  tcaSelecionar(7);
  Serial.println(laserFrente.begin() ? "VL53L0X Frente conectado :)" : "VL53L0X Frente conexão falhou :(");
  tcaDesliga();


  ligarGiroscopio();

  desligarLed(AMBOS);
}

void loop()
{
#if DEBUG
#if DEBUG_CALIBRACAO
  calibrar();
#endif
#if defined(DEBUG_EM_CURSO) && (DEBUG_EM_CURSO == 0)
  return;
#endif
#endif
  

  er = digitalRead(3);

  if (er) {
    ligarLed(AMBOS, ROXO, 0);
  } else {
    desligarLed(AMBOS);
  }


  lerQTRATodos();

  lerReflFrente();

  // CRUZAMENTO
  if (se3 >= LUZ && se2 >= LUZ && se1 >= LUZ && se0 >= LUZ && sd0 >= LUZ && sd1 >= LUZ && sd2 >= LUZ && sd3 >= LUZ)
  {
    pararMotor();
    ligarLed(AMBOS, BRANCO, 0);
    moverTrasPorMS(100);
    pararMotor();
    analisarVerde(true, true, true);
    desligarLed(AMBOS);
  }

  // MEIO CRUZAMENTO ESQUERDO
  if (sf >= LUZ_FRENTE && (se3 >= LUZ && se2 >= LUZ && se1 >= LUZ) && (sd1 <= LUZ && sd2 <= LUZ && sd3 <= LUZ))
  {
    pararMotor();
    ligarLed(ESQ, BRANCO, 0);
    moverTrasPorMS(100);
    pararMotor();
    analisarVerde(false, true, false);
    desligarLed(AMBOS);
  }

  // MEIO CRUZAMENTO DIREITO
  if (sf >= LUZ_FRENTE && (se3 <= LUZ && se2 <= LUZ && se1 <= LUZ) && (sd1 >= LUZ && sd2 >= LUZ && sd3 >= LUZ))
  {
    pararMotor();
    ligarLed(DIR, BRANCO, 0);
    moverTrasPorMS(100);
    pararMotor();
    analisarVerde(false, false, true);
    desligarLed(AMBOS);
  }

  lerQTRASegueLinha();

  moverFrente();

  if (se1 >= LUZ || se2 >= LUZ)
  {
    segueLinhaEsquerda();
  }

  if (sd1 >= LUZ || sd2 >= LUZ)
  {
    segueLinhaDireita();
  }
}

void analisarVerde(bool isBeco, bool isVerdeEsquerdo, bool isVerdeDireito)
{
  lerVerde();
  lerVerde();

  desligarLed(AMBOS);

  bool verdeEsq = false;
  bool verdeDir = false;

  if (rgbEsq[1] < CORTE_VERDE_ESQ && rgbEsq[0] < CORTE_VERMELHO_CRUZ)
  {
    ligarLed(ESQ, VERDE, 0);
    verdeEsq = true;
  }

  if (rgbDir[1] != 0)
  {
    if (rgbDir[1] < CORTE_VERDE_DIR && rgbDir[0] < CORTE_VERMELHO_CRUZ)
    {
      ligarLed(DIR, VERDE, 0);
      verdeDir = true;
    }
  }
  else
  {
    ligarLed(DIR, VERMELHO, 0); // avisa que o rgbdir não recebeu dados do TCS
  }

  moverFrentePorMS(TEMPO_MOVER_ANTES_CRUZ); // mover pra frente antes de virar 
  
  // Beco sem saida
  if (isBeco && verdeEsq && verdeDir)
  {
    virarEsquerdaGiro(210);
    moverFrentePorMS(300);
  }

  // Curva à esquerda
  if (isVerdeEsquerdo && verdeEsq && !verdeDir)
  {
    virarEsquerdaGiro(105);
    moverFrentePorMS(200);
  }

  // Curva à direita
  if (isVerdeDireito && !verdeEsq && verdeDir)
  {
    virarDireitaGiro(105);
    moverFrentePorMS(200);
  }

  // Seguir reto
  if (!verdeEsq && !verdeDir)
  {
    moverFrentePorMS(200);
  }

  pararMotor();
}

void tcaSelecionar(uint8_t i) {
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void tcaDesliga() {
  Wire.beginTransmission(TCAADDR);
  Wire.write(0);  // Desligar todos os canais
  Wire.endTransmission();
}