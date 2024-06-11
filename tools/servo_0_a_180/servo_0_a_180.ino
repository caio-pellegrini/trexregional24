#include <Servo.h>

Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

#define SERVO_PIN 12
#define DELAY_TIME 15

int pos = 0;    // variable to store the servo position
int start_position = 0;    // variable to store start positon
int end_position = 180;    // variable to store end positon

void setup() {
  servo.attach(SERVO_PIN);  // attaches the servo on pin to the servo object
}

void loop() {
  for (pos = start_position; pos <= end_position; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(DELAY_TIME);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = end_position; pos >= start_position; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(DELAY_TIME);                       // waits 15 ms for the servo to reach the position
  }
}
