#include <Wire.h>
#include <Adafruit_TCS34725.h>

#define TCAADDR 0x70

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

uint8_t canalTcsDir = 1;

void tcaSelecionar(uint8_t i) {
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void tcadesliga() {
  Wire.beginTransmission(TCAADDR);
  Wire.write(0); // Desligar todos os canais
  Wire.endTransmission();
}

void setup() {
    Wire.begin();
    Serial.begin(9600);

    tcaSelecionar(canalTcsDir);  // Selecionar o primeiro sensor de cor e inicializá-lo
    Serial.println(tcs.begin() ? "TCS34725 #1 conectado :)" : "TCS34725 #1 conexão falhou :(");
}

void loop() {
    tcaSelecionar(canalTcsDir);
    Serial.println(lerSensorCor(&tcs));
    delay(1000);
}

int lerSensorCor(Adafruit_TCS34725 *tcs) {
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs->getRawData(&r, &g, &b, &c);
  // colorTemp = tcs->calculateColorTemperature(r, g, b);
  // lux = tcs->calculateLux(r, g, b);
  return map(g, 0, 6200, 0, 1023);
}