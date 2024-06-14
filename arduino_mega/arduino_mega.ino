/*
  Nome do Projeto: Seguidor de Linha
  Descrição: Sketch para controlar um robô seguidor de linha.
  Autor: Caio P.
  Data: 10/01/2024
  Versão: 1.0
*/

#include "mega_def.h"

#define DEBUG 0

#define DEBUG_CALIBRACAO 1
#define DEBUG_EM_CURSO 0 // 1 para o robo ANDAR com o SERIAL LIGADO (não recomendado)
#define DEBUG_QTRA 1
#define DEBUG_REFL_FRENTE 1
#define DEBUG_TCS_VERDE 1
#define DEBUG_TCS_FRENTE 0
#define DEBUG_GIROSCOPIO 0
#define DEBUG_LASER_FRENTE 0
#define DEBUG_LASER_GARRA 0
#define DEBUG_ULTRA 0
#define DEBUG_BOTOES 0

#define CORTE_QTR 202              
#define CORTE_FRENTE 55           // 6 no branco e 110 no preto
#define TCS_SATURACAO_MAX 1500 // 4000 PARA 614ms
#define CORTE_VERDE_ESQ 90   // abaixo disso é verde
#define CORTE_VERDE_DIR 70
#define CORTE_VERMELHO_CRUZ 100

#define VEL_MOTOR_FRENTE     CONVERT_8B_DEC(50)
#define VEL_MOTOR_TRAS       CONVERT_8B_DEC(40)
#define VEL_MOTOR_CURVA      CONVERT_8B_DEC(55)

// #define VEL_MOTOR_SEG_FRENTE CONVERT_8B_DEC(65) // 38
// #define VEL_MOTOR_SEG_MAX    CONVERT_8B_DEC(90) // 76
// #define VEL_MOTOR_SEG_MIN    CONVERT_8B_DEC(88)  // 73
#define VEL_MOTOR_SEG_FRENTE CONVERT_8B_DEC(38)
#define VEL_MOTOR_SEG_MAX    CONVERT_8B_DEC(76)
#define VEL_MOTOR_SEG_MIN    CONVERT_8B_DEC(73)

#define TEMPO_MOVER_ANTES_CRUZ 370
#define TEMPO_MOVER_ANTES_ANALISAR_VERDE 150

uint8_t se3, se2, se1, se0, sd0, sd1, sd2, sd3;
uint8_t sf;

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

Adafruit_TCS34725 tcsFrente = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcsEsq = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);

VL53L0X laserFrente;
uint16_t distanciaLaserFrente;

VL53L0X laserGarra;
uint16_t distanciaLaserGarra;

// Servo servoPaGarra;
// Servo servoSubirGarra;
// Servo servoRotacionarGarra;
// Servo servoCancelaDir;
// Servo servoCancelaEsqu;

// a posição inicial dos servos é armazenada nessas variáveis
uint8_t posicaoServoPaGarra = 90, posicaoServoSubirGarra = 0, posicaoServoRotacionarGarra = 0;
uint8_t posicaoServoCancelaDir = 0, posicaoServoCancelaEsq = 0;

Ultrasonic ultrasonicEsq(7, 6);
Ultrasonic ultrasonicDir(5, 4);
int ultraEsq, ultraDir;

uint8_t contadorGap = 0;

void setup()
{
  // DEFINIÇÕES DE PINOS DOS LEDS
  pinMode(LED_ESQ_RED_PIN, OUTPUT);
  pinMode(LED_ESQ_GREEN_PIN, OUTPUT);
  pinMode(LED_ESQ_BLUE_PIN, OUTPUT);
  pinMode(LED_DIR_RED_PIN, OUTPUT);
  pinMode(LED_DIR_GREEN_PIN, OUTPUT);
  pinMode(LED_DIR_BLUE_PIN, OUTPUT);

  ligarLed(AMBOS, VERMELHO);

  Serial2.begin(9600);
  Serial.begin(9600);
  Serial.println();
  Wire.begin();

  // Configura o sensor VL53L0X
  laserFrente.setTimeout(500); // padrão 500
  Serial.println(laserFrente.init() ? "Laser Frente conectado :)" : "Laser Frente falhou :(");
  // while (1) {}
  // laserFrente.setAddress(0x31); // Define o endereço I2C do sensor
  // delay(10);
  // laserFrente.startContinuous(); // Inicia leituras contínuas

  // i2c_scanner();

  tcaSelecionar(CANAL_LASER_GARRA);
  laserGarra.setTimeout(500);
  Serial.println(laserGarra.init() ? "Laser Garra conectado :)" : "Laser Garra falhou :(");
  laserGarra.setMeasurementTimingBudget(200000); // -> alta precisão
  tcaDesligar();

  tcaSelecionar(CANAL_TCS_ESQ);
  Serial.println(tcsEsq.begin() ? "TCS Esq conectado :)" : "TCS Esq falhou :(");
  tcaDesligar();

  tcaSelecionar(CANAL_TCS_FRENTE);
  Serial.println(tcsFrente.begin() ? "TCS Frente conectado :)" : "TCS Frente falhou :(");
  tcaDesligar();  

  // MOTORES
  pinMode(MOTOR_ESQ_F_PIN, OUTPUT);
  pinMode(MOTOR_ESQ_T_PIN, OUTPUT);
  pinMode(MOTOR_DIR_F_PIN, OUTPUT);
  pinMode(MOTOR_DIR_T_PIN, OUTPUT);

  // PORTA SENSORES REFLETANCIA
  pinMode(SE3_PIN, INPUT);
  pinMode(SE2_PIN, INPUT);
  pinMode(SE1_PIN, INPUT);
  pinMode(SE0_PIN, INPUT);
  pinMode(SD0_PIN, INPUT);
  pinMode(SD1_PIN, INPUT);
  pinMode(SD2_PIN, INPUT);
  pinMode(SD3_PIN, INPUT);

  // PORTA SENSOR DA FRENTE
  pinMode(SF_PIN, INPUT);

  // PORTAS DOS BOTOES
  pinMode(BTN_AREA_ESQ_PIN, INPUT_PULLUP);
  pinMode(BTN_AREA_DIR_PIN, INPUT_PULLUP);
  pinMode(BTN_PAREDE_ESQ_PIN, INPUT_PULLUP);
  pinMode(BTN_PAREDE_DIR_PIN, INPUT_PULLUP);
  pinMode(BTN_VITIMA_PIN, INPUT_PULLUP);

  ligarGiroscopio();

  // SERVOS
  // servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  // servoPaGarra.write(posicaoServoPaGarra);
  
  // servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  // servoSubirGarra.write(posicaoServoSubirGarra);

  // servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  // servoRotacionarGarra.write(posicaoServoRotacionarGarra);

  // servoCancelaDir.attach(SERVO_CANCELA_DIREITO_PIN);
  // servoCancelaDir.write(posicaoServoCancelaDir);

  // servoCancelaEsq.attach(SERVO_CANCELA_ESQUERDO_PIN);
  // servoCancelaEsq.write(posicaoServoCancelaEsq);

  #if defined(DEBUG) && (DEBUG == 0)
    Serial.print("Desligando Serial");
    Serial.end();
  #endif

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

  lerLaserFrente();
  // if (!laserFrente.timeoutOccurred())
  // {
  //   if (distanciaLaserFrente <= 60)
  //   {
  //     desviarObstaculo();
  //   }
  // }
  
  lerQTRATodos();
  lerReflFrente();

  // TUDO BRANCO
  // if (sf <= CORTE_FRENTE && se3 <= CORTE_QTR && se2 <= CORTE_QTR && se1 <= CORTE_QTR && se0 <= CORTE_QTR && sd0 <= CORTE_QTR && sd1 <= CORTE_QTR && sd2 <= CORTE_QTR && sd3 <= CORTE_QTR)
  // {
  //   moverFrentePorMS(1);
  //   lerQTRATodos();
  //   lerReflFrente();
  //   if (sf <= CORTE_FRENTE && se3 <= CORTE_QTR && se2 <= CORTE_QTR && se1 <= CORTE_QTR && se0 <= CORTE_QTR && sd0 <= CORTE_QTR && sd1 <= CORTE_QTR && sd2 <= CORTE_QTR && sd3 <= CORTE_QTR)
  //   { 
  //   verificarGap();
  //   }
  // } 

  if (sf <= 20 && se3 <= 155 && se2 <= 155 && se1 <= 155 && se0 <= 155 && sd0 <= 155 && sd1 <= 155 && sd2 <= 155 && sd3 <= 155)
  {
    verificarGap();
  }

  // CRUZAMENTO
  if (se3 >= CORTE_QTR && se2 >= CORTE_QTR && se1 >= CORTE_QTR && se0 >= CORTE_QTR && sd0 >= CORTE_QTR && sd1 >= CORTE_QTR && sd2 >= CORTE_QTR && sd3 >= CORTE_QTR)
  {
    ligarLed(AMBOS, BRANCO);
    moverTrasPorMS(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotor();
    analisarVerde(true, true, true);
    desligarLed(AMBOS);
  }

  // MEIO CRUZAMENTO ESQUERDO
  if (sf >= CORTE_FRENTE && (se3 >= CORTE_QTR && se2 >= CORTE_QTR && se1 >= CORTE_QTR) && (sd1 <= CORTE_QTR && sd2 <= CORTE_QTR && sd3 <= CORTE_QTR))
  {
    ligarLed(ESQ, BRANCO);
    moverTrasPorMS(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotor();
    analisarVerde(false, true, false);
    desligarLed(AMBOS);
  }

  // MEIO CRUZAMENTO DIREITO
  if (sf >= CORTE_FRENTE && (se3 <= CORTE_QTR && se2 <= CORTE_QTR && se1 <= CORTE_QTR) && (sd1 >= CORTE_QTR && sd2 >= CORTE_QTR && sd3 >= CORTE_QTR))
  {
    ligarLed(DIR, BRANCO);
    moverTrasPorMS(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotor();
    analisarVerde(false, false, true);
    desligarLed(AMBOS);
  }

  // 90 GRAUS DIREITO
  if (sf <= CORTE_FRENTE && (se3 <= CORTE_QTR && se2 <= CORTE_QTR && se1 <= CORTE_QTR) && (sd1 >= CORTE_QTR && sd2 >= CORTE_QTR && sd3 >= CORTE_QTR))
  {
    ligarLed(DIR, BRANCO);
    seguirLinhaDireita();
    seguirLinhaDireita();
    seguirLinhaDireita();
    desligarLed(AMBOS);
  }

  // 90 GRAUS ESQUERDO
  if (sf <= CORTE_FRENTE && (se3 >= CORTE_QTR && se2 >= CORTE_QTR && se1 >= CORTE_QTR) && (sd1 <= CORTE_QTR && sd2 <= CORTE_QTR && sd3 <= CORTE_QTR))
  {
    ligarLed(ESQ, BRANCO);
    seguirLinhaEsquerda();
    seguirLinhaEsquerda();
    seguirLinhaEsquerda();
    desligarLed(AMBOS);
  }

  // SEGUIDOR DE LINHA

  lerQTRASegueLinha();

  seguidorMoverFrente();

  if (se1 >= CORTE_QTR || se2 >= CORTE_QTR)
  {
    seguirLinhaEsquerda();
  }

  if (sd1 >= CORTE_QTR || sd2 >= CORTE_QTR)
  {
    seguirLinhaDireita();
  }

  

}