#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <VL53L0X.h>


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
VL53L0X sensor1;
unsigned int dist1;
  int laser_xshut = 14;
  int tcsa = 42;

  // Endereço do multiplexador
#define TCA9548A_ADDRESS 0x70

// Função para selecionar o canal do multiplexador
void tcaSelect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << i);
  Wire.endTransmission();
}


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  pinMode(laser_xshut, OUTPUT); // Configura o pino XSHUT do VL53L0X como saída
  pinMode(tcsa, OUTPUT); // Configura o pino Vin do TCS34725 como saída
  
  digitalWrite(laser_xshut, LOW); // Desliga o VL53L0X
  delay(50);
  digitalWrite(tcsa, LOW); // Desliga o TCS34725
  delay(50);
  
  // Esta linha configura o pino XSHUT como entrada, possivelmente para evitar interferência
  // ou permitir que o sensor assuma o controle do pino
  //pinMode(laser_xshut, INPUT); // ??
  //delay(1000);
  
  // scan(); // Escaneia dispositivos I2C
  
  // Configura o sensor VL53L0X
  // sensor1.setTimeout(500);
  digitalWrite(laser_xshut, HIGH); // Habilita o sensor puxando o pino XSHUT para alto
  delay(10);

  if (!sensor1.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  
  //sensor1.setAddress(0x31); // Define o endereço I2C do sensor
  delay(10);
  // sensor1.startContinuous(); // Inicia leituras contínuas
  
  digitalWrite(tcsa, HIGH); // Liga o sensor TCS34725
  delay(10);
  
  tcaSelect(6); // Supondo que o TCS34725 está no canal 0 do multiplexador
  if (!tcs.begin()) {
    Serial.println("Failed to initialize TCS34725 sensor!");
    while (1) {}
  }
}

void loop() {
  getDist();
  RGB();
  delay(500);
  scan();
}

void getDist() {
  dist1 = sensor1.readRangeSingleMillimeters();
  Serial.print("dist1= "); Serial.print(dist1);
}

void RGB() {
  uint16_t r, g, b, c;
  tcaSelect(6); // Supondo que o TCS34725 está no canal 0 do multiplexador
  tcs.getRawData(&r, &g, &b, &c);
  Serial.print(" R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.print(b);
  Serial.println();
}

void scan() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(2000); // wait 2 seconds for next scan
}
