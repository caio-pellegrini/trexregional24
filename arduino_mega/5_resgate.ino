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

// MUDAR COR PARA QUANDO ENCONTRAR VÍTIMA

void salaDeResgate() {
  entrarSalaResgate();

  varredura(false);

  varredura(false);

  varredura(false);

  varredura(false);

  // 2º VARREDURA - ENTREGA QUALQUER BASE

  varredura(true);

  varredura(true);

  varredura(true);

  varredura(true);

  // // 3º VARREDURA - DUAS EXTRAS

  // varredura(true);

  // varredura(true);

  // varredura(true);

  // varredura(true);

  varreduraSaida();
  varreduraSaida();
  varreduraSaida();
  varreduraSaida();
  
  
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

      virarDireitaGiro45();
      moverFrentePor(100);
      virarDireitaGiro45();
    } else {
      // PAREDE ESTA NA DIREITA
      entradaDirecao = true;
      ligarLed(DIR, AZUL, 1000);

      virarEsquerdaGiro45();
      moverFrentePor(100);
      virarEsquerdaGiro45();
    }

    moverTrasPor(1000);
    pararMotores();
  }
}

void varredura(bool entregarQualquerBase) {
  // esta função só termina quando encontrar área, parede ou saída
  // enquanto não encontrar, verifica e recolhe vítimas
  abrirGarra();
  descerGarra();
  laserGarra.startContinuous();
  delay(10);
  moverFrenteLento();

  while (true) {
    moverFrenteLento();
    lerLaserGarra();
    // Serial.println(distanciaLaserGarra);
    if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
      ligarLed(AMBOS, BRANCO);
      pararMotores();
      pegarVitima();
      moverTrasPor(200);
      pararMotores();
      abrirGarra();
      descerGarra();
      moverFrenteLento();
      desligarLed(AMBOS);
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
      encontrouArea(entregarQualquerBase);
      break;
    }

    // reconhecer saída
  }
  laserGarra.stopContinuous();
}

void pegarVitima() {

  fecharGarraVerificaBotao();

  // dupla verificação para ver se realmente há vitima na garra
  lerBtnVitima();
  lerBtnVitima();
  // verifica viva
  if (!btnVitima) {
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

void encontrouArea(bool entregarQualquerBase) {
  moverTrasPor(150);
  pararMotores();

  pegarVitima();
  subirGarra();
  // fecharGarra();
  // subirGarra();

  moverFrentePor(1050);

  if (entradaDirecao) {
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

  if (entradaDirecao) {
    virarDireitaGiro180();

  } else {
    virarEsquerdaGiro180();
  }

  desligarLed(AMBOS);

  moverTrasPor(1500);
  pararMotores();

  // entregar
  if (contadorVitimas != 0) {
    if (!entregarQualquerBase) {
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
    } else {
      abrirCancelaEsq();
      abrirCancelaDir();
      contTotalVivas = contCacambaVivas;
      contCacambaVivas = 0;
      contTotalMortas = contCacambaMortas;
      contCacambaMortas = 0;
      delay(500);
      moverFrentePor(350);
      moverTrasRapidoPor(400);
      moverFrentePor(300);
      moverTrasRapidoPor(400);
      pararMotores();
      delay(500);
      fecharCancelaEsq();
      fecharCancelaDir();
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
  moverTrasPor(300);
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

    if (millis() - tempoInicial > 1500) {
      break;
    }
  }

  pararMotores();
  moverTrasPor(700);
  if (entradaDirecao) {
    virarDireitaGiro90();
  } else {
    virarEsquerdaGiro90();
  }
  moverTrasPor(500); // 1100
  pararMotores();
}

void subirGarraVerificaVitima() {
  uint8_t posicaoFinal = SERVO_SUBIR_GARRA_POS_INICIAL;
  uint8_t velocidade = 8;
  vitimaGarraViva = false;

  uint8_t posicaoAtual = servoSubirGarra.read();
  while (posicaoAtual < posicaoFinal) {
    posicaoAtual++;
    servoSubirGarra.write(posicaoAtual);
    delay(velocidade);
    lerBtnVitima();
    if (btnVitima) {
      vitimaGarraViva = true;
    }
  }
}

void fecharGarraVerificaBotao() {
  uint8_t posicaoFinal = SERVO_PA_GARRA_POS_INICIAL;
  uint8_t velocidade = 3;
  uint8_t posicaoAtual = servoPaGarra.read();
  int8_t passo = posicaoAtual > posicaoFinal ? -1 : 1;

  while (posicaoAtual != posicaoFinal) {
    posicaoAtual += passo;
    servoPaGarra.write(posicaoAtual);
    delay(velocidade);

    lerBtnParede();

    if (btnParedeEsq || btnParedeDir) {
      moverTrasPor(200);
      pararMotores();
      delay(100);
    }
  }
}

void varreduraSaida() {
  // andar na mesma direçãpo da varredura
  // se encontrar parede, virar 90 graus
  // se encontrar area, alinhar e virar
  // se algos dos ultra ver menor que x, virar nessa direção e andar
  // andar até encontrar saída (verificar com refletancia)
  // em outro caso, caso a saida esteka na mesma reta, verificar com refletancia
  // esta função só termina quando encontrar área, parede ou saída
  // enquanto não encontrar, verifica e recolhe vítimas


  abrirGarra();
  descerGarra();
  moverFrenteLento();

  while (true) {
    // ultrassonico
    lerUltraEsq();
    lerUltraDir();

    if (distanciaUltraEsq > 100) {
      pararMotores();
      lerUltraEsq();
      if (distanciaUltraEsq > 100) {
        virarEsquerdaGiro90();
      }
    }
    if (distanciaUltraDir > 100) {
      pararMotores();
      lerUltraDir();
      if (distanciaUltraDir > 100) {
        virarDireitaGiro90();
      }
    }


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
      encontrouArea(true);
      break;
    }

    // reconhecer saída
  }
}