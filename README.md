# T-Rex Regional 2024 🤖

[![Platform](https://img.shields.io/badge/platform-Arduino-00979D.svg)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/language-C%2FC%2B%2B-00599C.svg)](https://isocpp.org/)

> Firmware embarcado para robô autônomo de resgate - OBR (Olimpíada Brasileira de Robótica) - Modalidade RoboCup Rescue Line

Sistema de controle completo para navegação autônoma em arenas de resgate, desenvolvido para a OBR. Implementa lógica de controle em tempo real usando máquinas de estados, integração de múltiplos sensores e algoritmos de decisão para missões de busca e resgate.

## 🏆 Conquistas

- 🎯 **Classificação para Etapa Nacional** - Primeira vez em 10 anos
- 🏅 **Prêmio Nacional de Melhor Design** - 2023
- 🥈 **Medalha de Prata** - Regional 2024

## 🎯 Sobre o Projeto

Firmware desenvolvido para controlar um robô autônomo capaz de:

- **Seguir linha preta** com alta precisão usando 7 sensores QTR
- **Detectar e desviar de obstáculos** com sensores laser VL53L0X
- **Navegar por rampas e gangorras** com controle via giroscópio MPU6050
- **Identificar e transpor gaps** (descontinuidades na linha)
- **Resgatar vítimas** com sistema de garra robotizada de 5 servomotores
- **Distinguir vítimas vivas e mortas** através de sensores tácteis
- **Reconhecer áreas de entrega** por cores usando sensores TCS34725 RGB

## 🏗️ Arquitetura do Sistema

O projeto utiliza **arquitetura dual-microcontrolador**:

```
┌─────────────────────────────────────────────────────┐
│              ARDUINO MEGA 2560 (Principal)          │
│  • Controle da máquina de estados                   │
│  • Seguidor de linha (7x QTR)                       │
│  • Motores DC via PWM                               │
│  • Sensores Laser VL53L0X (2x)                      │
│  • Sensores Ultrassônicos HC-SR04 (2x)              │
│  • Giroscópio MPU6050 com DMP                       │
│  • Servomotores (5x)                                │
│  • Sensor TCS34725 (esquerdo)                       │
│  • LEDs RGB de status                               │
└───────────────────┬─────────────────────────────────┘
                    │ Serial 9600 baud
┌───────────────────┴─────────────────────────────────┐
│              ARDUINO NANO (Auxiliar)                │
│  • Sensor TCS34725 (direito)                        │
│  • Processamento RGB                                │
└─────────────────────────────────────────────────────┘
```

**Comunicação Serial:**
- Comando: "caio\n" 
- Resposta: "R:xxx,G:xxx,B:xxx" (valores 0-255)

## ⚙️ Recursos Técnicos

### 1. Seguidor de Linha Inteligente

```cpp
void lerQTRASegueLinha() {
  se2 = analogRead(SE2_PIN) >> 2;  // 10-bit → 8-bit
  se1 = analogRead(SE1_PIN) >> 2;
  sd1 = analogRead(SD1_PIN) >> 2;
  sd2 = analogRead(SD2_PIN) >> 2;
}
```

- 7 sensores de refletância analógicos
- Conversão otimizada de resolução
- Detecção de curvas, cruzamentos e encruzilhadas

### 2. Máquina de Estados

| Estado | Sensores | Ação Principal |
|--------|----------|----------------|
| `SEGUIR_LINHA` | QTR Array | Controle PD |
| `GAP_DETECT` | QTR + TCS34725 | Verificação fita vermelha |
| `OBSTACULO` | VL53L0X frontal | Desvio lateral |
| `RAMPA` | MPU6050 (pitch > 9°) | Ajuste de potência |
| `SALA_RESGATE` | Múltiplos | Sistema de varredura |

### 3. Sistema de Garra Robotizada

```cpp
void pegarVitima() {
  fecharGarraVerificaBotao();
  lerBtnVitima();  // Dupla verificação
  
  subirGarraVerificaVitima();
  
  if (vitimaGarraViva) {
    contCacambaVivas++;
    rotacionarGarraDir();  // Entrega área verde
  } else {
    contCacambaMortas++;
    rotacionarGarraEsq();  // Entrega área vermelha
  }
  
  abrirGarra();
  rotacionarGarraMeio();
}
```

**Componentes:**
- 5 servomotores: pás, elevação, rotação, cancelas (2x)
- Sensor laser VL53L0X para detecção
- Botão táctil para identificação vítimas vivas
- Cancelas separadas para classificação

### 4. Desvio de Obstáculos

```cpp
void desviarObstaculo(bool isEsquerdo) {
  lerLaserFrente();
  if (distanciaLaserFrente >= DIST_OBSTACULO) return;
  
  moverTrasPor(300);
  isEsquerdo ? virarEsquerdaGiro90() : virarDireitaGiro90();
  moverFrentePor(1150);
  isEsquerdo ? virarDireitaGiro90() : virarEsquerdaGiro90();
  moverFrentePor(2460);  // Contornar obstáculo
  // ... retorno à linha
}
```

Utiliza leitura não-bloqueante para manter performance do seguidor.

### 5. Controle de Rampa com Giroscópio

```cpp
void rampaOuGangorra() {
  lerGiroscopioDMP();
  
  if (pitch > 21) {
    descerGarraRampa();  // Estabilização
    while (pitch > 5) {
      analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(48));
      analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(48));
      lerGiroscopioDMP();
    }
  }
}
```

MPU6050 com DMP para processamento de movimento em tempo real.

## 🔧 Hardware

### Componentes Principais

| Componente | Modelo | Qtd | Função |
|------------|--------|-----|--------|
| MCU Principal | Arduino Mega 2560 | 1 | Controle geral |
| MCU Auxiliar | Arduino Nano | 1 | Sensor cor direito |
| Sensores Linha | QTR Pololu | 7 | Refletância |
| Dist. Laser | VL53L0X ToF | 2 | Obstáculos/vítimas |
| Ultrassônico | HC-SR04 | 2 | Paredes laterais |
| Sensor Cor | TCS34725 | 2 | Detecção áreas |
| IMU | MPU6050 | 1 | Inclinação |
| Servomotores | - | 5 | Garra |
| Motores DC | - | 2 | Locomoção |
| LEDs RGB | - | 2 | Status |
| Botões | - | 5 | Toque |

### Pinout Resumido

```cpp
// Motores (PWM)
#define MOTOR_ESQ_F_PIN 3
#define MOTOR_DIR_F_PIN 5

// Sensores QTR (Analógico)
#define SE3_PIN A8
#define SE1_PIN A15
#define SM_PIN  A14
#define SD1_PIN A12
#define SD3_PIN A11

// Servos
#define SERVO_PA_GARRA_PIN 11
#define SERVO_SUBIR_GARRA_PIN 12
#define SERVO_ROTACIONAR_GARRA_PIN 10

// Botões
#define BTN_AREA_ESQ_PIN 24
#define BTN_PAREDE_ESQ_PIN 22
#define BTN_VITIMA_PIN 33

// Laser I2C
#define LASER_FRENTE_XSHUT_PIN 14
#define LASER_GARRA_XSHUT_PIN 19
```

## 📁 Estrutura do Código

```
trexregional24/
├── arduino_mega/              # Firmware principal
│   ├── arduino_mega.ino       # Loop principal + setup
│   ├── 1_debug.ino            # Debug e calibração
│   ├── 2_movimento.ino        # Motores e servos
│   ├── 3_sensores.ino         # Leitura sensores
│   ├── 4_desafios.ino         # Gap, obstáculo, rampa
│   ├── 5_resgate.ino          # Sistema de resgate
│   ├── mega_pins.h            # Definições de pinos
│   └── mega_def.h             # Constantes
│
├── arduino_nano/              # Firmware auxiliar
│   └── arduino_nano.ino       # TCS34725 direito
│
├── tests/                     # Testes unitários
├── libraries/                 # Bibliotecas
└── docs/                      # Documentação
```

### Arquivos Principais

- **`arduino_mega.ino`**: Máquina de estados global e loop principal
- **`3_sensores.ino`**: Abstração de todos os sensores (QTR, TCS, VL53L0X, MPU6050)
- **`4_desafios.ino`**: `analisarVerde()`, `desviarObstaculo()`, `verificarGap()`, `rampaOuGangorra()`
- **`5_resgate.ino`**: `salaDeResgate()`, `varredura()`, `pegarVitima()`, `encontrouArea()`

## 🚀 Instalação

### Pré-requisitos

- [Arduino IDE](https://www.arduino.cc/en/software) 2.0+

### Bibliotecas (incluídas em `/libraries`)

- QTRSensors - Refletância
- VL53L0X_mod - Laser ToF
- MPU6050 - IMU com DMP
- Ultrasonic - HC-SR04
- Adafruit_TCS34725 - Sensor RGB
- Adafruit_BusIO - I2C/SPI

### Passos

1. **Clone o repositório:**
```bash
git clone https://github.com/caio-pellegrini/trexregional24.git
```

2. **Copie bibliotecas para Arduino/libraries/**

3. **Configure `mega_def.h`:**
```cpp
#define DEBUG 0              // 0 = desabilitado
#define GAP 1                // Habilita gaps
#define OBSTACULO 1          // Habilita obstáculos
#define RAMPA 1              // Habilita rampa
```

4. **Upload:**
   - Arduino Mega: `arduino_mega/arduino_mega.ino`
   - Arduino Nano: `arduino_nano/arduino_nano.ino`

## 💻 Uso

### Calibração

1. Habilite debug: `#define DEBUG 1`
2. Mova robô sobre branco e preto por 10s
3. Ajuste limiares em `mega_def.h`:

```cpp
#define CORTE_QTR_P 110      // Limiar preto
#define CORTE_QTR_B 60       // Limiar branco
#define CORTE_FRENTE 110     // Sensor frontal
```

### LEDs de Status

- 🟢 Verde: Seguindo linha
- 🔴 Vermelho: Obstáculo
- 🔵 Azul: Rampa/gangorra
- 🟣 Roxo: Sala de resgate
- ⚪ Branco: Cruzamento

### Debug Serial

```
QTR: se3=25 se2=30 se1=120 sm=115 sd1=35 sd2=28 sd3=22
LaserFrente: 45mm | LaserGarra: 150mm
UltraEsq: 25cm | UltraDir: 30cm
Pitch: 2.5° | Roll: 0.8°
```

## 🎮 Desafios Implementados

- ✅ Seguidor de linha (curvas 90°, cruzamentos, becos)
- ✅ Obstáculos (detecção laser < 12cm, contorno)
- ✅ Gaps (fita vermelha, timeout para sala)
- ✅ Rampas (pitch > 9°, ajuste potência)
- ✅ Gangorra (diferenciação via ultrassônicos)
- ✅ Sala de resgate (varredura, coleta, classificação, entrega)

## 🐛 Solução de Problemas

### Robô não segue linha

```cpp
// Ative debug e ajuste limiares
#define DEBUG_QTR 1
#define CORTE_QTR_P 120  // Aumente se não detecta preto
```

### Sensores laser não inicializam

1. Verifique conexões I2C (SDA/SCL)
2. Confirme alimentação 5V
3. Execute scanner I2C para verificar endereços

### Comunicação Serial falha

```cpp
// Adicione timeouts
Serial2.setTimeout(100);  // Mega
Serial.setTimeout(100);   // Nano
```

### Servos tremem

1. Use fonte externa 6V/2A
2. Adicione capacitor 100µF
3. Separe GND de potência/lógica

## 🤝 Contribuindo

1. Fork o projeto
2. Crie branch (`git checkout -b feature/Nova`)
3. Commit (`git commit -m 'Adiciona funcionalidade'`)
4. Push (`git push origin feature/Nova`)
5. Abra Pull Request

### Diretrizes

- Funções: `camelCase` (ex: `lerSensores()`)
- Constantes: `UPPER_SNAKE_CASE` (ex: `CORTE_QTR_P`)
- Máximo 50 linhas por função
- Comente código complexo

## 👥 Equipe

- **Desenvolvedor**: Caio Pellegrini
- **Equipe**: T-Rex Robótica

## 🙏 Agradecimentos

- [Pololu](https://www.pololu.com/) - QTRSensors
- [Adafruit](https://www.adafruit.com/) - TCS34725, BusIO
- OBR - Olimpíada Brasileira de Robótica

## 🌐 Redes Sociais

- [Instagram T-Rex](https://www.instagram.com/sesi_trex/)

---

<p align="center">
  Desenvolvido com ❤️ pela equipe T-Rex Robótica<br>
  <sub>OBR 2024 - RoboCup Rescue Line</sub>
</p>
