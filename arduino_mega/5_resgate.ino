bool entradaDirecao;  // false = esquerda, true = direita
bool entradaNoMeio;
bool saidaDirecao;      // false = diferente da entrada, true = mesma direção da
                        // entrada
bool direcaoVarredura;  // true = sentido horario, false = antihorario
uint8_t contadorVitimas = 0;
uint8_t contTotalMortas = 0;
uint8_t contTotalVivas = 0;
uint8_t contCacambaMortas = 0;
uint8_t contCacambaVivas = 0;
bool vitimaGarraViva = false;
bool corAreaVermelha;
uint8_t contadorVarreduras = 0;

void salaDeResgate() {
  entrarSalaResgate();
  varredura();

  laserGarra.stopContinuous();
  // delay(10);
  // laserGarra.init();
  laserGarra.startContinuous();
  
  varredura();

  laserGarra.stopContinuous();
  // delay(10);
  // laserGarra.init();
  laserGarra.startContinuous();

  varredura();

  laserGarra.stopContinuous();
  // delay(10);
  // laserGarra.init();
  laserGarra.startContinuous();

  varredura();

  contadorVarreduras++;
  if (contadorVarreduras == 4) {
    pararMotores();
    ligarLed(AMBOS, VERDE);
    delayInfinito();
  }
}

void entrarSalaResgate() {
  // anexar servos
  servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

  laserGarra.startContinuous();

  desligarLed(AMBOS);  // desliga led roxo

  lerUltraEsq();
  lerUltraDir();

  if (distanciaUltraEsq > 20 && distanciaUltraDir > 20) {
    entradaNoMeio = true;
    ligarLed(AMBOS, AZUL, 1000);
    // fazer mais facil primeiro

  } else {
    entradaNoMeio = false;
    if (distanciaUltraEsq < distanciaUltraDir) {
      // PAREDE ESTA NA ESQUERDA
      entradaDirecao = false;
      ligarLed(ESQ, AZUL, 1000);
      virarDireitaGiro90();
    } else {
      // PAREDE ESTA NA DIREITA
      entradaDirecao = true;
      ligarLed(DIR, AZUL, 1000);
      virarEsquerdaGiro90();
    }

    moverTrasPor(1000);
    pararMotores();
  }
}

void varredura() {
  // esta função só termina quando encontrar área, parede ou saída
  // enquanto não encontrar, verifica e recolhe vítimas
  abrirGarra();
  descerGarra();
  moverFrenteLento();

  while (true) {
    // distanciaLaserGarra = 0;
    // // reconhecer vitima
    // if (lerLaserGarraNaoBloquante()) {
    //   if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
    //     pararMotores();
    //     pegarVitima();
    //     moverTrasPor(200);
    //     pararMotores();
    //     abrirGarra();
    //     descerGarra();
    //     moverFrenteLento();
    //   }
    // }
    lerLaserGarra();
    // Serial.println(distanciaLaserGarra);
    if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
      pararMotores();
      pegarVitima();
      moverTrasPor(200);
      pararMotores();
      abrirGarra();
      descerGarra();
      moverFrenteLento();
    }


    // botao da parede bateu
    lerBtnParede();
    if (btnParedeEsq || btnParedeDir) {
      encontrouParede();
      break;
    }

    // botao da area bateu
    lerBtnArea();
    if (btnAreaEsq || btnAreaDir) {
      encontrouArea();
      break;
    }

    // reconhecer saída
  }
}

void pegarVitima() {

  fecharGarraVerificaBotao();

  // dupla verificação para ver se realmente há vitima na garra
  lerBtnVitima();
  lerBtnVitima();
  // verifica viva
  if (!btnVitima) {
    // unsigned long tempoInicial = millis();
    // while (millis() - tempoInicial < 500) {
    //   // verifica viva ou morta
    //   if (lerLaserGarraNaoBloquante()) {
    //     if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
    //       break;
    //     }
    //   }
    // }
    // if (distanciaLaserGarra >= DIST_LASER_GARRA_VIT) {
    //   return;
    // }

    // while (true) {
    //   distanciaLaserGarra = 0;
    //   lerLaserGarraNaoBloquante();
    //   if (distanciaLaserGarra != 0) {
    //     break;
    //   }
    // }
    lerLaserGarra();
    lerLaserGarra();

    if (distanciaLaserGarra >= DIST_LASER_GARRA_VIT) {
      return;
    }
  }


  subirGarraVerificaVitima();

  contadorVitimas++;
  if (vitimaGarraViva) {
    contCacambaVivas++;
    rotacionarGarraDir();
  } else {
    contCacambaMortas++;
    rotacionarGarraEsq();
  }

  abrirGarra();
  delay(200);
  rotacionarGarraMeio();
}

void encontrouArea() {
  moverTrasPor(400);
  pararMotores();

  pegarVitima();
  subirGarra();
  // fecharGarra();
  // subirGarra();

  moverFrentePor(1100);

  if (btnAreaEsq) {
    virarEsquerdaGiro45();
  } else {
    virarDireitaGiro45();
  }

  moverFrentePor(1200);
  pararMotores();

  if (contadorVitimas != 0) {
    // identificar cor da área
    lerTcsFrente();
    lerTcsFrente();

    if (rgbTcsFrente[0] > rgbTcsFrente[1] && rgbTcsFrente[0] > rgbTcsFrente[2]) {
      ligarLed(AMBOS, VERMELHO);
      corAreaVermelha = true;
    } else {
      ligarLed(AMBOS, VERDE);
      corAreaVermelha = false;
    }
  }

  moverTrasPor(500);

  // if direcao
  virarEsquerdaGiro180();
  // virarDireitaGiro180();

  desligarLed(AMBOS);

  moverTrasPor(1500);
  pararMotores();

  // entregar
  if (contadorVitimas != 0) {
    if (!corAreaVermelha && contCacambaVivas > 0) {
      abrirCancelaDir();
      contTotalVivas = contCacambaVivas;
      contCacambaVivas = 0;
      delay(500);
      moverFrentePor(300);
      moverTrasRapidoPor(400);
      moverFrentePor(300);
      moverTrasRapidoPor(400);
      pararMotores();
      delay(500);
      fecharCancelaDir();
    }
    if (corAreaVermelha && contCacambaMortas > 0) {
      abrirCancelaEsq();
      contTotalMortas = contCacambaMortas;
      contCacambaMortas = 0;
      delay(500);
      moverFrentePor(300);
      moverTrasRapidoPor(400);
      moverFrentePor(300);
      moverTrasRapidoPor(400);
      pararMotores();
      delay(500);
      fecharCancelaEsq();
    }
  }

  if (btnAreaEsq) {
    virarEsquerdaGiro45();
  } else {
    virarDireitaGiro45();
  }
  pararMotores();
}

void encontrouParede() {
  moverTrasPor(400);
  pararMotores();

  pegarVitima();
  subirGarra();

  unsigned long tempoInicial = millis();
  while (true) {
    moverFrentePor(1);

    if (lerLaserFrenteNaoBloquante()) {
      if (distanciaLaserFrente < 30) {
        moverFrentePor(300);
        break;
      }
    }

    if (millis() - tempoInicial > 2000) {
      break;
    }
  }

  pararMotores();
  moverTrasPor(600);
  if (entradaDirecao) {
    virarDireitaGiro90();
  } else {
    virarEsquerdaGiro90();
  }
  moverTrasRapidoPor(1000);
  pararMotores();
}

void subirGarraVerificaVitima() {
  uint8_t posicaoFinal = SERVO_SUBIR_GARRA_POS_INICIAL;
  uint8_t velocidade = 6;
  vitimaGarraViva = false;

  uint8_t posicaoAtual = servoSubirGarra.read();
  while (posicaoAtual < posicaoFinal) {
    posicaoAtual++;
    servoSubirGarra.write(posicaoAtual);
    delay(6);
    lerBtnVitima();
    if (btnVitima) {
      vitimaGarraViva = true;
    }
  }
}

void fecharGarraVerificaBotao() {
  uint8_t posicaoFinal = SERVO_PA_GARRA_POS_INICIAL;
  uint8_t velocidade = 5;
  uint8_t posicaoAtual = servoPaGarra.read();
  int8_t passo = posicaoAtual > posicaoFinal ? -1 : 1;

  bool btnAntes[4] = { btnAreaEsq, btnAreaDir, btnParedeEsq, btnParedeDir };

  while (posicaoAtual != posicaoFinal) {
    posicaoAtual += passo;
    servoPaGarra.write(posicaoAtual);
    delay(velocidade);

    lerBtnArea();
    lerBtnParede();

    if (btnAreaEsq || btnAreaDir || btnParedeEsq || btnParedeDir) {
      break;
    }
  }
  btnAreaEsq = btnAntes[0];
  btnAreaDir = btnAntes[1];
  btnParedeEsq = btnAntes[2];
  btnParedeDir = btnAntes[3];

  if (posicaoAtual != posicaoFinal) {
    moverTrasPor(75);
    pararMotores();
    abrirGarra();
    fecharGarraVerificaBotao();
  }
}

void identificarCorArea() {
}