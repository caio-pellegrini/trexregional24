void verificarCruzamento() {
  if (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P && sm >= CORTE_QTR_P && sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P) {
    ligarLed(AMBOS, AMARELO);
    moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotores();
    analisarVerde(1, 1, 1); // 1, 1, 1
    desligarLed(AMBOS);
  }
}

void verificarMeioCruzamentoEsq() {
  if (sf >= CORTE_FRENTE && sm >= CORTE_QTR_P &&(se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd1 <= CORTE_QTR_P && sd2 <= CORTE_QTR_P && sd3 <= CORTE_QTR_P)) {
    ligarLed(ESQ, AMARELO);
    moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotores();
    analisarVerde(0, 1, 0);  // 0, 1, 0
    desligarLed(AMBOS);
  }
}

void verificarMeioCruzamentoDir() {
  if (sf >= CORTE_FRENTE && sm >= CORTE_QTR_P && (se3 <= CORTE_QTR_P && se2 <= CORTE_QTR_P && se1 <= CORTE_QTR_P) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
    ligarLed(DIR, AMARELO);
    moverTrasPor(TEMPO_MOVER_ANTES_ANALISAR_VERDE);
    pararMotores();
    analisarVerde(0, 0, 1);  // 0, 0, 1
    desligarLed(AMBOS);
  }
}

void verificarGap() {
  ligarLed(AMBOS, BRANCO);
  unsigned long tempoInicial = millis();

  // moverFrentePor(50);  // O QUANTO DEVE ANDAR ATÉ ENCONTRAR A FITA VERMELHA
  
  pararMotores();
  delay(200);
  lerTcsAmbos();
  lerTcsAmbos();

  if (rgbTcsEsq[0] > rgbTcsEsq[1] && rgbTcsEsq[0] > rgbTcsEsq[2] && rgbTcsDir[0] > rgbTcsDir[1] && rgbTcsDir[0] > rgbTcsDir[2]) {
    ligarLed(AMBOS, VERMELHO);
    delayInfinito();
    return;
  }

  moverFrentePor(80);  //  AJUSTE PARA PULAR LINHA PRETA EM CRUZAMENTOS

  while (true) {
    moverFrenteLentoPor(1);
    lerReflPrincipal();

    if (se3 > CORTE_QTR_P || se2 > CORTE_QTR_P || se1 > CORTE_QTR_P || sm > CORTE_QTR_P || sd1 > CORTE_QTR_P || sd2 > CORTE_QTR_P || sd3 > CORTE_QTR_P) {
      desligarLed(AMBOS);
      pararMotores();
      delay(200);
      break;
    }

    // VALOR DE ENTRADA PARA SALA DE RESGATE
    if (millis() - tempoInicial >= 2600) {
      pararMotores();
      ligarLed(AMBOS, ROXO, 500);
      salaDeResgate();
      break;
    }
  }
}

void analisarVerde(bool isBeco, bool isVerdeEsquerdo, bool isVerdeDireito) {
  lerTcsAmbos();
  lerTcsAmbos();

  desligarLed(AMBOS);

  bool verdeEsq = false;
  bool verdeDir = false;

  if (rgbTcsEsq[1] < CORTE_VERDE_ESQ && rgbTcsEsq[1] > CORTE_VERDE_ESQ2) {
    ligarLed(ESQ, VERDE);
    verdeEsq = true;
  }

  if (rgbTcsDir[1] != 0) {
    if (rgbTcsDir[1] < CORTE_VERDE_DIR && rgbTcsDir[1] > CORTE_VERDE_DIR2) {
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
    virarEsqGiro90();
    moverFrentePor(50);
    pararMotores();
    virarEsqGiro90();
    moverFrentePor(300);
  }

  // Curva à esquerda
  if (isVerdeEsquerdo && verdeEsq && !verdeDir) {
    virarEsqGiro90();
    // moverFrentePor(100); // coloquei para funcionar no circulo, mas se estiver atrapalhando pode tirar
  }

  // Curva à direita
  if (isVerdeDireito && !verdeEsq && verdeDir) {
    virarDirGiro90();
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

  // ADIONAR CURVINHA COM DELAY APENAS PARA ALINHAR

  moverTrasPor(300);
  if (isEsquerdo) {
    virarEsqGiro90();
  } else {
    virarDirGiro90();
  }
  moverFrentePor(1150);
  if (isEsquerdo) {
    virarDirGiro90();
  } else {
    virarEsqGiro90();
  }
  moverFrentePor(2460);  // AJUSTAR DE ACORDO COM O TAMANHO DO OBSTACULO // OBJ GRANDE 2500
  if (isEsquerdo) {
    virarDirGiro90();
  } else {
    virarEsqGiro90();
  }
  unsigned long tempoInicial = millis();
  while (millis() - tempoInicial < 1350) {  // AJUSTAR TAMBÉM
    moverFrentePor(1);
    lerReflPrincipal();
    if (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P && sm >= CORTE_QTR_P && sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P) {
      pararMotores();
      delay(500);
      moverFrentePor(250);
      break;
    }
  }
  if (isEsquerdo) {
    virarEsqGiro90();
  } else {
    virarDirGiro90();
  }
  moverTrasPor(250);  // AJUSTAR RÉ
  desligarLed(AMBOS);
}

void rampaOuGangorra() {
  ligarLed(AMBOS, AZUL);
  moverFrentePor(300);
  pararMotores();
  delay(100);

  lerGiroscopioDMP();
  lerUltraDir();
  lerUltraEsq();

#if (GANGORRA == 1)
  // GANGORRA
  if (pitch > INCLINACAO && (distanciaUltraEsq > CORTE_ULTRA_SALA_RESGATE || distanciaUltraEsq > CORTE_ULTRA_SALA_RESGATE)) {
    ligarLed(AMBOS, AZUL);
    servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
    servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
    descerGarraRampa();
    while (pitch > 12) {
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

      lerReflSegueLinha();

      if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P) {
        seguirLinhaEsq(1);
      }

      if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P) {
        seguirLinhaDir(1);
      }
    }
    moverFrenteLento();
    delay(300);
    pararMotores();
    subirGarra();
    servoSubirGarra.detach();
    servoRotacionarGarra.detach();
    moverTrasPor(200);
    pararMotores();

    // adicione aqui se precisar fazer com que identifique algo depois da gangorra
  }
#endif
#if (RAMPA == 1)
  if (pitch > 21) {
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

      lerReflSegueLinha();

      if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P) {
        seguirLinhaEsq(1);
      }

      if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P) {
        seguirLinhaDir(1);
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
      lerReflPrincipal();
      lerReflFrente();

      verificarCruzamento();

      verificarMeioCruzamentoEsq();

      verificarMeioCruzamentoDir();

      // 90 GRAUS DIREITO
      if (sf <= CORTE_FRENTE && (se3 <= CORTE_QTR_P && se2 <= CORTE_QTR_P && se1 <= CORTE_QTR_P) && (sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P)) {
        seguirLinhaDir(12);
      }

      // 90 GRAUS ESQUERDO
      if (sf <= CORTE_FRENTE && (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P) && (sd1 <= CORTE_QTR_P && sd2 <= CORTE_QTR_P && sd3 <= CORTE_QTR_P)) {
        seguirLinhaEsq(12);
      }

      seguidorMoverFrente();
      lerReflSegueLinha();

      if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P) {
        seguirLinhaEsq(3);
      }

      if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P) {
        seguirLinhaDir(3);
      }


      lerGiroscopioDMP();
    }

    if (pitch <= -5) {
      moverFrenteLento();
      delay(1200);
    }
  }
#endif
  desligarLed(AMBOS);
}


void rampaSalaResgate() {
  ligarLed(AMBOS, ROXO);

  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  descerGarraRampa();

  while (true) {
    lerUltraEsq();
    lerUltraDir();

    lerReflPrincipal();

    if (distanciaUltraDir < 10 && (distanciaUltraEsq > 10 && distanciaUltraEsq < 120) && (se2 <= CORTE_QTR_B && se1 <= CORTE_QTR_B && se0 <= CORTE_QTR_B && sd0 <= CORTE_QTR_B && sd1 <= CORTE_QTR_B && sd2 <= CORTE_QTR_B)) {
      break;
    }

    moverFrenteRapido();

    if (se1 >= CORTE_QTR_P || se2 >= CORTE_QTR_P) {
      seguirLinhaEsq(3);
    }

    if (sd1 >= CORTE_QTR_P || sd2 >= CORTE_QTR_P) {
      seguirLinhaDir(3);
    }

    // CRUZAMENTO
    if (se3 >= CORTE_QTR_P && se2 >= CORTE_QTR_P && se1 >= CORTE_QTR_P && se0 >= CORTE_QTR_P && sd0 >= CORTE_QTR_P && sd1 >= CORTE_QTR_P && sd2 >= CORTE_QTR_P && sd3 >= CORTE_QTR_P) {
      ligarLed(AMBOS, BRANCO);
      moverFrenteRapidoPor(400);
      desligarLed(AMBOS);
    }
  }

  salaDeResgate();
}

// fita refletiva aumentar tempo de verificacao para gap