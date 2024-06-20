void analisarVerde(bool isBeco, bool isVerdeEsquerdo, bool isVerdeDireito) {
  lerTcsAmbos();
  lerTcsAmbos();

  desligarLed(AMBOS);

  bool verdeEsq = false;
  bool verdeDir = false;

  if (rgbTcsEsq[1] < CORTE_VERDE_ESQ && rgbTcsEsq[0] < CORTE_VERMELHO_CRUZ) {
    ligarLed(ESQ, VERDE);
    verdeEsq = true;
  }

  if (rgbTcsDir[1] != 0) {
    if (rgbTcsDir[1] < CORTE_VERDE_DIR && rgbTcsDir[0] < CORTE_VERMELHO_CRUZ) {
      ligarLed(DIR, VERDE);
      verdeDir = true;
    }
  } else {
    ligarLed(DIR, VERMELHO);  // avisa que o rgbTcsDir não recebeu dados do TCS
    analisarVerde(true, true, true);
  }

  moverFrentePor(TEMPO_MOVER_ANTES_CRUZ);  // mover pra frente antes de virar

  // Beco sem saida
  if (isBeco && verdeEsq && verdeDir) {
    virarEsquerdaGiro90();
    moverFrentePor(50);
    pararMotores();
    virarEsquerdaGiro90();
    moverFrentePor(300);
  }

  // Curva à esquerda
  if (isVerdeEsquerdo && verdeEsq && !verdeDir) {
    virarEsquerdaGiro90();
    // moverFrentePor(100); // coloquei para funcionar no circulo, mas se estiver atrapalhando pode tirar
  }

  // Curva à direita
  if (isVerdeDireito && !verdeEsq && verdeDir) {
    virarDireitaGiro90();
    // moverFrentePor(100); // coloquei para funcionar no circulo, mas se estiver atrapalhando pode tirar
  }

  // Seguir reto
  if (!verdeEsq && !verdeDir) {
    moverFrentePor(200);
  }

  pararMotores();
}

void desviarObstaculo(bool isEsquerdo) {
  pararMotores();
  delay(50);
  lerLaserFrente();
  if (distanciaLaserFrente == 0 || distanciaLaserFrente >= DIST_OBSTACULO) {
    return;
  }
  ligarLed(AMBOS, VERMELHO);
  moverTrasPor(300);
  if (isEsquerdo) {
    virarEsquerdaGiro90();
  } else {
    virarDireitaGiro90();
  }
  moverFrentePor(1100);
  if (isEsquerdo) {
    virarDireitaGiro90();
  } else {
    virarEsquerdaGiro90();
  }
  moverFrentePor(2100); // AJUSTAR DE ACORDO COM O TAMANHO DO OBSTACULO
  if (isEsquerdo) {
    virarDireitaGiro90();
  } else {
    virarEsquerdaGiro90();
  }
  unsigned long tempoInicial = millis();
  while (millis() - tempoInicial < 1100) {
    moverFrentePor(1);
    lerQTRATodos();
    if (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P && se0 >= CORTE_QTR_P && sd0 >= CORTE_QTR_P && sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P) {
      pararMotores();
      delay(500);
      moverFrentePor(150);
      break;
    }
  }
  if (isEsquerdo) {
    virarEsquerdaGiro90();
  } else {
    virarDireitaGiro90();
  }
  moverTrasPor(100);
  desligarLed(AMBOS);
}

void verificarGap() {
  ligarLed(AMBOS, AMARELO);
  unsigned long tempoInicial = millis();

  moverTrasPor(100);
  pararMotores();
  delay(300);
  lerTcsAmbos();
  lerTcsAmbos();

  if (rgbTcsEsq[0] > rgbTcsEsq[1] && rgbTcsEsq[0] > rgbTcsEsq[2] && rgbTcsDir[0] > rgbTcsDir[1] && rgbTcsDir[0] > rgbTcsDir[2]) {
    ligarLed(AMBOS, VERMELHO);
    delayInfinito();
    return;
  }

  moverFrentePor(100);

  while (true) {
    moverFrentePor(1);
    lerQTRATodos();

    if (se3 > CORTE_QTR_P || se2 > CORTE_QTR_P || se1 > CORTE_QTR_P || se0 > CORTE_QTR_P || sd0 > CORTE_QTR_P || sd1 > CORTE_QTR_P || sd2 > CORTE_QTR_P || sd3 > CORTE_QTR_P) {
      desligarLed(AMBOS);
      pararMotores();
      delay(200);
      break;
    }

    // VALOR DE ENTRADA PARA SALA DE RESGATE
    if (millis() - tempoInicial >= 2000) {
      pararMotores();
      ligarLed(AMBOS, ROXO, 500);
      salaDeResgate();
      break;
    }
  }
}

void rampaOuGangorra() {
  ligarLed(AMBOS, AZUL);
  moverFrentePor(350);
  pararMotores();
  delay(100);
  lerGiroscopioDMP();

  // GANGORRA
  if (pitch > 12 && pitch < 21) {
    ligarLed(AMBOS, AZUL);
    servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
    servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
    descerGarraRampa();
    while (pitch > 15) {
      lerGiroscopioDMP();
      analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(48));
      analogWrite(MOTOR_ESQ_T_PIN, 0);
      analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(48));
      analogWrite(MOTOR_DIR_T_PIN, 0);
      delay(3);

      // SE HOUVER LOMBADA
      analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(70));
      analogWrite(MOTOR_ESQ_T_PIN, 0);
      analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(70));
      analogWrite(MOTOR_DIR_T_PIN, 0);
      delay(1);

      lerQTRASegueLinha();

      if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P) {
        seguirLinhaEsquerda(1);
      }

      if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P) {
        seguirLinhaDireita(1);
      }
    }
    moverFrenteLento();
    delay(300);
    pararMotores();
    subirGarra();
    servoSubirGarra.detach();
    servoRotacionarGarra.detach();
    moverTrasPor(100);
    pararMotores();
  } else if (pitch > 21) {
    // RAMPA

    ligarLed(AMBOS, VERMELHO);
    servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
    servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
    descerGarraRampa();
    while (pitch > 5) {
      analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(48));
      analogWrite(MOTOR_ESQ_T_PIN, 0);
      analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(48));
      analogWrite(MOTOR_DIR_T_PIN, 0);
      delay(3);

      // SE HOUVER LOMBADA
      analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(70));
      analogWrite(MOTOR_ESQ_T_PIN, 0);
      analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(70));
      analogWrite(MOTOR_DIR_T_PIN, 0);
      delay(1);

      lerQTRASegueLinha();

      if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P) {
        seguirLinhaEsquerda(1);
      }

      if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P) {
        seguirLinhaDireita(1);
      }
      lerGiroscopioDMP();
    }

    pararMotores();
    subirGarra();
    servoSubirGarra.detach();
    servoRotacionarGarra.detach();
    // moverTrasPor(100);
    pararMotores();
    delay(50);
    lerGiroscopioDMP();

    while (pitch > -5) {
      lerQTRATodos();
      lerReflFrente();

      // CRUZAMENTO
      if (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P && se0 >= CORTE_QTR_P && sd0 >= CORTE_QTR_P && sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P) {
        ligarLed(AMBOS, BRANCO);
        moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
        pararMotores();
        analisarVerde(true, true, true);
        desligarLed(AMBOS);
      }

      // MEIO CRUZAMENTO ESQUERDO
      if (sf >= CORTE_FRENTE && (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd1 <= CORTE_QTR_P && sd2 <= CORTE_QTR_P && sd3 <= CORTE_QTR_P)) {
        ligarLed(ESQ, BRANCO);
        moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
        pararMotores();
        analisarVerde(false, true, false);
        desligarLed(AMBOS);
      }

      // MEIO CRUZAMENTO DIREITO
      if (sf >= CORTE_FRENTE && (se3 <= CORTE_QTR_P && se2 <= CORTE_QTR_P && se1 <= CORTE_QTR_P) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
        ligarLed(DIR, BRANCO);
        moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
        pararMotores();
        analisarVerde(false, false, true);
        desligarLed(AMBOS);
      }

      // 90 GRAUS DIREITO
      if (sf <= CORTE_FRENTE && (se3 <= CORTE_QTR_P && se2 <= CORTE_QTR_P && se1 <= CORTE_QTR_P) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
        seguirLinhaDireita(12);
      }

      // 90 GRAUS ESQUERDO
      if (sf <= CORTE_FRENTE && (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd1 <= CORTE_QTR_P && sd2 <= CORTE_QTR_P && sd3 <= CORTE_QTR_P)) {
        seguirLinhaEsquerda(12);
      }

      seguidorMoverFrente();

      lerQTRASegueLinha();

      if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P) {
        seguirLinhaEsquerda(3);
      }

      if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P) {
        seguirLinhaDireita(3);
      }


      lerGiroscopioDMP();
    }

    if (pitch <= -5) {
      moverFrenteLento();
      delay(1200);
    }
  }
  desligarLed(AMBOS);
}