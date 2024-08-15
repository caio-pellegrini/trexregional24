#include <Servo.h>

#define SERVO_PIN 45
#define POSITION 90

Servo servo;

void setup() {
    servo.attach(SERVO_PIN);
    servo.write(POSITION); // Set servo position to zero
}

void loop() {
    // Your code here
}