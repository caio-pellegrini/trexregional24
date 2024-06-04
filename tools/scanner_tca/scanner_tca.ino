#include <Wire.h>

// Endereço do multiplexador I2C TCA9548A
#define TCA9548A_ADDRESS 0x70

// Função para selecionar um canal do multiplexador
void tcaSelect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // Espera pela conexão com o monitor serial

  Wire.begin();

  Serial.println("TCA9548A Scanner");
}

void loop() {
  for (uint8_t i = 0; i < 8; i++) {
    tcaSelect(i);
    Serial.print("TCA9548A Channel: ");
    Serial.println(i);

    // Scan I2C devices on the selected channel
    for (uint8_t address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      uint8_t error = Wire.endTransmission();

      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address < 16) Serial.print("0");
        Serial.print(address, HEX);
        Serial.println(" !");
      } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16) Serial.print("0");
        Serial.println(address, HEX);
      }
    }
    Serial.println(); // Nova linha para separação dos resultados dos canais
  }
  delay(5000); // Espera 5 segundos antes de escanear novamente
}
