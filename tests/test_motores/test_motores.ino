#define MOTOR_EF 11 // verde escuro
#define MOTOR_ET 10 // verde claro
#define MOTOR_DF 8  // azul claro
#define MOTOR_DT 9  // azul escuro

void setup()
{
    pinMode(MOTOR_EF, OUTPUT);
    pinMode(MOTOR_ET, OUTPUT);
    pinMode(MOTOR_DF, OUTPUT);
    pinMode(MOTOR_DT, OUTPUT);


    analogWrite(MOTOR_EF, 255);
    analogWrite(MOTOR_ET, 0);
    analogWrite(MOTOR_DF, 255);
    analogWrite(MOTOR_DT, 0);
}

void loop()
{

}