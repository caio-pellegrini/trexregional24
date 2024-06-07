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

    const uint8_t rgbE[3] = {53, 51, 49}; // Vermelho, Verde, Azul
    const uint8_t rgbD[3] = {52, 50, 48}; // Vermelho, Verde, Azul

    #define TCAADDR 0x70

    uint8_t rgbEsq[3]; // lista de valores RGB do sensor TCS esquerdo
    uint8_t rgbDir[3]; // lista de valores RGB do sensor TCS direito
    uint16_t rgbFrente[3];

#endif