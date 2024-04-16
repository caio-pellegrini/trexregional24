#include <Wire.h>
#include "Adafruit_TCS34725.h"

char strtcs[40];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);

  if (!tcs.begin()) {
    Serial.println("Sensor TCS34725 não encontrado. Verifique as conexões.");
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(void) {
  static char buffer[64];
  static int index = 0;

  while (Serial.available() > 0) {
    char comando = Serial.read();
    if (comando == '\n') {  // Verifica se é o final do comando
      if (strstr(buffer, "caio") != NULL) {
        uint16_t r, g, b, c;
        tcs.getRawData(&r, &g, &b, &c);
        r = map(r, 0, 6000, 0, 255);
        g = map(g, 0, 6000, 0, 255);
        b = map(b, 0, 6000, 0, 255);

        snprintf(strtcs, sizeof(strtcs), "R:%d,G:%d,B:%d", r, g, b);
        Serial.println(strtcs);  // Envia os dados do sensor após receber o comando
      }
      memset(buffer, 0, sizeof(buffer));  // Limpa o buffer e reseta o índice após processar o comando
      index = 0;
    } else {
      if (index < 63) {
        buffer[index++] = comando;
      } else {
        // Buffer cheio sem encontrar '\n'
        memset(buffer, 0, sizeof(buffer));  // Limpa o buffer
        index = 0;                          // Reseta o índice
      }
    }
  }
}
