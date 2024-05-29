void ligarLed(bool esq, bool dir, uint8_t r, uint8_t g, uint8_t b, unsigned long delayTempo) {

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
      for (uint8_t i = 0; i < 3; i++) analogWrite(rgbE[i], 0);
    }
    if (dir) {
      for (uint8_t i = 0; i < 3; i++) analogWrite(rgbD[i], 0);
    }
  }
}

void desligarLed(bool esq, bool dir) {
  if (esq) {
    for (uint8_t i = 0; i < 3; i++) analogWrite(rgbE[i], 0);
  }
  if (dir) {
    for (uint8_t i = 0; i < 3; i++) analogWrite(rgbD[i], 0);
  }
}

void printDebug(String msg) {
  if (DEBUG) {
    Serial.println(msg);
  }
}