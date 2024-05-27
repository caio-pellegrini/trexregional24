void lerQTRATodos() {
  se3 = analogRead(SE3_PIN) >> 2; // >> 2 transforma o valor de 10-bits (0-1023) para 8-bits (0-255)
  se2 = analogRead(SE2_PIN) >> 2;
  se1 = analogRead(SE1_PIN) >> 2;
  se0 = analogRead(SE0_PIN) >> 2;
  sd0 = analogRead(SD0_PIN) >> 2;
  sd1 = analogRead(SD1_PIN) >> 2;
  sd2 = analogRead(SD2_PIN) >> 2;
  sd3 = analogRead(SD3_PIN) >> 2;
}

void lerQTRASegueLinha() {
  se2 = analogRead(SE2_PIN) >> 2;
  se1 = analogRead(SE1_PIN) >> 2;
  sd1 = analogRead(SD1_PIN) >> 2;
  sd2 = analogRead(SD2_PIN) >> 2;
}

void lerVerde() {
  Serial2.println("caio");
  lerSensorCor(&tcsEsq, rgbEsq);
  lerDadosSensorRemoto(rgbDir);
}

void lerDadosSensorRemoto(uint8_t *rgbValues)
{
  static char buffer[64] = {0};
  static int index = 0;
  bool dadosRecebidos = false;

  // Variáveis para armazenar os valores RGB extraídos
  uint16_t r, g, b;

  // Define um tempo limite para a recepção
  unsigned long startTime = millis();

  while (millis() - startTime < 1000)
  { // Aguarda até 1 segundo por uma resposta
    if (Serial2.available() > 0)
    {
      char received = Serial2.read();
      if (received == '\n')
      {
        buffer[index] = '\0'; // Termina a string se for o final da mensagem
        dadosRecebidos = true;
        break; // Sai do loop após processar a mensagem
      }
      else if (index < 63)
      {
        buffer[index++] = received;
      }
    }
  }

  if (dadosRecebidos)
  {
    // Serial.print(buffer);

    // Tenta extrair os valores R, G, B da string recebida
    if (sscanf(buffer, "R:%d,G:%d,B:%d", &r, &g, &b) == 3)
    { // Se três valores forem lidos com sucesso
      rgbValues[0] = r;
      rgbValues[1] = g;
      rgbValues[2] = b;
    }
    else
    {
      // Serial.print(" Formato de dados inválido.");
    }
  }
  else
  {
    Serial.print("Timeout");
    rgbValues[0] = 0;
    rgbValues[1] = 0;
    rgbValues[2] = 0;
  }

  // Limpa o buffer e reseta o índice após processar a mensagem
  memset(buffer, 0, sizeof(buffer));
  index = 0;
}

void lerSensorCor(Adafruit_TCS34725 *tcs, uint8_t *rgbValues)
{
  uint16_t r, g, b, c;

  tcs->getRawData(&r, &g, &b, &c);

  rgbValues[0] = map(r, 0, TCS_SATURACAO_MAX, 0, 255);
  rgbValues[1] = map(g, 0, TCS_SATURACAO_MAX, 0, 255);
  rgbValues[2] = map(b, 0, TCS_SATURACAO_MAX, 0, 255);
  // rgbValues[0] = r;
  // rgbValues[1] = g;
  // rgbValues[2] = b;
}

void ligarGiroscopio() {
  // initialize device
  mpu.initialize();
  pinMode(MPU6050_INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(mpu.testConnection() ? "MPU6050 conectado :)" : "MPU6050 conexão falhou :(");

  // rate 999 => 1 Hz / rate 49 => 20 Hz /  9 => 100 Hz
  mpu.setRate(9); // Taxa de amostragem
  mpu.setDLPFMode(1); // Filtro Digital de Passa Baixa
  mpu.setIntDataReadyEnabled(true);

  // load and configure the DMP
  Serial.print("Inicializando DMP... ");
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXAccelOffset(-579);
  mpu.setYAccelOffset(1373);
  mpu.setZAccelOffset(914);
  mpu.setXGyroOffset(34);
  mpu.setYGyroOffset(27);
  mpu.setZGyroOffset(27);
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // // Calibration Time: generate offsets and calibrate our MPU6050 (uncomment to calibrate)
    // mpu.CalibrateAccel(6);
    // mpu.CalibrateGyro(6);
    // Serial.println();
    // mpu.PrintActiveOffsets();

    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);
    
    // enable Arduino interrupt detection
    // Serial.print("Enabling interrupt detection - Arduino external interrupt " + digitalPinToInterrupt(INTERRUPT_PIN));
    attachInterrupt(digitalPinToInterrupt(MPU6050_INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
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

void lerGiroDMP() {
  while (!mpuInterrupt) {
      // Fica esperando
      delay(10);  // Adicione um pequeno delay para evitar a sobrecarga da CPU
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
      delay(10);  // Adicione um pequeno delay para evitar a sobrecarga da CPU
  }
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  pitch = map(ax, -17000, 17000, 0, 255);
  mpuInterrupt = false;
}