#include <Wire.h>
#include <VL53L0X.h>

// Declare dois objetos sensor
VL53L0X sensor1;
VL53L0X sensor2;

// Define os pinos XSHUT para cada sensor
#define SENSOR1_XSHUT 14
#define SENSOR2_XSHUT 19

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(SENSOR1_XSHUT, OUTPUT);
  pinMode(SENSOR2_XSHUT, OUTPUT);

  // Primeiro sensor
  digitalWrite(SENSOR1_XSHUT, LOW);  // Desliga o sensor 1
  digitalWrite(SENSOR2_XSHUT, LOW);  // Desliga o sensor 2
  delay(50);

  digitalWrite(SENSOR1_XSHUT, HIGH); // Liga o sensor 1
  delay(50);
  sensor1.setTimeout(500);
  sensor1.init();
  sensor1.setAddress(0x30);          // Novo endereço para o sensor 1

  // Segundo sensor
  digitalWrite(SENSOR2_XSHUT, HIGH); // Liga o sensor 2
  delay(50);
  sensor2.setTimeout(500);
  sensor2.init();
  sensor2.setAddress(0x31);          // Novo endereço para o sensor 2
}

void loop() {
  Serial.print("Sensor 1: ");
  Serial.print(sensor1.readRangeSingleMillimeters());
  if (sensor1.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.print("\tSensor 2: ");
  Serial.print(sensor2.readRangeSingleMillimeters());
  if (sensor2.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
}
