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
    Serial.print(rgbTcsEsq[0]);
    Serial.print(",G:");
    Serial.print(rgbTcsEsq[1]);
    Serial.print(",B:");
    Serial.print(rgbTcsEsq[2]);

    Serial.print(" TCS DIR: ");
    Serial.print("R:");
    Serial.print(rgbTcsDir[0]);
    Serial.print(",G:");
    Serial.print(rgbTcsDir[1]);
    Serial.print(",B:");
    Serial.print(rgbTcsDir[2]);
#endif

#if DEBUG_TCS_AREA
    tcaDesliga();
    tcaSelecionar(canalTcsFrente);
    lerSensorCorFrente(&tcsFrente, rgbFrente);

    Serial.print(" TCS FRENTE: ");
    Serial.print("R:");
    Serial.print(rgbFrente[0]);
    Serial.print(",G:");
    Serial.print(rgbFrente[1]);
    Serial.print(",B:");
    Serial.print(rgbFrente[2]);

    // Serial.print(" corArea: ");
    // corArea = lerCorArea(&tcsArea);
    // Serial.print(corArea);
    // tcadesliga();
    // if (corArea == 1)
    // {
    //     ligarLed(AMBOS, VERDE, 500);
    // }
    // else if (corArea == 2)
    // {
    //     ligarLed(AMBOS, VERMELHO, 500);
    // }
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
    Serial.print(" | LaserFrente: ");
    Serial.print(distanciaLaserFrente);
#endif

#if DEBUG_LASER_GARRA
    lerLaserGarra();
    Serial.print(" | LaserGarra: ");
    Serial.print(distanciaLaserGarra);
#endif

#if DEBUG_ULTRA
    lerUltraEsq();
    Serial.print(" | ultraE: ");
    Serial.print(ultraEsq);

    lerUltraDir();
    Serial.print(" ultraD: ");
    Serial.print(ultraDir);
#endif

#if DEBUG_BOTOES
    lerBtnArea();
    lerBtnParede();

    Serial.print(" | btnAreaE: ");
    Serial.print(btnAreaEsq);
    Serial.print(" btnAreaD: ");
    Serial.print(btnAreaDir);
    Serial.print(" btnParedeE: ");
    Serial.print(btnParedeEsq);
    Serial.print(" btnParedeD: ");
    Serial.print(btnParedeDir);
#endif
}
#endif