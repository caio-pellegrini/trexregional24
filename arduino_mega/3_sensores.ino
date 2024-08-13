void lerReflPrincipal() {
  se3 = analogRead(SE3_PIN) >> 2; // >> 2 transforma o valor de 10-bits (0-1023) para 8-bits (0-255)
  se2 = analogRead(SE2_PIN) >> 2;
  se1 = analogRead(SE1_PIN) >> 2;
  sm = analogRead(SM_PIN) >> 2;
  sd1 = analogRead(SD1_PIN) >> 2;
  sd2 = analogRead(SD2_PIN) >> 2;
  sd3 = analogRead(SD3_PIN) >> 2;
}

void lerReflSegueLinha() {
  se3 = analogRead(SE3_PIN) >> 2;
  se2 = analogRead(SE2_PIN) >> 2;
  se1 = analogRead(SE1_PIN) >> 2;
  sd1 = analogRead(SD1_PIN) >> 2;
  sd2 = analogRead(SD2_PIN) >> 2;
  sd3 = analogRead(SD3_PIN) >> 2;
}

void lerReflFrente() { sf = analogRead(SF_PIN) >> 2; }

void lerTcsAmbos() {
  Serial2.println(
      "caio"); // esta linha indica ao Nano que ele deve começar a leitura
  lerTcsEsq();
  lerTcsDir();
}

void lerTcsEsq() {
  uint16_t r, g, b, c;
  tcaSelecionar(CANAL_TCS_ESQ);
  tcsEsq.getRawData(&r, &g, &b, &c);
  tcaDesligar();

  rgbTcsEsq[0] = map(r, 0, TCS_SATURACAO_MAX, 0,
                     255); // usar constrain para limitar um valor específico
  rgbTcsEsq[1] = map(g, 0, TCS_SATURACAO_MAX, 0, 255);
  rgbTcsEsq[2] = map(b, 0, TCS_SATURACAO_MAX, 0, 255);
  // rgbTcsEsq[0] = r;
  // rgbTcsEsq[1] = g;
  // rgbTcsEsq[2] = b;
}

/// @brief Lê os valores RGB do sensor TCS34725 direito, conectado ao Arduino
/// Nano via Serial2.
void lerTcsDir() {
  static char buffer[64] = {0};
  static int index = 0;
  bool dadosRecebidos = false;

  // Variáveis para armazenar os valores RGB extraídos
  uint16_t r, g, b;

  // Define um tempo limite para a recepção
  unsigned long startTime = millis();

  while (millis() - startTime <
         1000) { // Aguarda até 1 segundo por uma resposta
    if (Serial2.available() > 0) {
      char received = Serial2.read();
      if (received == '\n') {
        buffer[index] = '\0'; // Termina a string se for o final da mensagem
        dadosRecebidos = true;
        break; // Sai do loop após processar a mensagem
      } else if (index < 63) {
        buffer[index++] = received;
      }
    }
  }

  if (dadosRecebidos) {
    // Serial.print(buffer);

    // Tenta extrair os valores R, G, B da string recebida
    if (sscanf(buffer, "R:%d,G:%d,B:%d", &r, &g, &b) ==
        3) { // Se três valores forem lidos com sucesso
      rgbTcsDir[0] = r;
      rgbTcsDir[1] = g;
      rgbTcsDir[2] = b;
    }
    // else { Serial.print(" Formato de dados inválido."); }
  } else {
    Serial.print("Timeout");
    rgbTcsDir[0] = 0;
    rgbTcsDir[1] = 0;
    rgbTcsDir[2] = 0;
  }

  // Limpa o buffer e reseta o índice após processar a mensagem
  memset(buffer, 0, sizeof(buffer));
  index = 0;
}

void lerTcsFrente() {
  uint16_t r, g, b, c;
  tcaSelecionar(CANAL_TCS_FRENTE);
  tcsFrente.getRawData(&r, &g, &b, &c);
  tcaDesligar();

  rgbTcsFrente[0] = r;
  rgbTcsFrente[1] = g;
  rgbTcsFrente[2] = b;
}

void ligarGiroscopio() {
  // initialize device
  mpu.initialize();
  pinMode(MPU6050_INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(mpu.testConnection() ? "MPU6050 conectado :)"
                                      : "MPU6050 conexão falhou :(");

  // rate 999 => 1 Hz / rate 49 => 20 Hz /  9 => 100 Hz
  mpu.setRate(9);     // Taxa de amostragem
  mpu.setDLPFMode(1); // Filtro Digital de Passa Baixa
  mpu.setIntDataReadyEnabled(true);

  // load and configure the DMP
  Serial.print("Inicializando DMP... ");
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXAccelOffset(1185);
  mpu.setYAccelOffset(3181);
  mpu.setZAccelOffset(1542);
  mpu.setXGyroOffset(71);
  mpu.setYGyroOffset(29);
  mpu.setZGyroOffset(-49);
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050 (uncomment to calibrate) 
    // mpu.CalibrateAccel(6); mpu.CalibrateGyro(6);
    // Serial.println();
    // mpu.PrintActiveOffsets();

    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    // Serial.print("Enabling interrupt detection - Arduino external interrupt "
    // + digitalPinToInterrupt(INTERRUPT_PIN));
    attachInterrupt(digitalPinToInterrupt(MPU6050_INTERRUPT_PIN), dmpDataReady,
                    RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use
    // it
    Serial.println("DMP pronto! Aguardando primeira leitura...");
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print("DMP Initialization failed - code ");
    Serial.print(devStatus);
  }
}

void lerGiroscopioDMP() {
  while (!mpuInterrupt) {
    // Fica esperando
    // delay(1); // Pequeno delay para evitar a sobrecarga da CPU
  }
  // read a packet from FIFO
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet
    // Serial.print("a");

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    yaw = ypr[0] * 180 / M_PI;
    pitch = ypr[1] * 180 / M_PI;
    roll = ypr[2] * 180 / M_PI;

    mpuInterrupt = false;
  }
}

void lerGiroscopio() {
  while (!mpuInterrupt) {
    // Fica esperando
    // delay(1); // Pequeno delay para evitar a sobrecarga da CPU
  }
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // pitch = map(ax, -17000, 17000, 0, 255);
  mpuInterrupt = false;
}

// Tempo de leitura Laser = 40ms
void lerLaserFrente() {
  distanciaLaserFrente = laserFrente.readRangeSingleMillimeters();
  // laserFrente.readRangeNoBlocking(distanciaLaserFrente);
}

void lerLaserGarra() {
  // distanciaLaserGarra = laserGarra.readRangeSingleMillimeters();
  distanciaLaserGarra = laserGarra.readRangeContinuousMillimeters();
}

bool lerLaserFrenteNaoBloquante() {
  return laserFrente.readRangeNoBlocking(distanciaLaserFrente);
}

bool lerLaserGarraNaoBloquante() {
  return laserGarra.readRangeNoBlocking(distanciaLaserGarra);
}

void lerBtnArea() {
  btnAreaEsq = digitalRead(BTN_AREA_ESQ_PIN);
  btnAreaDir = digitalRead(BTN_AREA_DIR_PIN);
}

void lerBtnParede() {
  btnParedeEsq = digitalRead(BTN_PAREDE_ESQ_PIN);
  btnParedeDir = digitalRead(BTN_PAREDE_DIR_PIN);
}

void lerBtnVitima() { btnVitima = !digitalRead(BTN_VITIMA_PIN); }

void lerBtnFc() {
  btnFcEsq = !digitalRead(BTN_FC_ESQ_PIN);
  btnFcDir = !digitalRead(BTN_FC_DIR_PIN);
}

// Tempo de leitura Ultrassonico = 5ms
void lerUltraEsq() { distanciaUltraEsq = ultrasonicEsq.read(CM); }

void lerUltraDir() { distanciaUltraDir = ultrasonicDir.read(CM); }