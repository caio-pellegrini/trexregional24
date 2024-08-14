#define MOTOR_ESQ_F_PIN 11 // verde escuro
#define MOTOR_ESQ_T_PIN 10 // verde claro
#define MOTOR_DIR_F_PIN 8  // azul claro
#define MOTOR_DIR_T_PIN 9  // azul escuro

void setup()
{
    pinMode(MOTOR_ESQ_F_PIN, OUTPUT);
    pinMode(MOTOR_ESQ_T_PIN, OUTPUT);
    pinMode(MOTOR_DIR_F_PIN, OUTPUT);
    pinMode(MOTOR_DIR_T_PIN, OUTPUT);


    analogWrite(MOTOR_ESQ_F_PIN, 0);
    analogWrite(MOTOR_ESQ_T_PIN, 0);
    analogWrite(MOTOR_DIR_F_PIN, 0);
    analogWrite(MOTOR_DIR_T_PIN, 0);
}

void loop()
{

}