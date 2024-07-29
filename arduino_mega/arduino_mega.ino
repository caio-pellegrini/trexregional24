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
  Serial.println(laserFrente.init() ? "Laser Frente conectado :)"
                                    : "Laser Frente falhou :(");
  laserFrente.setAddress(LASER_FRENTE_ENDERECO);
  laserFrente.startContinuous();  // Inicia leituras contínuas

  // i2c_scanner();

  digitalWrite(LASER_GARRA_XSHUT_PIN, HIGH);
  delay(50);
  laserGarra.setTimeout(500);
  Serial.println(laserGarra.init() ? "Laser Garra conectado :)"
                                   : "Laser Garra falhou :(");
  laserGarra.setAddress(LASER_GARRA_ENDERECO);
  laserGarra.startContinuous();
  // laserGarra.setMeasurementTimingBudget(200000); // -> alta precisão

  // i2c_scanner();

  tcaSelecionar(CANAL_TCS_ESQ);
  Serial.println(tcsEsq.begin() ? "TCS Esq conectado :)" : "TCS Esq falhou :(");
  tcaDesligar();

  tcaSelecionar(CANAL_TCS_FRENTE);
  Serial.println(tcsFrente.begin() ? "TCS Frente conectado :)"
                                   : "TCS Frente falhou :(");
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
  pinMode(SM_PIN, INPUT);
  // pinMode(SE0_PIN, INPUT);
  // pinMode(SD0_PIN, INPUT);
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

  // Desenxa depois de ligar o giroscópio para dar tempo dele ir pra posição
  // inicial
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
    if (pitch > INCLINACAO) {
      rampaOuGangorra();
    }
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


  lerReflPrincipal();
  lerReflFrente();

  #if (GAP == 1)
    if (se3 <= CORTE_QTR_B && se2 <= CORTE_QTR_B && se1 <= CORTE_QTR_B && sm <= CORTE_QTR_B && sd1 <= CORTE_QTR_B && sd2 <= CORTE_QTR_B && sd3 <= CORTE_QTR_B) {
      // se nao estiver func, colocar todos os sensores
      verificarGap();
    }
  #endif

  // // 90 GRAUS DIREITO
  // if (sf <= CORTE_FRENTE_B && sm >= CORTE_QTR_P && (se3 <= CORTE_QTR_P && se2 <= CORTE_QTR_P && se1 <= CORTE_QTR_P) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
  //   seguirLinhaDireita(50);
  //   // moverFrentePor(TEMPO_MOVER_ANTES_CRUZ);
  //   // virarDireitaGiro90();
  //   // pararMotores();
  // }

  // // 90 GRAUS ESQUERDO
  // if (sf <= CORTE_FRENTE_B && sm >= CORTE_QTR_P && (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd1 <= CORTE_QTR_P && sd2 <= CORTE_QTR_P && sd3 <= CORTE_QTR_P)) {
  //   seguirLinhaEsquerda(50);
  //   // moverFrentePor(TEMPO_MOVER_ANTES_CRUZ);
  //   // virarEsquerdaGiro90();
  //   // pararMotores();
  // }

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

  if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P || sd3 >= CORTE_QTR_P) {
    seguirLinhaDireita(7);
  }

  if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P || se3 >= CORTE_QTR_P) {
    seguirLinhaEsquerda(7);
  }

#if defined(OBSTACULO) && (OBSTACULO == 1)
  // leitura não bloqueante
  if (lerLaserFrenteNaoBloquante()) {
    if (distanciaLaserFrente != 0 && distanciaLaserFrente <= DIST_OBSTACULO) {
      // !laserFrente.timeoutOccurred() &&
      desviarObstaculo(true);
    }
  }
#endif

  lerReflPrincipal();
  lerReflFrente();
  // 90 GRAUS DIREITO
  if (sf <= CORTE_FRENTE_B && sm >= CORTE_QTR_P && (se3 <= CORTE_QTR_B && se2 <= CORTE_QTR_B) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
    // seguirLinhaDireita(50);
    virarDireitaPor(50);
  }

  // 90 GRAUS ESQUERDO
  if (sf <= CORTE_FRENTE_B && sm >= CORTE_QTR_P && (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd2 <= CORTE_QTR_B && sd3 <= CORTE_QTR_B)) {
    // seguirLinhaEsquerda(50);
    virarEsquerdaPor(50);
  }

  // Serial.print("Tempo: ");
  // Serial.print(millis() - tempoAtual);
  // Serial.println("ms");
}