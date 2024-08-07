// LINHA 1-136 - MOTORES GRANDES

void seguirLinhaEsquerda(unsigned long ms) {
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, CONVERT_8B_DEC(VEL_MOTOR_SEG_MIN));
  analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_SEG_MAX));
  analogWrite(MOTOR_DIR_T_PIN, 0);
  delay(ms);
}

void seguirLinhaDireita(unsigned long ms) {
  analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_SEG_MAX));
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, CONVERT_8B_DEC(VEL_MOTOR_SEG_MIN));
  delay(ms);
}

void seguidorMoverFrente() {
  analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_SEG_FRENTE));
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_SEG_FRENTE));
  analogWrite(MOTOR_DIR_T_PIN, 0);
  delay(1);
  // delayMicroseconds(750);
}

void moverFrente() {
  analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_FRENTE));
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_FRENTE));
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void moverFrenteLento() {
  analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(35));
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(35));
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void moverFrenteRapido() {
  analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(60));
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(60));
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void moverTras() {
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, CONVERT_8B_DEC(VEL_MOTOR_TRAS));
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, CONVERT_8B_DEC(VEL_MOTOR_TRAS));
}

void moverTrasRapido() {
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, CONVERT_8B_DEC(72));
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, CONVERT_8B_DEC(72));
}

void pararMotores() {
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void virarEsquerda() {
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, CONVERT_8B_DEC(VEL_MOTOR_CURVA));
  analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_CURVA));
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void virarDireita() {
  analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_CURVA));
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, CONVERT_8B_DEC(VEL_MOTOR_CURVA));
}

void virarDireitaUmMotor() {
  analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_CURVA));
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void virarEsquerdaUmMotor() {
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(VEL_MOTOR_CURVA));
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void moverFrentePor(unsigned long ms) {
  moverFrente();
  delay(ms);
}

void moverFrenteLentoPor(unsigned long ms) {
  moverFrenteLento();
  delay(ms);
}

void moverFrenteRapidoPor(unsigned long ms) {
  moverFrenteRapido();
  delay(ms);
}

void moverTrasPor(unsigned long ms) {
  moverTras();
  delay(ms);
}

void moverTrasRapidoPor(unsigned long ms) {
  moverTrasRapido();
  delay(ms);
}

void virarEsquerdaPor(unsigned long ms) {
  virarEsquerda();
  delay(ms);
}

void virarDireitaPor(unsigned long ms) {
  virarDireita();
  delay(ms);
}

void virarEsquerdaGiro(uint8_t graus, bool umMotor) {
  lerGiroscopioDMP();
  initialYaw = yaw; // Armazenar yaw inicial em graus
  float targetYaw = initialYaw - graus; // Alvo é 90 graus à direita do atual
  if (targetYaw < -180)
    targetYaw += 360; // Correção de ângulo

  // Código para mover o robô à esquerda
  if (umMotor) {
    virarEsquerdaUmMotor();
  } else {
    virarEsquerda();
  }

  while (true) {
    // Atualize a orientação atual
    lerGiroscopioDMP();
    if (abs(yaw - targetYaw) <= 1)
      break; // Tolerância de 1 grau
  }
}

void virarDireitaGiro(uint8_t graus, bool umMotor) {
  lerGiroscopioDMP();
  initialYaw = yaw; // Armazenar yaw inicial em graus
  float targetYaw = initialYaw + graus; // Alvo é 90 graus à esquerda do atual
  if (targetYaw > 180) {
    targetYaw -= 360; // Correção de ângulo
  }

  // Código para mover o robô à direita
  if (umMotor) {
    virarDireitaUmMotor();
  } else {
    virarDireita();
  }

  while (true) {
    // Atualize a orientação atual
    lerGiroscopioDMP();
    if (abs(yaw - targetYaw) <= 1)
      break; // Tolerância de 1 grau
  }
  // melhorar funcao de cima colocando a condicao no lugar do true
}
void virarEsquerdaGiro45() { virarEsquerdaGiro(53, false); }

void virarDireitaGiro45() { virarDireitaGiro(53, false); }

void virarEsquerdaGiro90() { virarEsquerdaGiro(108, false); }

void virarDireitaGiro90() { virarDireitaGiro(108, false); }

void virarEsquerdaGiro180() { virarEsquerdaGiro(214, false); }

void virarDireitaGiro180() { virarDireitaGiro(214, false); }

void virarEsquerdaGiro90UmMotor() { virarEsquerdaGiro(105, true); }

void virarDireitaGiro90UmMotor() { virarDireitaGiro(105, true); }

// SERVOMOTORES

void movimentarServo(Servo *servo, uint8_t posicaoFinal, uint8_t velocidade) {
  uint8_t posicaoAtual = servo->read();
  uint8_t passo = posicaoAtual > posicaoFinal ? -1 : 1;

  while (posicaoAtual != posicaoFinal) {
    posicaoAtual += passo;
    servo->write(posicaoAtual);
    delay(velocidade);
  }
}

void fecharGarra() { movimentarServo(&servoPaGarra, SERVO_PA_GARRA_POS_INICIAL, 3); }

void abrirGarra() { movimentarServo(&servoPaGarra, 35, 3); }

void subirGarra() { movimentarServo(&servoSubirGarra, SERVO_SUBIR_GARRA_POS_INICIAL, 6); }

void descerGarraRampa() { movimentarServo(&servoSubirGarra, 15, 3); }
// GANGORRA OU RAMPA - 30
// RAMPA SALA RESGATE - 15

void descerGarra() { movimentarServo(&servoSubirGarra, 0, 6); }

void rotacionarGarraDir() { movimentarServo(&servoRotacionarGarra, 145, 10); }

void rotacionarGarraMeio() { movimentarServo(&servoRotacionarGarra, SERVO_ROTACIONAR_GARRA_POS_INICIAL, 10); }

void rotacionarGarraEsq() { movimentarServo(&servoRotacionarGarra, 65, 10); }

void abrirCancelaEsq() { movimentarServo(&servoCancelaEsq, 170, 4); }

void fecharCancelaEsq() { movimentarServo(&servoCancelaEsq, SERVO_CANCELA_ESQ_POS_INICIAL, 4); }

void abrirCancelaDir() { movimentarServo(&servoCancelaDir, 75, 4); }

void fecharCancelaDir() { movimentarServo(&servoCancelaDir, SERVO_CANCELA_DIR_POS_INICIAL, 4); }