#include <Servo.h>

#define SERVO_PIN 46

Servo servo;

void setup() {
    servo.attach(SERVO_PIN);
    servo.write(0); // Set servo position to zero
}

void loop() {
    // Your code here
}