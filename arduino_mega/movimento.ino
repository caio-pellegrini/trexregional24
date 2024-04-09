void frente() {
  analogWrite(MOTOR_DF, 85);
  analogWrite(MOTOR_DT, 0);
  analogWrite(MOTOR_EF, 85);
  analogWrite(MOTOR_ET, 0);
  delay(1);
}

void segueLinhaEsquerda() {
  analogWrite(MOTOR_DF, 220);
  analogWrite(MOTOR_DT, 0);
  analogWrite(MOTOR_EF, 0);
  analogWrite(MOTOR_ET, 195);
  delay(1);
}

void segueLinhaDireita() {
  analogWrite(MOTOR_DF, 0);
  analogWrite(MOTOR_DT, 195);
  analogWrite(MOTOR_EF, 220);
  analogWrite(MOTOR_ET, 0);
  delay(1);
}