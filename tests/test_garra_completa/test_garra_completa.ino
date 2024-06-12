#include <Servo.h>

#define SERVO_PA_GARRA_PIN 2
#define SERVO_SUBIR_GARRA_PIN 3
#define SERVO_ROTACIONAR_GARRA_PIN 4
#define SERVO_CANCELA_DIREITO_PIN 5
#define SERVO_CANCELA_ESQUERDO_PIN 6

Servo servoPaGarra;
Servo servoSubirGarra;
Servo servoRotacionarGarra;
Servo servoCancelaDireito;
Servo servoCancelaEsquerdo;

uint8_t posicaoServoPaGarra = 90, posicaoServoSubirGarra = 0, posicaoServoRotacionarGarra = 0;
uint8_t posicaoServoCancelaDireito = 0, posicaoServoCancelaEsquerdo = 0;

void setup()
{
  pinMode(SERVO_PA_GARRA_PIN, OUTPUT);
  pinMode(SERVO_SUBIR_GARRA_PIN, OUTPUT);
  pinMode(SERVO_ROTACIONAR_GARRA_PIN, OUTPUT);
  pinMode(SERVO_CANCELA_DIREITO_PIN, OUTPUT);
  pinMode(SERVO_CANCELA_ESQUERDO_PIN, OUTPUT);

  servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaDireito.attach(SERVO_CANCELA_DIREITO_PIN);
  servoCancelaEsquerdo.attach(SERVO_CANCELA_ESQUERDO_PIN);

  servoPaGarra.write(posicaoServoPaGarra);
  servoSubirGarra.write(posicaoServoSubirGarra);
  servoRotacionarGarra.write(posicaoServoRotacionarGarra);
  servoCancelaDireito.write(posicaoServoCancelaDireito);
  servoCancelaEsquerdo.write(posicaoServoCancelaEsquerdo);
}

void loop()
{
  movimentarServo(ServoPaGarra, 50, 90, 180);

  movimentarServo(ServoPaGarra, 50, 180, 90);
}

void movimentarServo(Servo *servo, uint8_t velocidade, uint8_t posicaoInicial, uint8_t posicaoFinal)
{
  if (posicaoInicial > posicaoFinal)
  {
    for (uint8_t i = posicaoInicial; i >= posicaoFinal; i--)
    {
      servo->write(i);
      delay(velocidade);
    }
  }
  else
  {
    for (uint8_t i = posicaoInicial; i <= posicaoFinal; i++)
    {
      servo->write(i);
      delay(velocidade);
    }
  }
}