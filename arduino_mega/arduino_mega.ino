/*
  Nome do Projeto: Seguidor de Linha
  Descrição: Sketch para controlar um robô seguidor de linha.
  Autor: Seu Nome
  Data: 10/01/2024
  Versão: 1.0
*/

#include "mega_def.h"
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <VL53L0X.h>
#include <Ultrasonic.h>
#include "MPU6050_6Axis_MotionApps612.h"


#define DEBUG 0
#define DEBUG_CALIBRACAO 1
#define DEBUG_EM_CURSO 0 // 1 para o robo ANDAR com o SERIAL LIGADO (não recomendado)
#define DEBUG_QTRA 1
#define DEBUG_REFL_FRENTE 1
#define DEBUG_TCS_VERDE 0
#define DEBUG_TCS_AREA 0
#define DEBUG_ULTRA 0
#define DEBUG_GIROSCOPIO 1
#define DEBUG_LASER_FRENTE 1
#define DEBUG_VISAO_GARRA 0
#define DEBUG_BOTOES 0

#define LUZ 210              // 900 quando range = 0-1023
#define LUZ_FRENTE 50           // 6 no branco e 110 no preto
#define TCS_SATURACAO_MAX 1500 // 4000 PARA 614ms
#define CORTE_VERDE_ESQ 80   // abaixo disso é verde
#define CORTE_VERDE_DIR 70
#define CORTE_VERMELHO_CRUZ 100

#define CONVERT_8B_DEC(vel) ((vel * 255) / 100)
#define VEL_MOTOR_FRENTE     CONVERT_8B_DEC(50)
#define VEL_MOTOR_CURVA      CONVERT_8B_DEC(43)
#define VEL_MOTOR_TRAS       CONVERT_8B_DEC(40)
#define VEL_MOTOR_SEG_FRENTE CONVERT_8B_DEC(40)
#define VEL_MOTOR_SEG_MAX    CONVERT_8B_DEC(78)
#define VEL_MOTOR_SEG_MIN    CONVERT_8B_DEC(75)

#define TEMPO_MOVER_ANTES_CRUZ 350

uint8_t se3, se2, se1, se0, sd0, sd1, sd2, sd3;
uint8_t sf;

uint8_t canalTcsFrente = 7;
uint8_t canalTcsEsq = 6;



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

bool er;

Adafruit_TCS34725 tcsFrente = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcsEsq = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);

VL53L0X laserFrente;
uint16_t distanciaFrente;

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
  Serial.println();
  Wire.begin();


  // Configura o sensor VL53L0X
  laserFrente.setTimeout(500);
  Serial.println(laserFrente.init() ? "Laser Frente conectado :)" : "Laser Frente conexão falhou :(");
  // while (1) {}
  
  // laserFrente.setAddress(0x31); // Define o endereço I2C do sensor
  delay(10);
  // laserFrente.startContinuous(); // Inicia leituras contínuas

  i2c_scanner();

  tcaSelecionar(canalTcsEsq);
  Serial.println(tcsEsq.begin() ? "TCS34725 Esq conectado :)" : "TCS34725 Esq conexão falhou :(");
  tcaDesliga();

  tcaSelecionar(canalTcsFrente);
  Serial.println(tcsFrente.begin() ? "TCS34725 area conectado :)" : "TCS34725 area conexão falhou :(");
  tcaDesliga();

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

  ligarGiroscopio();

  desligarLed(AMBOS);

  #if defined(DEBUG) && (DEBUG == 0)
    Serial.print("Desligando Serial");
    Serial.end();
  #endif
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
  

  // er = digitalRead(3);

  // if (er) {
  //   ligarLed(AMBOS, ROXO, 0);
  // } else {
  //   desligarLed(AMBOS);
  // }

  lerLaserFrente();

  if (distanciaFrente <= 60) {
    desviarObstaculo();
  }

  lerQTRATodos();
  lerReflFrente();

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

  lerQTRASegueLinha();

  moverFrenteSeguidor();

  if (se1 >= LUZ || se2 >= LUZ)
  {
    segueLinhaEsquerda();
  }

  if (sd1 >= LUZ || sd2 >= LUZ)
  {
    segueLinhaDireita();
  }
}