void moverFrente()
{
  analogWrite(MOTOR_DF, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_DT, 0);
  analogWrite(MOTOR_EF, VEL_MOTOR_FRENTE);
  analogWrite(MOTOR_ET, 0);
  delay(1);
}

void moverTras()
{
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, 100);
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, 100);
  delay(1);
}

void pararMotor()
{
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, 0);
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, 0);
}

void segueLinhaEsquerda()
{
  analogWrite(MOTOR_DF, VEL_MOTOR_SEG_MAX);
  analogWrite(MOTOR_DT, 0);
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, VEL_MOTOR_SEG_MIN);
  delay(3);
}

void segueLinhaDireita()
{
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, VEL_MOTOR_SEG_MIN);
  analogWrite(MOTOR_EF, VEL_MOTOR_SEG_MAX);
  analogWrite(MOTOR_ET, 0);
  delay(3);
}



void moverTrasPorMS(unsigned long ms)
{
  moverTras();
  delay(ms);
}