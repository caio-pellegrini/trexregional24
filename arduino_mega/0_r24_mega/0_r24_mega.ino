const byte rgbE[3] = { 53, 51, 49 };  // Vermelho, Verde, Azul
const byte rgbD[3] = { 52, 50, 48 };  // Vermelho, Verde, Azul

#define DESLIGADO   0,0,0
#define VERMELHO    255,0,0
#define VERDE       0,255,0
#define AZUL        0,0,255

#define AMBOS       true, true
#define ESQ         true, false
#define DIR         false, true

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

void setup() {
  for (byte i = 0; i < 3; i++) {
    pinMode(rgbD[i], OUTPUT);
    pinMode(rgbE[i], OUTPUT);
  }
}

void loop() {
  ligarLed(AMBOS, VERMELHO, 1000);
  ligarLed(AMBOS, DESLIGADO, 1000);
  ligarLed(AMBOS, VERDE, 1000);
  ligarLed(AMBOS, DESLIGADO, 1000);
  ligarLed(AMBOS, AZUL, 1000);
  ligarLed(AMBOS, DESLIGADO, 1000);
}