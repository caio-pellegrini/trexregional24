#if DEBUG

    void calibrar()
    {
        Serial.println();

        #if DEBUG_QTRA
            lerQTRATodos();

            Serial.print(" | se3: ");
            Serial.print(se3);
            Serial.print(" se2: ");
            Serial.print(se2);
            Serial.print(" se1: ");
            Serial.print(se1);
            Serial.print(" | sm: ");
            Serial.print(sm);
            Serial.print(" | sd1: ");
            Serial.print(sd1);
            Serial.print(" sd2: ");
            Serial.print(sd2);
            Serial.print(" sd3: ");
            Serial.print(sd3);
        #endif

        #if DEBUG_REFL_FRENTE
            lerReflFrente();

            Serial.print(" | sf: ");
            Serial.print(sf);
        #endif

        #if DEBUG_TCS_AMBOS
            lerTcsAmbos();
            lerTcsAmbos();

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

        #if DEBUG_TCS_FRENTE
            lerTcsFrente();

            Serial.print(" TCS FRENTE: ");
            Serial.print("R:");
            Serial.print(rgbTcsFrente[0]);
            Serial.print(",G:");
            Serial.print(rgbTcsFrente[1]);
            Serial.print(",B:");
            Serial.print(rgbTcsFrente[2]);
        #endif

        #if DEBUG_GIROSCOPIO
            // unsigned long tempoInicial3 = millis();
            // lerGiroscopio();
            // unsigned long tempoFinal3 = millis();
            // Serial.print(" | ax: ");
            // Serial.print(ax);
            // Serial.print(" ay: ");
            // Serial.print(ay);
            // Serial.print(" az: ");
            // Serial.print(az);
            // Serial.print(" gx: ");
            // Serial.print(gx);
            // Serial.print(" gy: ");
            // Serial.print(gy);
            // Serial.print(" gz: ");
            // Serial.print(gz);
            // Serial.print(" | Tempo: ");
            // Serial.print(tempoFinal3 - tempoInicial3);
            // Serial.print("ms");

            lerGiroscopioDMP();

            Serial.print(" | yaw: ");
            Serial.print(yaw);
            Serial.print(" pitch: ");
            Serial.print(pitch);
            Serial.print(" roll: ");
            Serial.print(roll);
        #endif

        #if DEBUG_LASER_FRENTE
            unsigned long tempoInicial = millis();
            lerLaserFrente();
            unsigned long tempoFinal = millis();
            Serial.print(" | LaserFrente: ");
            Serial.print(distanciaLaserFrente);
            Serial.print(" | Tempo: ");
            Serial.print(tempoFinal - tempoInicial);
            Serial.print("ms");
        #endif

        #if DEBUG_LASER_GARRA
            unsigned long tempoInicial2 = millis();
            lerLaserGarra();
            unsigned long tempoFinal2 = millis();

            Serial.print(" | LaserGarra: ");
            Serial.print(distanciaLaserGarra);
            Serial.print(" | Tempo: ");
            Serial.print(tempoFinal2 - tempoInicial2);
            Serial.print("ms");

            // unsigned long tempoInicial3 = millis();
            // distanciaLaserGarra = 0;
            // while(true) {
            //     lerLaserGarraNaoBloquante();
            //     if (distanciaLaserGarra != 0) {
            //         break;
            //     }
            // };
            // unsigned long tempoFinal3 = millis();

            // Serial.print(" | LaserGarraNaoBlock: ");
            // Serial.print(distanciaLaserGarra);
            // Serial.print(" | Tempo: ");
            // Serial.print(tempoFinal3 - tempoInicial3);
            // Serial.print("ms");

        #endif

        #if DEBUG_ULTRA
            
            lerUltraEsq();
            
            lerUltraDir();
            
            Serial.print(" | ultraEsq: ");
            Serial.print(distanciaUltraEsq);
            

            Serial.print(" ultraDir: ");
            Serial.print(distanciaUltraDir);
            
            
        #endif

        #if DEBUG_BOTOES
            lerBtnArea();
            lerBtnParede();
            lerBtnVitima();

            Serial.print(" | btnAreaE: ");
            Serial.print(btnAreaEsq);
            Serial.print(" btnAreaD: ");
            Serial.print(btnAreaDir);
            Serial.print(" btnParedeE: ");
            Serial.print(btnParedeEsq);
            Serial.print(" btnParedeD: ");
            Serial.print(btnParedeDir);
            Serial.print(" btnVitima: ");
            Serial.print(btnVitima);
        #endif
    }

#endif