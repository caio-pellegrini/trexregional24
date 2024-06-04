#include <Wire.h>
#include <Adafruit_TCS34725.h>

#define TCAADDR 0x70

Adafruit_TCS34725 tcsMulti = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcsBar = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

uint8_t canalTcsMulti = 6;

void tcaSelecionar(uint8_t i) {
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void tcadesliga() {
  Wire.beginTransmission(TCAADDR);
  Wire.write(0);  // Desligar todos os canais
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println(tcsBar.begin() ? "TCS34725 Bar conectado :)" : "TCS34725 Bar conexão falhou :(");

  tcadesliga();
  tcaSelecionar(canalTcsMulti);  // Selecionar o primeiro sensor de cor e inicializá-lo
  Serial.println(tcsMulti.begin() ? "TCS34725 Multi conectado :)" : "TCS34725 Multi conexão falhou :(");
  tcadesliga();
}

void loop() {
  Serial.print("TCS BAR: ");
  lerSensorCor(&tcsBar);
  tcadesliga();

  delay(500);

  Serial.print("  TCS MULTI: ");
  tcadesliga();
  tcaSelecionar(canalTcsMulti);
  lerSensorCor(&tcsMulti);
  tcadesliga();

  Serial.println();
}

void lerSensorCor(Adafruit_TCS34725 *tcs) {
  uint16_t r, g, b, c, colorTemp, lux;

  tcs->getRawData(&r, &g, &b, &c);
  // colorTemp = tcs->calculateColorTemperature(r, g, b);
  // lux = tcs->calculateLux(r, g, b);

  Serial.print("R: ");
  Serial.print(r);
  Serial.print(" G: ");
  Serial.print(g);
  Serial.print(" B: ");
  Serial.print(b);
}