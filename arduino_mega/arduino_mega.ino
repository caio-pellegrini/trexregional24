#include "mega_def.h"

#define DEBUG 0

#define DEBUG_CALIBRACAO 1
#define DEBUG_EM_CURSO 0  // 1 para o robo ANDAR com o SERIAL LIGADO (não recomendado)
#define DEBUG_QTRA 0
#define DEBUG_REFL_FRENTE 0
#define DEBUG_TCS_AMBOS 0
#define DEBUG_TCS_FRENTE 0
#define DEBUG_GIROSCOPIO 1
#define DEBUG_LASER_FRENTE 0
#define DEBUG_LASER_GARRA 0
#define DEBUG_ULTRA 0
#define DEBUG_BOTOES 0

#define CORTE_QTR 200
#define CORTE_FRENTE 55  // 6 no branco e 110 no preto
#define CORTE_2 165

#define CORTE_VERDE_ESQ 65  // abaixo disso é verde
#define CORTE_VERDE_DIR 45
#define CORTE_VERMELHO_CRUZ 100

// USE VALORES DE 0 A 100
#define VEL_MOTOR_FRENTE 50
#define VEL_MOTOR_TRAS 40
#define VEL_MOTOR_CURVA 50

#define VEL_MOTOR_SEG_FRENTE 33
#define VEL_MOTOR_SEG_MAX 75
#define VEL_MOTOR_SEG_MIN 69

#define TEMPO_MOVER_ANTES_ANALISAR_VERDE 160
#define TEMPO_MOVER_ANTES_CRUZ 375

#define DIST_LASER_GARRA_VIT 47
#define DIST_OBSTACULO 70

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
  // while (1) {}
  laserFrente.setAddress(LASER_FRENTE_ENDERECO);
  laserFrente.startContinuous();  // Inicia leituras contínuas

  // i2c_scanner();

  digitalWrite(LASER_GARRA_XSHUT, HIGH);
  delay(50);
  laserGarra.setTimeout(500);
  Serial.println(laserGarra.init() ? "Laser Garra conectado :)"
                                   : "Laser Garra falhou :(");
  laserGarra.setAddress(LASER_GARRA_ENDERECO);

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

  // entrarSalaResgate();
}

void loop() {
#if DEBUG
#if DEBUG_CALIBRACAO
  calibrar();
#endif
#if defined(DEBUG_EM_CURSO) && (DEBUG_EM_CURSO == 0)
  return;
#endif
#endif

  // leitura não bloqueante
  if (lerLaserFrenteNaoBloquante()) {
    if (distanciaLaserFrente != 0 && distanciaLaserFrente <= DIST_OBSTACULO) {
      // !laserFrente.timeoutOccurred() &&
      desviarObstaculo(false);
    }
  };

  // unsigned long tempoAtual = millis();

  lerGiroscopioDMP();

  if (pitch > 12) {
    rampa();
  }


  lerUltraEsq();
  lerUltraDir();
  if (distanciaUltraEsq < 8 && distanciaUltraDir < 8) {
    ligarLed(AMBOS, ROXO);
    moverFrentePor(500);
    pararMotores();
    verificarGap();
  }

  lerQTRATodos();
  lerReflFrente();

  if (se3 <= CORTE_2 && se2 <= CORTE_2 && se1 <= CORTE_2 && se0 <= CORTE_2 && sd0 <= CORTE_2 && sd1 <= CORTE_2 && sd2 <= CORTE_2 && sd3 <= CORTE_2) {
    verificarGap();
  }

  // CRUZAMENTO
  if (se3 >= CORTE_QTR && se2 >= CORTE_QTR && se1 >= CORTE_QTR && se0 >= CORTE_QTR && sd0 >= CORTE_QTR && sd1 >= CORTE_QTR && sd2 >= CORTE_QTR && sd3 >= CORTE_QTR) {
    ligarLed(AMBOS, BRANCO);
    moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotores();
    analisarVerde(true, true, true);
    desligarLed(AMBOS);
  }

  // MEIO CRUZAMENTO ESQUERDO
  if (sf >= CORTE_FRENTE && (se3 >= CORTE_QTR && se2 >= CORTE_QTR && se1 >= CORTE_QTR) && (sd1 <= CORTE_QTR && sd2 <= CORTE_QTR && sd3 <= CORTE_QTR)) {
    ligarLed(ESQ, BRANCO);
    moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotores();
    analisarVerde(false, true, false);
    desligarLed(AMBOS);
  }

  // MEIO CRUZAMENTO DIREITO
  if (sf >= CORTE_FRENTE && (se3 <= CORTE_QTR && se2 <= CORTE_QTR && se1 <= CORTE_QTR) && (sd1 >= CORTE_QTR && sd2 >= CORTE_QTR && sd3 >= CORTE_QTR)) {
    ligarLed(DIR, BRANCO);
    moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotores();
    analisarVerde(false, false, true);
    desligarLed(AMBOS);
  }

  // 90 GRAUS DIREITO
  if (sf <= CORTE_FRENTE && (se3 <= CORTE_QTR && se2 <= CORTE_QTR && se1 <= CORTE_QTR) && (sd1 >= CORTE_QTR && sd2 >= CORTE_QTR && sd3 >= CORTE_QTR)) {
    seguirLinhaDireita(12);
  }

  // 90 GRAUS ESQUERDO
  if (sf <= CORTE_FRENTE && (se3 >= CORTE_QTR && se2 >= CORTE_QTR && se1 >= CORTE_QTR) && (sd1 <= CORTE_QTR && sd2 <= CORTE_QTR && sd3 <= CORTE_QTR)) {
    seguirLinhaEsquerda(12);
  }
  // passar lá pra baixo

  // SEGUIDOR DE LINHA
  lerQTRASegueLinha();

  // se for fita estilo silver tape (com pouco reflexo)
  seguidorMoverFrente();

  if (se1 >= CORTE_QTR || se2 >= CORTE_QTR) {
    seguirLinhaEsquerda(3);
  }

  if (sd1 >= CORTE_QTR || sd2 >= CORTE_QTR) {
    seguirLinhaDireita(3);
  }

  // Serial.print("Tempo: ");
  // Serial.print(millis() - tempoAtual);
  // Serial.println("ms");
}