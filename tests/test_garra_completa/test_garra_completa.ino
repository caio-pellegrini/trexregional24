#include <Servo.h>

#define SERVO_PA_GARRA_PIN 44
#define SERVO_SUBIR_GARRA_PIN 46
#define SERVO_ROTACIONAR_GARRA_PIN 45
#define SERVO_CANCELA_DIREITO_PIN 12
#define SERVO_CANCELA_ESQUERDO_PIN 13

Servo servoPaGarra; // 180 fechada, 60 aberta
Servo servoSubirGarra; // 0 baixo, 160 cima
Servo servoRotacionarGarra; // 0 esq, 110 dir 55 padrao
Servo servoCancelaDireito; 
Servo servoCancelaEsquerdo; 

uint8_t posicaoServoPaGarra = 180, posicaoServoSubirGarra = 160, posicaoServoRotacionarGarra = 55;
uint8_t posicaoServoCancelaDireito = 0, posicaoServoCancelaEsquerdo = 0;

void setup()
{
  Serial.begin(9600);
  servoPaGarra.attach(SERVO_PA_GARRA_PIN, 700, 2000); // testar isso amanha
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  // servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);
  // servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);

  servoPaGarra.write(posicaoServoPaGarra);
  servoSubirGarra.write(posicaoServoSubirGarra);
  servoRotacionarGarra.write(posicaoServoRotacionarGarra);
  // servoCancelaDir.write(posicaoServoCancelaDir);
  // servoCancelaEsq.write(posicaoServoCancelaEsq);
  Serial.println("fim do setup");
}

void loop()
{
  movimentarServo(&servoPaGarra, 60, 180, 5);
  movimentarServo(&servoPaGarra, 180, 60, 5);

  movimentarServo(&servoSubirGarra, 0, 160, 25);
  movimentarServo(&servoSubirGarra, 160, 0, 25);

  movimentarServo(&servoRotacionarGarra, 0, 110, 40); // 55
  movimentarServo(&servoRotacionarGarra, 110, 0, 40);

}

void movimentarServo(Servo *motor, uint8_t posicaoInicial, uint8_t posicaoFinal, uint8_t velocidade)
{
  if (posicaoInicial > posicaoFinal)
  {
    for (int i = posicaoInicial; i >= posicaoFinal; i--)
    {
      Serial.println(i);
      motor->write(i);
      delay(velocidade);
    }
  }
  else
  {
    for (uint8_t i = posicaoInicial; i <= posicaoFinal; i++)
    {
      Serial.println(i);
      motor->write(i);
      delay(velocidade);
    }
  }
}