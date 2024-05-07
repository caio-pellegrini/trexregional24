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


#define DEBUG 0
#define DEBUG_CALIBRACAO 1
#define DEBUG_EM_CURSO 0 // 1 para o robo ANDAR com o SERIAL LIGADO (não recomendado)
#define DEBUG_QTRA 0
#define DEBUG_QTRRC 0
#define DEBUG_TCS_VERDE 0
#define DEBUG_TCS_AREA 0
#define DEBUG_ULTRA 0
#define DEBUG_GIROSCOPIO 1
#define DEBUG_LASER_FRENTE 0
#define DEBUG_VISAO_GARRA 0
#define DEBUG_BOTOES 0

#define LUZ 225 // 900 quando range = 0-1023
#define LUZ_F 40 // 30 no branco e 90 no preto
#define TCS_SATURACAO_MAX 1500 // 4000 PARA 614ms
#define VEL_MOTOR_FRENTE 135
#define VEL_MOTOR_SEG_MAX 210
#define VEL_MOTOR_SEG_MIN 193
#define CORTE_VERDE_ESQ   80     // abaixo disso é verde
#define CORTE_VERDE_DIR   70
#define CORTE_VERMELHO_CRUZ 100

QTRSensors qtrc;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];
byte sfe3, sfe2, sfe1, sfd1, sfd2, sfd3;
byte se3, se2, se1, se0, sd0, sd1, sd2, sd3;

Ultrasonic ultrasonicEsq(7, 6);
Ultrasonic ultrasonicDir(5, 4);
int ultraE, ultraD;

// Adafruit_TCS34725 tcsFrente = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcsEsq = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);

int rgbEsq[3]; // pode ser trocado para byte posteriormente
int rgbDir[3]; // pode ser trocado para byte posteriormente

// Variáveis e definições para o MPU-6050 com DMP
MPU6050 mpu;
uint8_t mpuIntStatus;
uint16_t fifoCount;
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
bool dmpReady = false;  // set true if DMP init was successful
uint8_t fifoBuffer[64]; // FIFO storage buffer
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float yaw, pitch, roll;
float initialYaw;
volatile bool mpuInterrupt = false;
// mpu antigo
int16_t ax, ay, az;
int16_t gx, gy, gz;
void dmpDataReady() {
  mpuInterrupt = true;
}

void setup()
{
  for (byte i = 0; i < 3; i++)
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

  qtrc.setTypeRC();
  qtrc.setSensorPins((const uint8_t[]){32, 34, 36, 38, 40, 42}, SensorCount);

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

  se3 = analogRead(SE3_PIN) >> 2; // >> 2 transforma o valor de 10-bits (0-1023) para 8-bits (0-255)
  se2 = analogRead(SE2_PIN) >> 2;
  se1 = analogRead(SE1_PIN) >> 2;
  se0 = analogRead(SE0_PIN) >> 2;
  sd0 = analogRead(SD0_PIN) >> 2;
  sd1 = analogRead(SD1_PIN) >> 2;
  sd2 = analogRead(SD2_PIN) >> 2;
  sd3 = analogRead(SD3_PIN) >> 2;

  qtrc.read(sensorValues);
  sfe1 = map(sensorValues[3], 0, 2500, 0, 255);

  // CRUZAMENTO
  if (se3 >= LUZ && se2 >= LUZ && se1 >= LUZ && se0 >= LUZ && sd0 >= LUZ && sd1 >= LUZ && sd2 >= LUZ && sd3 >= LUZ)
  {
    pararMotor();
    moverTrasPorMS(100);
    pararMotor();

    lerVerde();
    lerVerde();

    bool verdeEsq = false;
    bool verdeDir = false;

    if (rgbEsq[1] < CORTE_VERDE_ESQ && rgbEsq[0] < CORTE_VERMELHO_CRUZ)
    {
      ligarLed(ESQ, VERDE, 0);
      verdeEsq = true;
    }
    else
    {
      ligarLed(ESQ, BRANCO, 0);
    }


    if (rgbDir[1] != 0)
    {
      if (rgbDir[1] < CORTE_VERDE_DIR && rgbDir[0] < CORTE_VERMELHO_CRUZ)
      {
        ligarLed(DIR, VERDE, 0);
        verdeDir = true;
      }
      else
      {
        ligarLed(DIR, BRANCO, 0);
      }
    }
    else
    {
      ligarLed(DIR, VERMELHO, 0);
    }

    moverFrentePorMS(400);

    if (verdeEsq && verdeDir)
    {
      // beco sem saida
      virarEsquerdaPorMS(2200);
    }
    else if (verdeEsq && !verdeDir)
    {
      // curva a esquerda
      virarEsquerdaGiro(105);
      moverFrentePorMS(300);
    }
    else if (!verdeEsq && verdeDir)
    {
      // curva a direita
      virarDireitaGiro(105);
      moverFrentePorMS(300);
    }
    else
    {
      // seguir reto
      moverFrentePorMS(200);
    }

    pararMotor();
    delay(3000);

    desligarLed(AMBOS);
  }

  // MEIO CRUZAMENTO ESQUERDO
  if (sfe1 >= LUZ_F && (se3 >= LUZ && se2 >= LUZ && se1 >= LUZ) && (sd1 <= LUZ && sd2 <= LUZ && sd3 <= LUZ)) {
    pararMotor();
    moverTrasPorMS(100);
    pararMotor();

    lerVerde();
    lerVerde();

    bool verdeEsq = false;
    bool verdeDir = false;

    if (rgbEsq[1] < CORTE_VERDE_ESQ && rgbEsq[0] < CORTE_VERMELHO_CRUZ)
    {
      ligarLed(ESQ, VERDE, 0);
      verdeEsq = true;
    }
    else
    {
      ligarLed(ESQ, BRANCO, 0);
    }


    if (rgbDir[1] != 0)
    {
      if (rgbDir[1] < CORTE_VERDE_DIR && rgbDir[0] < CORTE_VERMELHO_CRUZ)
      {
        ligarLed(DIR, VERDE, 0);
        verdeDir = true;
      }
      else
      {
        ligarLed(DIR, BRANCO, 0);
      }
    }
    else
    {
      ligarLed(DIR, VERMELHO, 0);
    }

    moverFrentePorMS(400);

    if (verdeEsq && verdeDir)
    {
      // beco sem saida
      virarEsquerdaPorMS(2200);
    }
    else if (verdeEsq && !verdeDir)
    {
      // curva a esquerda
      virarEsquerdaPorMS(1100);
    }
    else if (!verdeEsq && verdeDir)
    {
      // curva a direita
      virarDireitaPorMS(1100);
    }
    else
    {
      // seguir reto
      moverFrentePorMS(200);
    }

    pararMotor();

    desligarLed(AMBOS);
  }

  // MEIO CRUZAMENTO DIREITO
  if (sfe1 >= LUZ_F && (se3 <= LUZ && se2 <= LUZ && se1 <= LUZ) && (sd1 >= LUZ && sd2 >= LUZ && sd3 >= LUZ)) {
    pararMotor();
    moverTrasPorMS(100);
    pararMotor();

    lerVerde();
    lerVerde();

    bool verdeEsq = false;
    bool verdeDir = false;

    if (rgbEsq[1] < CORTE_VERDE_ESQ && rgbEsq[0] < CORTE_VERMELHO_CRUZ)
    {
      ligarLed(ESQ, VERDE, 0);
      verdeEsq = true;
    }
    else
    {
      ligarLed(ESQ, BRANCO, 0);
    }


    if (rgbDir[1] != 0)
    {
      if (rgbDir[1] < CORTE_VERDE_DIR && rgbDir[0] < CORTE_VERMELHO_CRUZ)
      {
        ligarLed(DIR, VERDE, 0);
        verdeDir = true;
      }
      else
      {
        ligarLed(DIR, BRANCO, 0);
      }
    }
    else
    {
      ligarLed(DIR, VERMELHO, 0);
    }

    moverFrentePorMS(400);

    if (verdeEsq && verdeDir)
    {
      // beco sem saida
      virarEsquerdaPorMS(2200);
    }
    else if (verdeEsq && !verdeDir)
    {
      // curva a esquerda
      virarEsquerdaPorMS(1100);
    }
    else if (!verdeEsq && verdeDir)
    {
      // curva a direita
      virarDireitaPorMS(1100);
    }
    else
    {
      // seguir reto
      moverFrentePorMS(200);
    }

    pararMotor();

    desligarLed(AMBOS);
  }

  se2 = analogRead(SE2_PIN) >> 2;
  se1 = analogRead(SE1_PIN) >> 2;
  sd1 = analogRead(SD1_PIN) >> 2;
  sd2 = analogRead(SD2_PIN) >> 2;

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

void lerDadosSensorRemoto(int *rgbValues)
{
  static char buffer[64] = {0};
  static int index = 0;
  bool dadosRecebidos = false;

  // Variáveis para armazenar os valores RGB extraídos
  uint16_t r, g, b;

  // Define um tempo limite para a recepção
  unsigned long startTime = millis();

  while (millis() - startTime < 1000)
  { // Aguarda até 1 segundo por uma resposta
    if (Serial2.available() > 0)
    {
      char received = Serial2.read();
      if (received == '\n')
      {
        buffer[index] = '\0'; // Termina a string se for o final da mensagem
        dadosRecebidos = true;
        break; // Sai do loop após processar a mensagem
      }
      else if (index < 63)
      {
        buffer[index++] = received;
      }
    }
  }

  if (dadosRecebidos)
  {
    // Serial.print(buffer);

    // Tenta extrair os valores R, G, B da string recebida
    if (sscanf(buffer, "R:%d,G:%d,B:%d", &r, &g, &b) == 3)
    { // Se três valores forem lidos com sucesso
      rgbValues[0] = r;
      rgbValues[1] = g;
      rgbValues[2] = b;
    }
    else
    {
      // Serial.print(" Formato de dados inválido.");
    }
  }
  else
  {
    Serial.print("Timeout");
    rgbValues[0] = 0;
    rgbValues[1] = 0;
    rgbValues[2] = 0;
  }

  // Limpa o buffer e reseta o índice após processar a mensagem
  memset(buffer, 0, sizeof(buffer));
  index = 0;
}

void lerSensorCor(Adafruit_TCS34725 *tcs, int *rgbValues)
{
  uint16_t r, g, b, c;

  tcs->getRawData(&r, &g, &b, &c);

  rgbValues[0] = map(r, 0, TCS_SATURACAO_MAX, 0, 255);
  rgbValues[1] = map(g, 0, TCS_SATURACAO_MAX, 0, 255);
  rgbValues[2] = map(b, 0, TCS_SATURACAO_MAX, 0, 255);
  // rgbValues[0] = r;
  // rgbValues[1] = g;
  // rgbValues[2] = b;
  
}

// void tcaSelecionar(uint8_t i) {
//   Wire.beginTransmission(TCAADDR);
//   Wire.write(1 << i);
//   Wire.endTransmission();
// }

// void tcaDesliga() {
//   Wire.beginTransmission(TCAADDR);
//   Wire.write(0);  // Desligar todos os canais
//   Wire.endTransmission();
// }
