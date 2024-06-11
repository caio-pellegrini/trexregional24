#ifndef MEGA_PINS_H

    #define MEGA_PINS_H

    // MOTORES
    #define MOTOR_EF   11 // verde escuro
    #define MOTOR_ET   10 // verde claro
    #define MOTOR_DF   8  // azul claro
    #define MOTOR_DT   9  // azul escuro

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

    #define MPU6050_INTERRUPT_PIN 2

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


    #define LED_ESQ_RED_PIN   53
    #define LED_ESQ_GREEN_PIN 51
    #define LED_ESQ_BLUE_PIN  49
    #define LED_DIR_RED_PIN   52
    #define LED_DIR_GREEN_PIN 50
    #define LED_DIR_BLUE_PIN  48

    // DEFINIÇÕES PARA MULTIPLEXADOR I2C
    #define TCAADDR 0x70
    #define CANAL_TCS_FRENTE  7
    #define CANAL_TCS_ESQ  6

    uint8_t rgbEsq[3]; // lista de valores RGB do sensor TCS esquerdo
    uint8_t rgbDir[3]; // lista de valores RGB do sensor TCS direito
    uint16_t rgbFrente[3]; // lista de valores RGB do sensor TCS da frente

#endif