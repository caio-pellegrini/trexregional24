void ligarLed(bool esq, bool dir, byte r, byte g, byte b, unsigned long delayTempo) {

  if (esq) {
    analogWrite(rgbE[0], r);
    analogWrite(rgbE[1], g);
    analogWrite(rgbE[2], b);
  }
  if (dir) {
    analogWrite(rgbD[0], r);
    analogWrite(rgbD[1], g);
    analogWrite(rgbD[2], b);
  }

  if (delayTempo != 0) {
    delay(delayTempo);
    if (esq) {
      for (byte i = 0; i < 3; i++) analogWrite(rgbE[i], 0);
    }
    if (dir) {
      for (byte i = 0; i < 3; i++) analogWrite(rgbD[i], 0);
    }
  }
}

void desligarLed(bool esq, bool dir) {
  if (esq) {
    for (byte i = 0; i < 3; i++) analogWrite(rgbE[i], 0);
  }
  if (dir) {
    for (byte i = 0; i < 3; i++) analogWrite(rgbD[i], 0);
  }
}