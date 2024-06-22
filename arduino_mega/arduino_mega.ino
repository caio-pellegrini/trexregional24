#include "mega_def.h"

#define DEBUG 0

// rpp, rf, tcs no branco, tcs preto, tcs verde
// tcs branco fita vermelha

// ve - b 112 - v 72 - p 26
// vd - b 94 - v 62 - p 24

#define DEBUG_CALIBRACAO 1
#define DEBUG_EM_CURSO 0  // 1 para o robo ANDAR com o SERIAL LIGADO (não recomendado)
#define DEBUG_QTRA 1
#define DEBUG_REFL_FRENTE 1
#define DEBUG_TCS_AMBOS 1
#define DEBUG_TCS_FRENTE 0
#define DEBUG_GIROSCOPIO 1
#define DEBUG_LASER_FRENTE 0
#define DEBUG_LASER_GARRA 0
#define DEBUG_ULTRA 1
#define DEBUG_BOTOES 0

#define CORTE_QTR_P 195    // acima é preto - papel 208 - 202
#define CORTE_QTR_B 135    // abaixo é branco - papel 145 - madeira 165 // abaixe mais
#define CORTE_FRENTE 120   // abaixo é branco e acima é preto - papel 11
#define CORTE_FRENTE_B 25  // corte frente branco

#define CORTE_VERDE_ESQ 62  // abaixo disso é verde // 65 no verde escuro
#define CORTE_VERDE_DIR 60
#define CORTE_VERDE_ESQ2 9
#define CORTE_VERDE_DIR2 9
#define CORTE_VERMELHO_CRUZ 80  // 100

// USE VALORES DE 0 A 100
#define VEL_MOTOR_FRENTE 50
#define VEL_MOTOR_TRAS 40
#define VEL_MOTOR_CURVA 59 // 52
#define VEL_MOTOR_SEG_FRENTE 38
#define VEL_MOTOR_SEG_MAX 75
#define VEL_MOTOR_SEG_MIN 68  // 75

#define TEMPO_MOVER_ANTES_ANALISAR_VERDE 145
#define TEMPO_MOVER_ANTES_CRUZ 360  // 375

#define DIST_LASER_GARRA_VIT 45
#define DIST_OBSTACULO 70

#define CORTE_ULTRA_SALA_RESGATE 7
#define INCLINACAO 7


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

  pinMode(LASER_FRENTE_XSHUT, OUTPUT);
  pinMode(LASER_GARRA_XSHUT, OUTPUT);

  digitalWrite(LASER_FRENTE_XSHUT, LOW);
  digitalWrite(LASER_GARRA_XSHUT, LOW);
  delay(50);  // testar mudar para 10

  digitalWrite(LASER_FRENTE_XSHUT, HIGH);
  delay(50);
  // Configura o sensor VL53L0X
  laserFrente.setTimeout(500);  // padrão 500
  Serial.println(laserFrente.init() ? "Laser Frente conectado :)"
                                    : "Laser Frente falhou :(");
  laserFrente.setAddress(LASER_FRENTE_ENDERECO);
  laserFrente.startContinuous();  // Inicia leituras contínuas

  // i2c_scanner();

  digitalWrite(LASER_GARRA_XSHUT, HIGH);
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

int contUltra = 0;

void loop() {
#if DEBUG
#if DEBUG_CALIBRACAO
  calibrar();
#endif
#if defined(DEBUG_EM_CURSO) && (DEBUG_EM_CURSO == 0)
  return;
#endif
#endif

  // unsigned long tempoAtual = millis();

  #if (RAMPA == 1) || (GANGORRA == 1)
    lerGiroscopioDMP();
    if (pitch > INCLINACAO) {
      rampaOuGangorra();
    }
  #endif

  #if defined(FITA_PRATEADA) && (FITA_PRATEADA == 0)
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


  lerQTRATodos();
  lerReflFrente();

  // se for papel - se3 <= (CORTE_QTR_B - 20)
  if (se2 <= CORTE_QTR_B && se1 <= CORTE_QTR_B && se0 <= CORTE_QTR_B && sd0 <= CORTE_QTR_B && sd1 <= CORTE_QTR_B && sd2 <= CORTE_QTR_B) {
    // se nao estiver func, colocar todos os sensores
    verificarGap();
  }

  // 90 GRAUS DIREITO
  if (sf <= CORTE_FRENTE_B && (se3 <= CORTE_QTR_P && se2 <= CORTE_QTR_P && se1 <= CORTE_QTR_P) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
    seguirLinhaDireita(100);
    // moverFrentePor(TEMPO_MOVER_ANTES_CRUZ);
    // virarDireitaGiro90();
    // pararMotores();
  }

  // 90 GRAUS ESQUERDO
  if (sf <= CORTE_FRENTE_B && (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd1 <= CORTE_QTR_P && sd2 <= CORTE_QTR_P && sd3 <= CORTE_QTR_P)) {
    seguirLinhaEsquerda(100);
    // moverFrentePor(TEMPO_MOVER_ANTES_CRUZ);
    // virarEsquerdaGiro90();
    // pararMotores();
  }

  // CRUZAMENTO
  if (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P && se0 >= CORTE_QTR_P && sd0 >= CORTE_QTR_P && sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P) {
    ligarLed(AMBOS, BRANCO);
    moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotores();
    analisarVerde(true, true, true); // true true true
    desligarLed(AMBOS);
  }

  // // MEIO CRUZAMENTO ESQUERDO
  // if (sf >= CORTE_FRENTE && (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd1 <= CORTE_QTR_P && sd2 <= CORTE_QTR_P && sd3 <= CORTE_QTR_P)) {
  //   ligarLed(ESQ, BRANCO);
  //   moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
  //   pararMotores();
  //   analisarVerde(false, false, false);  // false, true, false
  //   desligarLed(AMBOS);
  // }

  // MEIO CRUZAMENTO DIREITO
  // if (sf >= CORTE_FRENTE && (se3 <= CORTE_QTR_P && se2 <= CORTE_QTR_P && se1 <= CORTE_QTR_P) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
  //   ligarLed(DIR, BRANCO);
  //   moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
  //   pararMotores();
  //   analisarVerde(false, false, true);  // false, false, true
  //   desligarLed(AMBOS);
  // }


  // SEGUIDOR DE LINHA
  lerQTRASegueLinha();

  // se for fita estilo silver tape (com pouco reflexo)
  seguidorMoverFrente();

  if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P) {
    seguirLinhaEsquerda(3);
  }

  if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P) {
    seguirLinhaDireita(3);
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

  lerQTRATodos();
  lerReflFrente();
  // 90 GRAUS DIREITO
  if (sf <= CORTE_FRENTE && (se3 <= CORTE_QTR_P && se2 <= CORTE_QTR_P && se1 <= CORTE_QTR_P) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
    seguirLinhaDireita(10);
  }

  // 90 GRAUS ESQUERDO
  if (sf <= CORTE_FRENTE && (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd1 <= CORTE_QTR_P && sd2 <= CORTE_QTR_P && sd3 <= CORTE_QTR_P)) {
    seguirLinhaEsquerda(10);
  }
  // passar lá pra baixo

  // Serial.print("Tempo: ");
  // Serial.print(millis() - tempoAtual);
  // Serial.println("ms");
}