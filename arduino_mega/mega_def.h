#ifndef MEGA_PINS_H
    #define MEGA_PINS_H

    // IMPORTAÇÃO DE BIBLIOTECAS
    #include <Wire.h>
    #include <Adafruit_TCS34725.h>
    #include <VL53L0X.h>
    #include <Ultrasonic.h>
    #include "MPU6050_6Axis_MotionApps612.h"
    #include <Servo.h>

    // MOTORES GRANDES
    #define MOTOR_EF   11 // verde escuro
    #define MOTOR_ET   10 // verde claro
    #define MOTOR_DF   8  // azul claro
    #define MOTOR_DT   9  // azul escuro

    // SERVOMOTORES
    #define SERVO_PA_GARRA_PIN 2
    #define SERVO_SUBIR_GARRA_PIN 3
    #define SERVO_ROTACIONAR_GARRA_PIN 4
    #define SERVO_CANCELA_DIREITO_PIN 5
    #define SERVO_CANCELA_ESQUERDO_PIN 6

    // PLACA DE SENSORES DE REFLETÂNCIA
    #define SE3_PIN    A8
    #define SE2_PIN    A9
    #define SE1_PIN    A15
    #define SE0_PIN    A14
    #define SD0_PIN    A13
    #define SD1_PIN    A12
    #define SD2_PIN    A10
    #define SD3_PIN    A11

    // SENSOR DE REFLETÂNCIA FRENTE
    #define SF_PIN     A7

    // BOTOES
    #define BTN_AREA_ESQ_PIN    22
    #define BTN_AREA_DIR_PIN    23
    #define BTN_PAREDE_ESQ_PIN  24
    #define BTN_PAREDE_DIR_PIN  25
    bool btnAreaEsq, btnAreaDir, btnParedeEsq, btnParedeDir;

    #define MPU6050_INTERRUPT_PIN 2

    // PINOS PARA LEDS RGB ("SETA" DO ROBÔ)
    #define LED_ESQ_RED_PIN   53
    #define LED_ESQ_GREEN_PIN 51
    #define LED_ESQ_BLUE_PIN  49
    #define LED_DIR_RED_PIN   52
    #define LED_DIR_GREEN_PIN 50
    #define LED_DIR_BLUE_PIN  48

    // CONSTANTES PARA CORES
    #define DESLIGADO  0, 0, 0
    #define VERMELHO   255, 0, 0
    #define VERDE      0, 255, 0
    #define AZUL       0, 0, 255
    #define ROXO       255, 0, 255
    #define BRANCO     255, 255, 255

    // CONSTANTES PARA LEDS
    #define AMBOS      true, true
    #define ESQ        true, false
    #define DIR        false, true

    // DEFINIÇÕES PARA MULTIPLEXADOR I2C
    #define TCAADDR 0x70
    #define CANAL_TCS_FRENTE  7
    #define CANAL_TCS_ESQ     6

    uint8_t rgbTcsEsq[3]; // lista de valores RGB do sensor TCS esquerdo
    uint8_t rgbTcsDir[3]; // lista de valores RGB do sensor TCS direito
    uint16_t rgbFrente[3]; // lista de valores RGB do sensor TCS da frente

    // OUTROS
    #define CONVERT_8B_DEC(vel) ((vel * 255) / 100)

#endif