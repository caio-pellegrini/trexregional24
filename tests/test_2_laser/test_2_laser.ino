#include <Wire.h>
#include <VL53L0X.h>

// Declare dois objetos sensor
VL53L0X laserFrente;
VL53L0X laserGarra;

// Define os pinos XSHUT para cada sensor
#define LASER_FRENTE_XSHUT 14
#define LASER_GARRA_XSHUT 19

void setup() {
    Serial.begin(9600);
    Wire.begin();

    pinMode(LASER_FRENTE_XSHUT, OUTPUT);
    pinMode(LASER_GARRA_XSHUT, OUTPUT);

    digitalWrite(LASER_FRENTE_XSHUT, LOW);
    digitalWrite(LASER_GARRA_XSHUT, LOW);
    delay(50);

    digitalWrite(LASER_FRENTE_XSHUT, HIGH);
    delay(50);
    laserFrente.setTimeout(500);
    laserFrente.init();
    laserFrente.setAddress(0x30);

    digitalWrite(LASER_GARRA_XSHUT, HIGH);
    delay(50);
    laserGarra.setTimeout(500);
    laserGarra.init();
    laserGarra.setAddress(0x31);
}

void loop() {
  Serial.print("Sensor 1: ");
  Serial.print(laserFrente.readRangeSingleMillimeters());
  if (laserFrente.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.print("\tSensor 2: ");
  Serial.print(laserGarra.readRangeSingleMillimeters());
  if (laserGarra.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
}
