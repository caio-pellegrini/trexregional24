#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <VL53L0X.h>

// Definição dos sensores
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
VL53L0X sensorVL53L0X;

// Endereço do multiplexador
#define TCA9548A_ADDRESS 0x70

// Função para selecionar o canal do multiplexador
void tcaSelect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << i);
  Wire.endTransmission();
}

// Função para desativar todos os canais do multiplexador
void tcaDeselect() {
  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Inicializar o sensor VL53L0X diretamente (sem multiplexador)
  Serial.println("Inicializando sensor VL53L0X...");
  if (!sensorVL53L0X.init()) {
    Serial.println("Falha ao iniciar o sensor VL53L0X! Verifique a conexão.");
    while (1);
  } else {
    Serial.println("Sensor VL53L0X iniciado com sucesso!");
  }
  
  // Aguardar um pouco para garantir inicialização completa
  delay(500);
  
  // Selecionar o canal do multiplexador para o sensor TCS34725
  Serial.println("Selecionando canal para TCS34725...");
  tcaSelect(6); // Supondo que o TCS34725 está no canal 0 do multiplexador
  
  // Inicializar o sensor TCS34725 através do multiplexador
  Serial.println("Inicializando sensor TCS34725...");
  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 iniciado com sucesso!");
  } else {
    Serial.println("Falha ao iniciar o sensor TCS34725. Verifique a conexão!");
    while (1);
  }

  // Desativar o canal do multiplexador após a inicialização do TCS34725
  tcaDeselect();
}

void loop() {
  // Selecionar o canal do multiplexador para o sensor TCS34725
  Serial.println("Selecionando canal para TCS34725...");
  tcaSelect(6); // Supondo que o TCS34725 está no canal 0 do multiplexador
  
  // Ler dados do sensor TCS34725
  Serial.println("Lendo dados do sensor TCS34725...");
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  Serial.print("R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.print(b);
  Serial.print(" C: "); Serial.println(c);

  // Desativar o canal do multiplexador após a leitura do TCS34725
  tcaDeselect();
  
  // Ler dados do sensor VL53L0X
  Serial.println("Lendo dados do sensor VL53L0X...");
  uint16_t distance = sensorVL53L0X.readRangeSingleMillimeters();
  if (sensorVL53L0X.timeoutOccurred()) {
    Serial.println(" TIMEOUT");
  } else {
    Serial.print("Distância: ");
    Serial.print(distance);
    Serial.println(" mm");
  }

  delay(1000); // Atraso para leitura
}
