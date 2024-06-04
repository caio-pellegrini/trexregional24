#include <Wire.h>
#include <VL53L0X.h>
#include "Adafruit_TCS34725.h"

VL53L0X sensor1;

unsigned int dist1;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
int x = 14;
int tcsa = 13;

void setup()
{

  Serial.begin (9600);
  Wire.begin();
  pinMode(x, OUTPUT);//-> VL53L0x XSHUT pin (sensor1)
  pinMode(tcsa, OUTPUT);//-> TCS34725 Vin pin
  digitalWrite(x, LOW);  delay(50);
  digitalWrite(tcsa, LOW);  delay(50);
  delay(10);
  pinMode(x, INPUT);
  delay(1000);
  scan();
  sensor1.setTimeout(500);
  if (!sensor1.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  sensor1.setAddress(0x31);
  delay(10);
  digitalWrite(tcsa, HIGH);
  delay(10);
  tcs.begin();
}

void loop() {
  getDist();
  delay(500);
  RGB();
  delay(500);
  scan();
  
}
void getDist() {
  dist1 = sensor1.readRangeContinuousMillimeters();
  Serial.print("dist1= ");  Serial.print(dist1);
}

void RGB() {
  int red, green, blue, clear;
  //tcs.enable();
  tcs.getRawData(&red, &green, &blue, &clear);
  Serial.print("R:\t"); Serial.print(int(red));
  Serial.print("\tG:\t"); Serial.print(int(green));
  Serial.print("\tB:\t"); Serial.print(int(blue));
  Serial.print("\n");
  Serial.print("\n");
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

  delay(2000);           // wait 5 seconds for next scan
}