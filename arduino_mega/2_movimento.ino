void segueLinhaEsquerda()
{
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, VEL_MOTOR_SEG_MIN);
  analogWrite(MOTOR_DF, VEL_MOTOR_SEG_MAX);
  analogWrite(MOTOR_DT, 0);
  delay(3);
}

void segueLinhaDireita()
{
  analogWrite(MOTOR_EF, VEL_MOTOR_SEG_MAX);
  analogWrite(MOTOR_ET, 0);
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, VEL_MOTOR_SEG_MIN);
  delay(3);
}

void moverFrente()
{
  analogWrite(MOTOR_EF, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_ET, 0);
  analogWrite(MOTOR_DF, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_DT, 0);
  delay(1);
}

void moverTras()
{
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, 100);
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, 100);
  delay(1);
}

void pararMotor()
{
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, 0);
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, 0);
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

void virarEsquerda()
{
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, VEL_MOTOR_CURVA);
  analogWrite(MOTOR_DF, VEL_MOTOR_CURVA);
  analogWrite(MOTOR_DT, 0);
}

void virarDireita()
{
  analogWrite(MOTOR_EF, VEL_MOTOR_CURVA);
  analogWrite(MOTOR_ET, 0);
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, VEL_MOTOR_CURVA);
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
  initialYaw = yaw;  // Armazenar yaw inicial em graus

  float targetYaw = initialYaw - graus; // Alvo é 90 graus à direita do atual
  if (targetYaw < -180) targetYaw += 360;  // Correção de ângulo

  // Código para mover o robô à esquerda
  virarEsquerda();

  while (true) {
    // Atualize a orientação atual
    lerGiroDMP();

    if (abs(yaw - targetYaw) <= 1) break;  // Tolerância de 1 grau
  }
}

void virarDireitaGiro(uint8_t graus)
{
  lerGiroDMP();
  initialYaw = yaw;  // Armazenar yaw inicial em graus

  float targetYaw = initialYaw + graus; // Alvo é 90 graus à esquerda do atual
  if (targetYaw > 180) targetYaw -= 360;  // Correção de ângulo
  // Código para mover o robô à esquerda
  virarDireita();

  while (true) {
    // Atualize a orientação atual
    lerGiroDMP();

    if (abs(yaw - targetYaw) <= 1) break;  // Tolerância de 1 grau
  }
}
