#ifndef MEGA_PINS_H
    #define MEGA_PINS_H

    // IMPORTAÇÃO DE BIBLIOTECAS
    #include <Wire.h>
    #include <Adafruit_TCS34725.h>
    #include <VL53L0X_mod.h>
    #include <Ultrasonic.h>
    #include "MPU6050_6Axis_MotionApps612.h"
    #include <Servo.h>

    // MOTORES GRANDES
    #define MOTOR_ESQ_F_PIN   11 // verde escuro
    #define MOTOR_ESQ_T_PIN   10 // verde claro
    #define MOTOR_DIR_F_PIN   8  // azul claro
    #define MOTOR_DIR_T_PIN   9  // azul escuro

    // SERVOMOTORES
    #define SERVO_PA_GARRA_PIN 44
    #define SERVO_SUBIR_GARRA_PIN 46
    #define SERVO_ROTACIONAR_GARRA_PIN 45
    #define SERVO_CANCELA_ESQ_PIN 13
    #define SERVO_CANCELA_DIR_PIN 12

    #define SERVO_PA_GARRA_POS_INICIAL 110
    #define SERVO_SUBIR_GARRA_POS_INICIAL 125
    #define SERVO_ROTACIONAR_GARRA_POS_INICIAL 55
    #define SERVO_CANCELA_ESQ_POS_INICIAL 3
    #define SERVO_CANCELA_DIR_POS_INICIAL 99

    // PLACA DE SENSORES DE REFLETÂNCIA
    #define SE3_PIN    A9
    #define SE2_PIN    A9
    #define SE1_PIN    A10
    #define SE0_PIN    A11
    #define SD0_PIN    A13
    #define SD1_PIN    A14
    #define SD2_PIN    A15
    #define SD3_PIN    A15

    // #define SE3_PIN    A8
    // #define SE2_PIN    A9
    // #define SE1_PIN    A15
    // #define SE0_PIN    A14
    // #define SD0_PIN    A13
    // #define SD1_PIN    A12
    // #define SD2_PIN    A10
    // #define SD3_PIN    A11

    // SENSOR DE REFLETÂNCIA FRENTE
    #define SF_PIN     A12

    // BOTOES
    #define BTN_AREA_ESQ_PIN    24
    #define BTN_AREA_DIR_PIN    28
    #define BTN_PAREDE_ESQ_PIN  22
    #define BTN_PAREDE_DIR_PIN  26
    #define BTN_VITIMA_PIN      33
    bool btnAreaEsq = false, btnAreaDir = false, btnParedeEsq = false, btnParedeDir = false, btnVitima = false;

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
    #define AMARELO    255, 255, 0
    #define ROXO       255, 0, 255

    // CONSTANTES PARA LEDS
    #define AMBOS      true, true
    #define ESQ        true, false
    #define DIR        false, true

    // DEFINIÇÕES PARA MULTIPLEXADOR I2C
    #define TCA_ENDERECO       0x70
    #define CANAL_TCS_FRENTE   7
    #define CANAL_TCS_ESQ      6

    #define LASER_FRENTE_ENDERECO 0x30
    #define LASER_FRENTE_XSHUT 14
    #define LASER_GARRA_ENDERECO 0x31
    #define LASER_GARRA_XSHUT  19

    uint8_t rgbTcsEsq[3];  // lista de valores RGB do sensor TCS esquerdo
    uint8_t rgbTcsDir[3];  // lista de valores RGB do sensor TCS direito
    uint16_t rgbTcsFrente[3]; // lista de valores RGB do sensor TCS da frente

    // OUTROS
    #define CONVERT_8B_DEC(vel) ((vel * 255) / 100)
    #define TCS_SATURACAO_MAX 1500  // 4000 PARA 614ms


    // VARIÁVEIS E CLASSES

    uint8_t se3, se2, se1, se0, sd0, sd1, sd2, sd3;
    uint8_t sf;

    Adafruit_TCS34725 tcsFrente = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
    Adafruit_TCS34725 tcsEsq = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);

    VL53L0X_mod laserFrente;
    uint16_t distanciaLaserFrente;

    VL53L0X_mod laserGarra;
    uint16_t distanciaLaserGarra;

    Servo servoPaGarra;
    Servo servoSubirGarra;
    Servo servoRotacionarGarra;
    Servo servoCancelaEsq;
    Servo servoCancelaDir;

    Ultrasonic ultrasonicEsq(7, 6);
    Ultrasonic ultrasonicDir(5, 4);
    int distanciaUltraEsq, distanciaUltraDir;

    uint8_t contadorGap = 0;

    // Variáveis e definições para o MPU-6050 com DMP
    MPU6050 mpu;
    uint8_t mpuIntStatus;
    uint16_t fifoCount;
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t fifoBuffer[64]; // FIFO storage buffer
    // orientation/motion vars
    Quaternion q;        // [w, x, y, z]         quaternion container
    VectorFloat gravity; // [x, y, z]            gravity vector
    float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    float yaw, pitch, roll;
    float initialYaw;
    volatile bool mpuInterrupt = false;
    // mpu antigo
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    void dmpDataReady()
    {
      mpuInterrupt = true;
    }
    
    #define FITA_PRATEADA 0
    #define RAMPA_SALA_RESGATE 1
    #define RAMPA 0
    #define GANGORRA 1
    #define OBSTACULO 1
    #define GAP 0

#endif