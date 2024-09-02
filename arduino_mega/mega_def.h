#define DEBUG 0

#define DEBUG_REFL 1
#define DEBUG_TCS_AMBOS 1
#define DEBUG_TCS_FRENTE 0
#define DEBUG_GIROSCOPIO 0
#define DEBUG_LASER_FRENTE 0
#define DEBUG_LASER_GARRA 0
#define DEBUG_ULTRA 0
#define DEBUG_BOTOES 0

#define LUZ_PRETO 205 // acima é preto    200
#define LUZ_BRANCO 152 // abaixo é branco  147
#define LUZ_FRENTE 90  // abaixo é branco e acima é preto   120

#define CORTE_VERDE_ESQ 80   // abaixo disso é verde -- 120 branco 50 verde
#define CORTE_VERDE_DIR 80   // 130 branco 30 verde
#define CORTE_VERMELHO_CRUZ 80 // 100

// USE VALORES DE 0 A 100
#define VEL_MOTOR_FRENTE 50
#define VEL_MOTOR_TRAS 40
#define VEL_MOTOR_CURVA 64 // 59
#define VEL_MOTOR_SEG_FRENTE 36 // 38
#define VEL_MOTOR_SEG_MAX 75
#define VEL_MOTOR_SEG_MIN 70 // 70

#define TEMPO_MOVER_ANTES_ANALISAR_VERDE moverFrentePor(120); // 98
#define TEMPO_MOVER_ANTES_CRUZ 200 // 300

#define DIST_LASER_GARRA_VIT 45
#define DIST_OBSTACULO 70

#define CORTE_ULTRA_SALA_RESGATE 7
#define INCLINACAO 7
#define INCLI_RAMPA 23

// ---------------

#define SERVO_PA_GARRA_POS_INICIAL 110
#define SERVO_SUBIR_GARRA_POS_INICIAL 132 // 132
#define SERVO_ROTACIONAR_GARRA_POS_INICIAL 90
#define SERVO_CANCELA_ESQ_POS_INICIAL 75
#define SERVO_CANCELA_DIR_POS_INICIAL 170

bool btnAreaEsq = false, btnAreaDir = false;
bool btnParedeEsq = false, btnParedeDir = false;
bool btnVitima = false;
bool btnFcEsq = false, btnFcDir = false;


// CONSTANTES PARA CORES
#define DESLIGADO 0, 0, 0
#define BRANCO 255, 255, 255
#define VERMELHO 255, 0, 0
#define VERDE 0, 255, 0
#define AZUL 0, 0, 255
#define ROXO 255, 0, 255
#define AMARELO 255, 255, 0
#define CIANO 0, 255, 255

// CONSTANTES PARA LEDS
#define AMBOS true, true
#define ESQ true, false
#define DIR false, true

// DEFINIÇÕES PARA MULTIPLEXADOR I2C
#define TCA_ENDERECO 0x70
#define CANAL_TCS_FRENTE 7
#define CANAL_TCS_ESQ 6

#define LASER_FRENTE_ENDERECO 0x30
#define LASER_GARRA_ENDERECO 0x31


uint8_t rgbTcsEsq[3];     // lista de valores RGB do sensor TCS esquerdo
uint8_t rgbTcsDir[3];     // lista de valores RGB do sensor TCS direito
uint16_t rgbTcsFrente[3]; // lista de valores RGB do sensor TCS da frente

// OUTROS
#define CONVERT_8B_DEC(vel) ((vel * 255) / 100)
#define TCS_SATURACAO_MAX 1500 // 4000 PARA 614ms

// VARIÁVEIS E CLASSES

uint8_t se3, se2, se1, se0, sd0, sd1, sd2, sd3;
uint8_t sm, sf;

Adafruit_TCS34725 tcsFrente =
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcsEsq =
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);

VL53L0X_mod laserFrente;
uint16_t distanciaLaserFrente;

VL53L0X_mod laserGarra;
uint16_t distanciaLaserGarra;

Servo servoPaGarra;
Servo servoSubirGarra;
Servo servoRotacionarGarra;
Servo servoCancelaEsq;
Servo servoCancelaDir;

Ultrasonic ultrasonicEsq(ULTRA_ESQ_TRIG_PIN, ULTRA_ESQ_ECHO_PIN);
Ultrasonic ultrasonicDir(ULTRA_DIR_TRIG_PIN, ULTRA_DIR_ECHO_PIN);
int distanciaUltraEsq, distanciaUltraDir;

uint8_t contadorGap = 0;

// Variáveis e definições para o MPU-6050 com DMP
MPU6050 mpu;
uint8_t mpuIntStatus;
uint16_t fifoCount;
uint16_t packetSize; // expected DMP packet size (default is 42 bytes)
uint8_t devStatus; // return status after each device operation (0 = success, !0
                   // = error)
bool dmpReady = false;  // set true if DMP init was successful
uint8_t fifoBuffer[64]; // FIFO storage buffer
// orientation/motion vars
Quaternion q;        // [w, x, y, z]         quaternion container
VectorFloat gravity; // [x, y, z]            gravity vector
float ypr[3]; // [yaw, pitch, roll] container and gravity vector
float yaw, pitch, roll;
float initialYaw;
volatile bool mpuInterrupt = false;
// mpu antigo
int16_t ax, ay, az;
int16_t gx, gy, gz;

float curvaYaw;
uint8_t contadorCurva90 = 0;

void dmpDataReady() { mpuInterrupt = true; }

int contUltra = 0;


#define ULTRA_ENTRADA_SALA 1
#define RAMPA 1
#define GANGORRA 0
#define OBSTACULO 1
#define GAP 1
#define MCD 0
#define MCE 0
#define GRAUS90 1

// em teste
#define LOMBADA 0

// talvez tenha, talvez nao
#define RAMPA_SALA_RESGATE 0