#include "Adafruit_VL53L0X.h"
#include "Adafruit_TCS34725.h"

// Define os pinos de shutdown
#define VL53L0X_XSHUT 14

// Cria objetos para os sensores
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);

void setup() {
  Serial.begin(9600);

  // Inicializa o pino de shutdown
  pinMode(VL53L0X_XSHUT, OUTPUT);
  digitalWrite(VL53L0X_XSHUT, LOW); // Mantém o VL53L0X desligado inicialmente

  // Inicializa o TCS34725
  if (tcs.begin()) {
    Serial.println("Found TCS34725");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // Se falhar, para o código aqui
  }

  // Inicializa o VL53L0X
  digitalWrite(VL53L0X_XSHUT, HIGH); // Liga o VL53L0X
  delay(10); // Aguarda o sensor inicializar

  if (!lox.begin(0x2B)) { // Inicia o VL53L0X com endereço 0x30
    Serial.println(F("Failed to boot VL53L0X"));
    while (1); // Se falhar, para o código aqui
  }

  Serial.println("VL53L0X and TCS34725 initialized successfully");
}

void loop() {
  // Código para ler dados dos sensores e utilizá-los
  // Exemplo de leitura do VL53L0X
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) { // Verifica se a medição é válida
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter); // Imprime a distância medida em milímetros
  } else {
    Serial.println(" out of range "); // Se a medição não for válida, imprime "fora de alcance"
  }

  // Exemplo de leitura do TCS34725
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  Serial.print("R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.print(b);
  Serial.print(" C: "); Serial.println(c);

  delay(1000); // Aguarda 1 segundo antes da próxima leitura
}
