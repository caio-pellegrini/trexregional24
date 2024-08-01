#include <Servo.h>

Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

#define SERVO_PIN 13
#define DELAY_TIME 5

int pos = 0;    // variable to store the servo position
int start_position = 75;    // variable to store start positon
int end_position = 170;    // variable to store end positon

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);  // attaches the servo on pin to the servo object
}

void loop() {
  for (pos = start_position; pos <= end_position; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);
    delay(DELAY_TIME);
    Serial.println(pos);
  }
  delay(1000);

  for (pos = end_position; pos >= start_position; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);
    delay(DELAY_TIME);
    Serial.println(pos);
  }
  delay(1000);
}
