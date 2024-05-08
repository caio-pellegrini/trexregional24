void lerVerde() {
  Serial2.println("caio");
  lerSensorCor(&tcsEsq, rgbEsq);
  lerDadosSensorRemoto(rgbDir);
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