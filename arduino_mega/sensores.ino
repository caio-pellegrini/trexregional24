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

  // testeeee
  mpu.setRate(999); // 1 Hz
  mpu.setDLPFMode(1);
  mpu.setIntDataReadyEnabled(true);

  // load and configure the DMP
  Serial.print("Inicializando DMP... ");
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXAccelOffset(-581);
  mpu.setYAccelOffset(1369);
  mpu.setZAccelOffset(914);
  mpu.setXGyroOffset(34);
  mpu.setYGyroOffset(27);
  mpu.setZGyroOffset(26);
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

    // Serial.print("ypr\t");
    // Serial.print(ypr[0] * 180 / M_PI);
    // Serial.print("\t");
    // Serial.print(ypr[1] * 180 / M_PI);
    // Serial.print("\t");
    // Serial.print(ypr[2] * 180 / M_PI);

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