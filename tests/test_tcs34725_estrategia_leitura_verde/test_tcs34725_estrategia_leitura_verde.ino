#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Now we're ready to get readings!
}

void loop(void) {
  uint16_t r1, g1, b1, c1;
  tcs.getRawData(&r1, &g1, &b1, &c1);

  Serial.print("RAW VALUES:   ");
  Serial.print("R: "); Serial.print(r1); Serial.print(" ");
  Serial.print("G: "); Serial.print(g1); Serial.print(" ");
  Serial.print("B: "); Serial.print(b1); Serial.print(" ");
  Serial.print("C: "); Serial.print(c1, DEC); Serial.print(" ");
  Serial.print("    ---    ");


  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  r = map(r, 0, 4000, 0, 255);
  g = map(g, 0, 4000, 0, 255);
  b = map(b, 0, 4000, 0, 255);

  Serial.print("ARRED 4000:   ");
  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print(g); Serial.print(" ");
  Serial.print("B: "); Serial.print(b); Serial.print(" ");
  Serial.print("    ---   ");

  uint16_t r2, g2, b2, c2, soma;
  tcs.getRawData(&r2, &g2, &b2, &c2);
  soma = r2 + g2 + b2;
  r2 = map(r2, 0, soma, 0, 255);
  g2 = map(g2, 0, soma, 0, 255);
  b2 = map(b2, 0, soma, 0, 255);

  Serial.print("ARRED SOMA:   ");
  Serial.print("R: "); Serial.print(r2); Serial.print(" ");
  Serial.print("G: "); Serial.print(g2); Serial.print(" ");
  Serial.print("B: "); Serial.print(b2); Serial.print(" ");
  Serial.println("");
  
  delay(4000);


}

void lerSensorCor(Adafruit_TCS34725 *tcs) {
  uint16_t r, g, b, c, colorTemp, lux;

  tcs->getRawData(&r, &g, &b, &c);
  // colorTemp = tcs->calculateColorTemperature(r, g, b);
  // lux = tcs->calculateLux(r, g, b);
  r = map(r, 0, 6000, 0, 255);
  g = map(g, 0, 6000, 0, 255);
  b = map(b, 0, 6000, 0, 255);

  Serial.print("TCS ESQ: R:");
  Serial.print(r);
  Serial.print(",G:");
  Serial.print(g);
  Serial.print(",B:");
  Serial.print(b);
}