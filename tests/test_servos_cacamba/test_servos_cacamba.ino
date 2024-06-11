#include <Servo.h>

Servo servoD;
Servo servoE;

#define SERVOD_PIN 44
#define SERVOE_PIN 45
#define DELAY_TIME 15

int pos_d = 0;            // variable to store the servo position
int start_position_d = 0; // variable to store start positon
int end_position_d = 90; // variable to store end positon

int pos_e = 0;            // variable to store the servo position
int start_position_e = 0; // variable to store start positon
int end_position_e = 90; // variable to store end positon

void setup()
{
    servo.attach(SERVOD_PIN);
    servo.attach(SERVOE_PIN);
}

void loop()
{
    // abrir direito
    for (pos_d = start_position_d; pos <= end_position_d; pos_d += 1)
    {
        servo.write(pos_d);
        delay(DELAY_TIME);
    }

    // abrir esquerdo
    for (pos_e = start_position_e; pos <= end_position_e; pos_e += 1)
    {
        servo.write(pos_e);
        delay(DELAY_TIME);
    }

    // fechar direito
    for (pos_d = end_position_d; pos >= start_position_d; pos_d -= 1)
    {
        servo.write(pos_d);
        delay(DELAY_TIME);
    }

    // fechar esquerdo
    for (pos_e = end_position_e; pos >= start_position_e; pos_e -= 1)
    {
        servo.write(pos_e);
        delay(DELAY_TIME);
    }
}
