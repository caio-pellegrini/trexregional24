#if CALIBRACAO 
  void calibrar() {
    Serial.println();

    #if CALIBRAR_QTRA

        se2 = analogRead(SE2_PIN);
        se1 = analogRead(SE1_PIN);

        sd1 = analogRead(SD1_PIN);
        sd2 = analogRead(SD2_PIN);

      Serial.print(" | sE3: ");
      Serial.print(sE3);
      Serial.print(" sE2: ");
      Serial.print(sE2);
      Serial.print(" sE1: ");
      Serial.print(sE1);
      Serial.print(" sEM: ");
      Serial.print(sEM);
      Serial.print(" sDM: ");
      Serial.print(sDM);
      Serial.print(" sD1: ");
      Serial.print(sD1);
      Serial.print(" sD2: ");
      Serial.print(sD2);
      Serial.print(" sD3: ");
      Serial.print(sD3);
    #endif

    #if CALIBRAR_QTRRC
      qtrc.read(sensorValues);
      sFE3 = map(sensorValues[0], 0, 2500, 0, 1023);
      sFE2 = map(sensorValues[1], 0, 2500, 0, 1023);
      sFE1 = map(sensorValues[2], 0, 2500, 0, 1023);
      sFD1 = map(sensorValues[3], 0, 2500, 0, 1023);
      sFD2 = map(sensorValues[4], 0, 2500, 0, 1023);
      sFD3 = map(sensorValues[5], 0, 2500, 0, 1023);

      Serial.print(" sFE3: " + String(sFE3));
      Serial.print(" sFE2: " + String(sFE2));
      Serial.print(" sFE1: " + String(sFE1));
      Serial.print(" | sFD1: " + String(sFD1));
      Serial.print(" sFD2: " + String(sFD2));
      Serial.print(" sFD3: " + String(sFD3));
    #endif

    #if CALIBRAR_TCS_VERDE
      // tcadesliga();
      // tcaSelecionar(canalTcsEsq);
      // SCE = lerSensorCor(&tcsEsq);
      // tcaSelecionar(canalTcsDir);
      // SCD = lerSensorCor(&tcsDir);
      // Serial.print(" | SCE: "); Serial.print(SCE);
      // Serial.print(" SCD: "); Serial.print(SCD);
      // tcadesliga();
      tcaSelecionar(canalTcsDir);
      Serial.print(" | SCD vermelho: ");
      printarFitaVermelha(&tcsDir);
      tcaSelecionar(canalTcsEsq);
      Serial.print(" | SCE vermelho: ");
      printarFitaVermelha(&tcsEsq);
    #endif

    #if CALIBRAR_TCS_AREA
      tcadesliga();
      tcaSelecionar(canalTcsArea);
      Serial.print(" corArea: ");
      corArea = lerCorArea(&tcsArea);
      Serial.print(corArea);
      tcadesliga();
      if (corArea == 1) {
        ligarLed(AMBOS, VERDE, 500);
      } else if (corArea == 2) {
        ligarLed(AMBOS, VERMELHO, 500);
      }
    #endif

    #if CALIBRAR_GIROSCOPIO
      if (mpuInterrupt) {
        //lerGiroscopio();
        lerGirodmp();
      }
      Serial.print(" | yaw: ");
      Serial.print(yaw);
      Serial.print(" pitch: ");
      Serial.print(pitch);
      Serial.print(" roll: ");
      Serial.print(roll);
    #endif

    #if CALIBRAR_LASER_FRENTE
      lerLaserFrente();
      Serial.print(" LaserFrente: ");
      if (medidaLaserFrente.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print(medidaLaserFrente.RangeMilliMeter); Serial.print(" mm");
      } else {
        Serial.print("Fora de alcance ");
      }
    #endif

    #if CALIBRAR_LASER_ULTRA_GARRA
      lerLaserVit();
      Serial.print(" LaserVitima: ");
      if (medidaLaserVitima.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print(medidaLaserVitima.RangeMilliMeter); Serial.print(" mm ");
      } else {
        Serial.print("Fora de alcance ");
      }

      ultraGarra = ultrasonicoGarra.read(CM);
      Serial.print(" ultraGarra: ");
      Serial.print(ultraGarra);
    #endif

    #if CALIBRAR_ULTRA_FRENTE
      ultraF = ultrasonicF.read(CM);
      Serial.print(" ultraF: ");
      Serial.print(ultraF);
    #endif

    #if CALIBRAR_BOTOES
      btnE = !digitalRead(btnEpin);
      btnD = !digitalRead(btnDpin);
      btnPa = !digitalRead(btnPaPin);
      btnVit = digitalRead(btnVitPin);
      Serial.print(" | btnE: "); Serial.print(btnE);
      Serial.print(" btnD: "); Serial.print(btnD);
      Serial.print(" btnPa: "); Serial.print(btnPa);
      Serial.print(" btnVit: "); Serial.print(btnVit);
    #endif
  }
#endif