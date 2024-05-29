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

    #if DEBUG_REFL_FRENTE
      sf = analogRead(SF_PIN) >> 2;

      Serial.print(" | sf: " + String(sf));
    #endif

#if DEBUG_TCS_VERDE
    lerVerde();
    lerVerde();

    Serial.print(" | TCS ESQ: ");
    Serial.print("R:");
    Serial.print(rgbEsq[0]);
    Serial.print(",G:");
    Serial.print(rgbEsq[1]);
    Serial.print(",B:");
    Serial.print(rgbEsq[2]);

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
    // lerGiroscopio();
    lerGiroDMP();
    
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
    Serial.print(medidaLaserFrente.RangeStatus);
    Serial.print(" ");
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