void verificarCruzamento() {
  if (se3 >= LUZ_PRETO && se2 >= LUZ_PRETO && se1 >= LUZ_PRETO && sm >= LUZ_PRETO && sd1 >= LUZ_PRETO && sd2 >= LUZ_PRETO && sd3 >= LUZ_PRETO) {
    ligarLed(AMBOS, AMARELO);
    TEMPO_MOVER_ANTES_ANALISAR_VERDE
    pararMotores();
    analisarVerde(1, 1, 1);  // 1, 1, 1
    desligarLed(AMBOS);
  }
}

void verificarMeioCruzamentoEsq() {
  if (sf >= LUZ_FRENTE && sm >= LUZ_PRETO && (se3 >= LUZ_PRETO && se2 >= LUZ_PRETO && se1 >= LUZ_PRETO) && (sd1 <= LUZ_PRETO && sd2 <= LUZ_PRETO && sd3 <= LUZ_BRANCO)) { // sd3 <= LUZ_BRANCO
    ligarLed(ESQ, AMARELO);
    TEMPO_MOVER_ANTES_ANALISAR_VERDE
    pararMotores();
    analisarVerde(0, 1, 1);  // 0, 1, 0
    desligarLed(AMBOS);
  }
}

void verificarMeioCruzamentoDir() {
  if (sf >= LUZ_FRENTE && sm >= LUZ_PRETO && (se3 <= LUZ_PRETO && se2 <= LUZ_PRETO && se1 <= LUZ_BRANCO) && (sd1 >= LUZ_PRETO && sd2 >= LUZ_PRETO && sd3 >= LUZ_PRETO)) {
    ligarLed(DIR, AMARELO);
    TEMPO_MOVER_ANTES_ANALISAR_VERDE
    pararMotores();
    analisarVerde(0, 1, 1);  // 0, 0, 1
    desligarLed(AMBOS);
  }
}

void verificar90GrausDir() {
  if (sf <= LUZ_FRENTE && sm >= LUZ_PRETO && (se3 <= LUZ_PRETO && se2 <= LUZ_PRETO) && (sd1 >= LUZ_PRETO && sd2 >= LUZ_PRETO && sd3 >= LUZ_PRETO)) {
    ligarLed(DIR, VERMELHO);
    for (uint8_t i = 0; i < 10; i++) {  // 5
      virarDirPor(5);
      moverFrentePor(2); // 2
    }
    pararMotores();
    desligarLed(DIR);
  }
}

void verificar90GrausEsq() {
  if (sf <= LUZ_FRENTE && sm >= LUZ_PRETO && (se3 >= LUZ_PRETO && se2 >= LUZ_PRETO && se1 >= LUZ_PRETO) && (sd2 <= LUZ_PRETO && sd3 <= LUZ_PRETO)) {
    ligarLed(ESQ, VERMELHO);
    for (uint8_t i = 0; i < 10; i++) {
      virarEsqPor(5);
      moverFrentePor(2);
    }
    pararMotores();
    desligarLed(ESQ);
  }
}

void graus90gradual(bool lado) { // 0 esq e 1 dir
  for (uint8_t i = 0; i < 8; i++) {  // 5
  if (lado == 0) virarEsqPor(5);
  else virarDirPor(5);
  moverFrentePor(2); 
  }
}

void verificarSegueLinhaEsq() {

}

void verificarGap() {
  ligarLed(AMBOS, BRANCO);

  // moverFrentePor(50);  // O QUANTO DEVE ANDAR ATÉ ENCONTRAR A FITA VERMELHA

  pararMotores();
  delay(150);
  lerTcsAmbos();
  lerTcsAmbos();

  if (rgbTcsEsq[0] > rgbTcsEsq[1] && rgbTcsEsq[0] > rgbTcsEsq[2] && rgbTcsDir[0] > rgbTcsDir[1] && rgbTcsDir[0] > rgbTcsDir[2]) {
    ligarLed(AMBOS, VERMELHO);
    delayInfinito();
    return;
  }

  moverFrentePor(80);  //  AJUSTE PARA PULAR LINHA PRETA EM CRUZAMENTOS

  uint8_t valor = (LUZ_PRETO + LUZ_BRANCO) / 2;

  unsigned long tempoInicial = millis();

  while (true) {

    lerReflPrincipal();

    if (se3 > valor || se2 > valor || se1 > valor || sm > valor || sd1 > valor || sd2 > valor || sd3 > valor) {
      desligarLed(AMBOS);
      pararMotores();
      delay(200);
      break;
    }

    moverFrenteLentoPor(1);

    // LER ULTRA DA PAREDE SE SAIDA FOR MEIO LADO
    if (millis() - tempoInicial >= 1100 && millis() - tempoInicial <= 1300) {
      pararMotores();
      delay(10);
      lerUltraEsq();
      lerUltraDir();
    }

    // VALOR DE ENTRADA PARA SALA DE RESGATE
    if (millis() - tempoInicial >= 2200) {
      pararMotores();
      desligarLed(AMBOS);
      delay(250);
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
    // COMENTE ESSA LINHA SE HOUVER CIRCULO
    // virarEsqGiro90(); 

    //  DESCOMENTE ABAIXO APENAS SE HOUVER CIRCULO
    pararMotores();
    delay(50);
    lerGiroscopioDMP();
    lerGiroscopioDMP();
    if ((abs(yaw - curvaYaw) > 30 && abs(yaw - curvaYaw) < 90) && contadorCurva90 != 0) {
      ligarLed(ESQ, AZUL);
      virarEsqGiro45();
      desligarLed(AMBOS);
    } else {
      virarEsqGiro90(); //

      moverTrasPor(50); // testar se não irá atrapalhar outras partes
      pararMotores();
      delay(50);
      lerGiroscopioDMP();
      lerGiroscopioDMP();
      curvaYaw = yaw;
      contadorCurva90++;
    }

    
  }

  // Curva à direita
  if (isVerdeDireito && !verdeEsq && verdeDir) {
    // COMENTE ESSA LINHA SE HOUVER CIRCULO
    // virarDirGiro90();

    //  DESCOMENTE ABAIXO APENAS SE HOUVER CIRCULO
    pararMotores();
    delay(50);
    lerGiroscopioDMP();
    lerGiroscopioDMP();
    if ((abs(yaw - curvaYaw) > 30 && abs(yaw - curvaYaw) < 90) && contadorCurva90 != 0) {
      ligarLed(DIR, AZUL);
      virarDirGiro45();
      desligarLed(AMBOS);
    } else {
      virarDirGiro90(); // apenas essa
      moverTrasPor(50); // testar se não irá atrapalhar outras partes
      
      pararMotores();
      delay(50);
      lerGiroscopioDMP();
      lerGiroscopioDMP();
      curvaYaw = yaw;
      contadorCurva90++;
    }
    
  }

  // Seguir reto
  if (!verdeEsq && !verdeDir) {
    moverFrentePor(150); // 200
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
    if (se3 >= LUZ_PRETO && se2 >= LUZ_PRETO && se1 >= LUZ_PRETO && sm >= LUZ_PRETO && sd1 >= LUZ_PRETO && sd2 >= LUZ_PRETO && sd3 >= LUZ_PRETO) {
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
  moverFrentePor(250);
  pararMotores();
  delay(50);

  lerGiroscopioDMP();

  if (pitch <= INCLINACAO) return;  // garantir que não pegou uma falsa leitura

  moverFrentePor(200);
  pararMotores();
  delay(50);

  lerGiroscopioDMP();

  lerUltraDir();
  lerUltraEsq();

#if (GANGORRA == 1)
  // GANGORRA
  if ((pitch > INCLINACAO && pitch < INCLI_RAMPA) && (distanciaUltraEsq > CORTE_ULTRA_SALA_RESGATE || distanciaUltraEsq > CORTE_ULTRA_SALA_RESGATE)) {
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

      if (se1 >= LUZ_PRETO || se2 >= LUZ_PRETO) {
        seguirLinhaEsq(1);
      }

      if (sd1 >= LUZ_PRETO || sd2 >= LUZ_PRETO) {
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
  if (pitch > INCLI_RAMPA) {
    // RAMPA

    ligarLed(AMBOS, VERMELHO);
    servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
    servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
    movimentarServo(&servoSubirGarra, 30, 4);  // desce servo (Ram ou Gan 30 / rampa sr 15)
    unsigned long tempo5 = millis();

    while (pitch > 5) {
      analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(48));
      analogWrite(MOTOR_ESQ_T_PIN, 0);
      analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(48));
      analogWrite(MOTOR_DIR_T_PIN, 0);
      delay(3);

      // SE HOUVER LOMBADA
      analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(95));
      analogWrite(MOTOR_ESQ_T_PIN, 0);
      analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(95));
      analogWrite(MOTOR_DIR_T_PIN, 0);
      delay(1);

      if (millis() - tempo5 > 2500) {
        movimentarServo(&servoSubirGarra, 0, 1);
      } else {
        lerReflSegueLinha();

        if (se1 >= LUZ_PRETO || se2 >= LUZ_PRETO) seguirLinhaEsq(1);
        if (sd1 >= LUZ_PRETO || sd2 >= LUZ_PRETO) seguirLinhaDir(1);
      }

      lerGiroscopioDMP();
    }

    pararMotores();
    subirGarra();
    servoSubirGarra.detach();
    servoRotacionarGarra.detach();
    // moverTrasPor(100);
    pararMotores();

    delay(40);
    lerGiroscopioDMP();

    while (pitch > -5) {
      // AJUSTE PARA OS DESAFIOS QUE TIVER NA RAMPA

      lerGiroscopioDMP();
      


      // lerReflTodos();
      // verificarCruzamento();
      // verificarMeioCruzamentoEsq();
      // verificarMeioCruzamentoDir();
      
      lerReflSegueLinha();
      seguidorMoverFrente();
      delay(1);
      if (sd1 >= LUZ_PRETO || sd2 >= LUZ_PRETO || sd3 >= LUZ_PRETO) seguirLinhaDir(7);
      if (se1 >= LUZ_PRETO || se2 >= LUZ_PRETO || se3 >= LUZ_PRETO) seguirLinhaEsq(7);
      

      lerReflTodos();

      // verificar90GrausEsq();
      // verificar90GrausDir();

      
    }

    ligarLed(AMBOS, VERMELHO);

    if (pitch <= -4) {
      ligarLed(AMBOS, AMARELO);
      moverFrenteLentoPor(100);
      unsigned long tempo = millis();
      while (millis() - tempo < 1000) {
        analogWrite(MOTOR_ESQ_F_PIN, CONVERT_8B_DEC(25));
        analogWrite(MOTOR_ESQ_T_PIN, 0);
        analogWrite(MOTOR_DIR_F_PIN, CONVERT_8B_DEC(25));
        analogWrite(MOTOR_DIR_T_PIN, 0);
        delay(3);

        lerReflSegueLinha();
        if (sd1 >= LUZ_PRETO || sd2 >= LUZ_PRETO || sd3 >= LUZ_PRETO) seguirLinhaDir(3);
        if (se1 >= LUZ_PRETO || se2 >= LUZ_PRETO || se3 >= LUZ_PRETO) seguirLinhaEsq(3);
      }
      pararMotores();
      delay(1000);
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

    if (distanciaUltraDir < 10 && (distanciaUltraEsq > 10 && distanciaUltraEsq < 120) && (se2 <= LUZ_BRANCO && se1 <= LUZ_BRANCO && se0 <= LUZ_BRANCO && sd0 <= LUZ_BRANCO && sd1 <= LUZ_BRANCO && sd2 <= LUZ_BRANCO)) {
      break;
    }

    moverFrenteRapido();

    if (se1 >= LUZ_PRETO || se2 >= LUZ_PRETO) {
      seguirLinhaEsq(3);
    }

    if (sd1 >= LUZ_PRETO || sd2 >= LUZ_PRETO) {
      seguirLinhaDir(3);
    }

    // CRUZAMENTO
    if (se3 >= LUZ_PRETO && se2 >= LUZ_PRETO && se1 >= LUZ_PRETO && se0 >= LUZ_PRETO && sd0 >= LUZ_PRETO && sd1 >= LUZ_PRETO && sd2 >= LUZ_PRETO && sd3 >= LUZ_PRETO) {
      ligarLed(AMBOS, BRANCO);
      moverFrenteRapidoPor(400);
      desligarLed(AMBOS);
    }
  }

  salaDeResgate();
}