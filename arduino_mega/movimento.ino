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

void virarEsquerdaPorMS(unsigned long ms)
{
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_DF, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_DT, 0);
  delay(ms);
}

void virarDireitaPorMS(unsigned long ms)
{
  analogWrite(MOTOR_EF, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_ET, 0);
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, VEL_MOTOR_FRENTE);
  delay(ms);
}

