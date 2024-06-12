#include <Servo.h>

Servo servoMotor;  // Cria um objeto Servo para controlar o servo motor
int pinServo = 9;  // Escolha o pino ao qual o servo motor está conectado

void setup() {
  servoMotor.attach(pinServo);  // Anexa o servo motor ao pino especificado
  Serial.begin(9600);  // Inicia a comunicação serial para debug
}

void loop() {
  int angulo = servoMotor.read();  // Lê o ângulo atual do servo motor
  Serial.println("Angulo atual: " + String(angulo));  // Exibe o ângulo atual no monitor serial
  delay(100);  
}