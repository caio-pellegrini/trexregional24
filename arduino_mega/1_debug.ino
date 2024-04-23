#if DEBUG
void calibrar()
{
    Serial.println();

    #if DEBUG_QTRA
        se3 = analogRead(SE3_PIN) >> 2; // transforma o valor de 10-bits (0-1023) para 8-bits (0-255)
        se2 = analogRead(SE2_PIN) >> 2;
        se1 = analogRead(SE1_PIN) >> 2;
        se0 = analogRead(SE0_PIN) >> 2;
        sd0 = analogRead(SD0_PIN) >> 2;
        sd1 = analogRead(SD1_PIN) >> 2;
        sd2 = analogRead(SD2_PIN) >> 2;
        sd3 = analogRead(SD3_PIN) >> 2;

        Serial.print(" | se3: ");
        Serial.print(se3);
        Serial.print(" se2: ");
        Serial.print(se2);
        Serial.print(" se1: ");
        Serial.print(se1);
        Serial.print(" se0: ");
        Serial.print(se0);
        Serial.print(" sd0: ");
        Serial.print(sd0);
        Serial.print(" sd1: ");
        Serial.print(sd1);
        Serial.print(" sd2: ");
        Serial.print(sd2);
        Serial.print(" sd3: ");
        Serial.print(sd3);
    #endif

    #if DEBUG_QTRRC
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

#if DEBUG_TCS_VERDE
    Serial2.println("caio");

    lerSensorCor(&tcsEsq, rgbEsq);
    
    Serial.print("TCS ESQ: ");
    Serial.print("R:");
    Serial.print(rgbEsq[0]);
    Serial.print(",G:");
    Serial.print(rgbEsq[1]);
    Serial.print(",B:");
    Serial.print(rgbEsq[2]);

    lerDadosSensorRemoto(rgbDir);

    Serial.print(" TCS DIR: ");
    Serial.print("R:");
    Serial.print(rgbDir[0]);
    Serial.print(",G:");
    Serial.print(rgbDir[1]);
    Serial.print(",B:");
    Serial.print(rgbDir[2]);
#endif

#if DEBUG_TCS_AREA
    tcadesliga();
    tcaSelecionar(canalTcsArea);
    Serial.print(" corArea: ");
    corArea = lerCorArea(&tcsArea);
    Serial.print(corArea);
    tcadesliga();
    if (corArea == 1)
    {
        ligarLed(AMBOS, VERDE, 500);
    }
    else if (corArea == 2)
    {
        ligarLed(AMBOS, VERMELHO, 500);
    }
#endif

#if DEBUG_GIROSCOPIO
    if (mpuInterrupt)
    {
        // lerGiroscopio();
        lerGirodmp();
    }
    Serial.print(" | yaw: ");
    Serial.print(yaw);
    Serial.print(" pitch: ");
    Serial.print(pitch);
    Serial.print(" roll: ");
    Serial.print(roll);
#endif

#if DEBUG_LASER_FRENTE
    lerLaserFrente();
    Serial.print(" LaserFrente: ");
    if (medidaLaserFrente.RangeStatus != 4)
    { // phase failures have incorrect data
        Serial.print(medidaLaserFrente.RangeMilliMeter);
        Serial.print(" mm");
    }
    else
    {
        Serial.print("Fora de alcance ");
    }
#endif

#if DEBUG_LASER_ULTRA_GARRA
    lerLaserVit();
    Serial.print(" LaserVitima: ");
    if (medidaLaserVitima.RangeStatus != 4)
    { // phase failures have incorrect data
        Serial.print(medidaLaserVitima.RangeMilliMeter);
        Serial.print(" mm ");
    }
    else
    {
        Serial.print("Fora de alcance ");
    }

    ultraGarra = ultrasonicoGarra.read(CM);
    Serial.print(" ultraGarra: ");
    Serial.print(ultraGarra);
#endif

#if DEBUG_ULTRA
    ultraE = ultrasonicEsq.read(CM);
    Serial.print(" ultraE: ");
    Serial.print(ultraE);
    ultraD = ultrasonicDir.read(CM);
    Serial.print(" ultraD: ");
    Serial.print(ultraD);
#endif

#if DEBUG_BOTOES
    btnE = !digitalRead(btnEpin);
    btnD = !digitalRead(btnDpin);
    btnPa = !digitalRead(btnPaPin);
    btnVit = digitalRead(btnVitPin);
    Serial.print(" | btnE: ");
    Serial.print(btnE);
    Serial.print(" btnD: ");
    Serial.print(btnD);
    Serial.print(" btnPa: ");
    Serial.print(btnPa);
    Serial.print(" btnVit: ");
    Serial.print(btnVit);
#endif
}
#endif