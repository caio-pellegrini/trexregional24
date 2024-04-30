#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Cria uma instância do sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup() {
  Serial.begin(9600);

  // Inicializa o sensor TCS34725
  if (tcs.begin()) {
    Serial.println("Sensor encontrado!");
  } else {
    Serial.println("Não foi possível encontrar o sensor.");
    while (1); // Para a execução se o sensor não for encontrado
  }
}

void loop() {
  uint16_t clear, red, green, blue;

  // Lê os valores de cor do sensor
  tcs.getRawData(&red, &green, &blue, &clear);

  // Calcula as proporções
  float redRatio = green / (float)red;
  float blueRatio = green / (float)blue;

  Serial.print("redRatio: ");
  Serial.print(redRatio);
    Serial.print(" blueRatio: ");
    Serial.println(blueRatio);

  // Verifica se a cor é verde
  if (redRatio > 1.5 && blueRatio > 1.5) {
    Serial.println("Verde detectado");
  }
  // Verifica se a cor é branca
  else if (redRatio > 0.8 && redRatio < 1.2 && blueRatio > 0.8 && blueRatio < 1.2) {
    Serial.println("Branco detectado");
  } else {
    Serial.println("Outra cor detectada");
  }

  delay(1000); // Aguarda um segundo antes da próxima leitura
}
