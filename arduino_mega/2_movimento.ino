// LINHA 1-136 - MOTORES GRANDES

void seguirLinhaEsquerda()
{
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, VEL_MOTOR_SEG_MIN);
  analogWrite(MOTOR_DIR_F_PIN, VEL_MOTOR_SEG_MAX);
  analogWrite(MOTOR_DIR_T_PIN, 0);
  delay(3);
}

void seguirLinhaDireita()
{
  analogWrite(MOTOR_ESQ_F_PIN, VEL_MOTOR_SEG_MAX);
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, VEL_MOTOR_SEG_MIN);
  delay(3);
}

void seguidorMoverFrente()
{
  analogWrite(MOTOR_ESQ_F_PIN, VEL_MOTOR_SEG_FRENTE);
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, VEL_MOTOR_SEG_FRENTE);
  analogWrite(MOTOR_DIR_T_PIN, 0);
  delay(1);
}

void moverFrente()
{
  analogWrite(MOTOR_ESQ_F_PIN, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void moverTras()
{
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, VEL_MOTOR_TRAS);
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, VEL_MOTOR_TRAS);
}

void pararMotor()
{
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void virarEsquerda()
{
  analogWrite(MOTOR_ESQ_F_PIN, 0);
  analogWrite(MOTOR_ESQ_T_PIN, VEL_MOTOR_CURVA);
  analogWrite(MOTOR_DIR_F_PIN, VEL_MOTOR_CURVA);
  analogWrite(MOTOR_DIR_T_PIN, 0);
}

void virarDireita()
{
  analogWrite(MOTOR_ESQ_F_PIN, VEL_MOTOR_CURVA);
  analogWrite(MOTOR_ESQ_T_PIN, 0);
  analogWrite(MOTOR_DIR_F_PIN, 0);
  analogWrite(MOTOR_DIR_T_PIN, VEL_MOTOR_CURVA);
}

void moverFrentePorMS(unsigned long ms)
{
  moverFrente();
  delay(ms);
}

void moverTrasPorMS(unsigned long ms)
{
  moverTras();
  delay(ms);
}

void virarEsquerdaPorMS(unsigned long ms)
{
  virarEsquerda();
  delay(ms);
}

void virarDireitaPorMS(unsigned long ms)
{
  virarDireita();
  delay(ms);
}

void virarEsquerdaGiro(uint8_t graus)
{
  lerGiroDMP();
  initialYaw = yaw; // Armazenar yaw inicial em graus

  float targetYaw = initialYaw - graus; // Alvo é 90 graus à direita do atual
  if (targetYaw < -180)
    targetYaw += 360; // Correção de ângulo

  // Código para mover o robô à esquerda
  virarEsquerda();

  while (true)
  {
    // Atualize a orientação atual
    lerGiroDMP();

    if (abs(yaw - targetYaw) <= 1)
      break; // Tolerância de 1 grau
  }
}

void virarDireitaGiro(uint8_t graus)
{
  lerGiroDMP();
  initialYaw = yaw; // Armazenar yaw inicial em graus

  float targetYaw = initialYaw + graus; // Alvo é 90 graus à esquerda do atual
  if (targetYaw > 180)
    targetYaw -= 360; // Correção de ângulo
  // Código para mover o robô à esquerda
  virarDireita();

  while (true)
  {
    // Atualize a orientação atual
    lerGiroDMP();

    if (abs(yaw - targetYaw) <= 1)
      break; // Tolerância de 1 grau
  }
}

void virarDireitaGiro90()
{
  virarDireitaGiro(110);
}

void virarEsquerdaGiro90()
{
  virarEsquerdaGiro(110);
}

// SERVOMOTORES

void movimentarServo(Servo *servo, uint8_t posicaoFinal, uint8_t velocidade)
{
  uint8_t posicaoAtual = servo->read();
  uint8_t passo = posicaoAtual > posicaoFinal ? -1 : 1;

  while (posicaoAtual != posicaoFinal)
  {
    posicaoAtual += passo;
    servo->write(posicaoAtual);
    delay(velocidade);
  }
}

void fecharPas()
{
  movimentarServo(&servoPaGarra, 80, 3);
}

void abrirPas()
{
  movimentarServo(&servoPaGarra, 0, 3);
}

void subirGarra()
{
  movimentarServo(&servoSubirGarra, 160, 10);
}

void subirGarraVerificaVitima()
{
  vitimaViva = false;
  uint8_t posicaoAtual = servoSubirGarra.read();
  while (posicaoAtual < 160)
  {
    posicaoAtual++;
    servoSubirGarra.write(posicaoAtual);
    delay(10);
    lerBtnVitima();
    if (btnVitima)
    {
      vitimaViva = true;
    }
  }
}

void descerGarra()
{
  movimentarServo(&servoSubirGarra, 0, 10);
}

void rotacionarGarraDir()
{
  movimentarServo(&servoRotacionarGarra, 90, 10);
}

void rotacionarGarraEsq()
{
  movimentarServo(&servoRotacionarGarra, 20, 10);
}

void rotacionarGarraMeio()
{
  movimentarServo(&servoRotacionarGarra, 55, 10);
}

void abrirCancelaEsq()
{
  movimentarServo(&servoCancelaEsq, 95, 4);
}

void fecharCancelaEsq()
{
  movimentarServo(&servoCancelaEsq, 3, 4);
}

void abrirCancelaDir()
{
  movimentarServo(&servoCancelaDir, 0, 4);
}

void fecharCancelaDir()
{
  movimentarServo(&servoCancelaDir, 99, 4);
}