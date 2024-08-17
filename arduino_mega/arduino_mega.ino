#include "MPU6050_6Axis_MotionApps612.h"
#include <Adafruit_TCS34725.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <VL53L0X_mod.h>
#include <Wire.h>

#include "mega_pins.h"
#include "mega_def.h"

void setup() {
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

  pinMode(LASER_FRENTE_XSHUT_PIN, OUTPUT);
  pinMode(LASER_GARRA_XSHUT_PIN, OUTPUT);

  digitalWrite(LASER_FRENTE_XSHUT_PIN, LOW);
  digitalWrite(LASER_GARRA_XSHUT_PIN, LOW);
  delay(50);  // testar mudar para 10

  digitalWrite(LASER_FRENTE_XSHUT_PIN, HIGH);
  delay(50);
  // Configura o sensor VL53L0X
  laserFrente.setTimeout(500);  // padrão 500
  Serial.println(laserFrente.init() ? "Laser Frente conectado :)" : "Laser Frente falhou :(");
  laserFrente.setAddress(LASER_FRENTE_ENDERECO);
  laserFrente.startContinuous();  // Inicia leituras contínuas

  digitalWrite(LASER_GARRA_XSHUT_PIN, HIGH);
  delay(50);
  laserGarra.setTimeout(500);
  Serial.println(laserGarra.init() ? "Laser Garra conectado :)" : "Laser Garra falhou :(");
  laserGarra.setAddress(LASER_GARRA_ENDERECO);
  laserGarra.startContinuous();
  // laserGarra.setMeasurementTimingBudget(200000); // -> alta precisão

  tcaSelecionar(CANAL_TCS_ESQ);
  Serial.println(tcsEsq.begin() ? "TCS Esq conectado :)" : "TCS Esq falhou :(");
  tcaDesligar();

  tcaSelecionar(CANAL_TCS_FRENTE);
  Serial.println(tcsFrente.begin() ? "TCS Frente conectado :)" : "TCS Frente falhou :(");
  tcaDesligar();

  setupAlgunsPins();

  // SERVOS
  servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

  servoPaGarra.write(SERVO_PA_GARRA_POS_INICIAL);
  servoSubirGarra.write(SERVO_SUBIR_GARRA_POS_INICIAL);
  servoRotacionarGarra.write(SERVO_ROTACIONAR_GARRA_POS_INICIAL);
  servoCancelaEsq.write(SERVO_CANCELA_ESQ_POS_INICIAL);
  servoCancelaDir.write(SERVO_CANCELA_DIR_POS_INICIAL);

  ligarGiroscopio();

  // transformer();  

  // Desenxa depois de ligar o giroscópio para dar tempo ir pra pos inicial
  servoPaGarra.detach();
  servoSubirGarra.detach();
  servoRotacionarGarra.detach();
  servoCancelaEsq.detach();
  servoCancelaDir.detach();

  #if defined(DEBUG) && (DEBUG == 0)
  Serial.print("Desligando Serial");
  Serial.end();
  #endif  

  desligarLed(AMBOS);
}

void loop() {
  #if defined(DEBUG) && (DEBUG == 1)
    calibrar();
    return; // Comente essa linha para o robo ANDAR com o SERIAL LIGADO (não recomendado)
  #endif

  // unsigned long tempoAtual = millis();

  #if (RAMPA == 1) || (GANGORRA == 1)
    lerGiroscopioDMP();
    if (pitch > INCLINACAO) { rampaOuGangorra(); }
  #endif

  #if defined(ULTRA_ENTRADA_SALA) && (ULTRA_ENTRADA_SALA == 1)
  if (contUltra == 4) {
    lerUltraEsq();
    lerUltraDir();
    if (distanciaUltraEsq < 8 && distanciaUltraDir < 8) {
      ligarLed(AMBOS, ROXO);
      moverFrentePor(500);
      pararMotores();
      verificarGap();
    }
    contUltra = 0;
  } else {
    contUltra++;
  }
  #endif

  #if (RAMPA_SALA_RESGATE == 1)
  lerGiroscopioDMP();
  if (pitch > 9) {
    moverFrentePor(200);
    pararMotores();
    lerUltraEsq();
    lerUltraDir();

    if (distanciaUltraEsq < CORTE_ULTRA_SALA_RESGATE && distanciaUltraDir < CORTE_ULTRA_SALA_RESGATE) {
      ligarLed(AMBOS, ROXO);
      moverFrentePor(200);
      pararMotores();
      // verificarGap();
      lerGiroscopioDMP();
      if (pitch > 9) {
        rampaSalaResgate();
      }
      desligarLed(AMBOS);
    }
  }
  #endif

  #if (LOMBADA == 1)
  lerGiroscopioDMP(); // ver se é mesmo necessário
  if (roll > 3) ligarLed(ESQ, ROXO);
  else if(roll < -3) ligarLed(DIR, ROXO);
  else desligarLed(AMBOS);
  #endif


  lerReflPrincipal();
  lerReflFrente();

  #if (GAP == 1)
    if (se3 <= LUZ_BRANCO && se2 <= LUZ_BRANCO && se1 <= LUZ_BRANCO && sm <= LUZ_BRANCO && sd1 <= LUZ_BRANCO && sd2 <= LUZ_BRANCO && sd3 <= LUZ_BRANCO) {
      // se nao estiver func, colocar todos os sensores
      verificarGap();
    }
  #endif

  // CRUZAMENTO
  verificarCruzamento();

  // MEIO CRUZAMENTO ESQUERDO
  #if (MCE == 1)
    verificarMeioCruzamentoEsq();
  #endif

  // MEIO CRUZAMENTO DIREITO
  #if (MCD == 1)
    verificarMeioCruzamentoDir();
  #endif

  // SEGUIDOR DE LINHA
  lerReflSegueLinha();
  seguidorMoverFrente();
  if (sd1 >= LUZ_PRETO || sd2 >= LUZ_PRETO || sd3 >= LUZ_PRETO) seguirLinhaDir(6);
  if (se1 >= LUZ_PRETO || se2 >= LUZ_PRETO || se3 >= LUZ_PRETO) seguirLinhaEsq(6);

#if defined(OBSTACULO) && (OBSTACULO == 1)
  // leitura não bloqueante
  if (lerLaserFrenteNaoBloquante()) {
    if (distanciaLaserFrente != 0 && distanciaLaserFrente <= DIST_OBSTACULO) {
      // !laserFrente.timeoutOccurred() &&
      desviarObstaculo(false);
    }
  }
#endif

  #if defined(GRAUS90) && (GRAUS90 == 1)
    lerReflPrincipal();
    lerReflFrente();

    // 90 GRAUS ESQUERDO
    verificar90GrausEsq();

    // 90 GRAUS DIREITO
    verificar90GrausDir();
  #endif

  // Serial.print("Tempo: ");
  // Serial.print(millis() - tempoAtual);
  // Serial.println("ms");
}