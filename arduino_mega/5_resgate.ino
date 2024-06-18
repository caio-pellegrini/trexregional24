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
}

void entrarSalaResgate() {
  // anexar servos
  servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

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

      // if (distanciaUltraDir != 0 && distanciaUltraDir <= 2) {
      //   virarEsquerdaGiro45();
      //   moverFrentePor(200);
      //   virarEsquerdaGiro45();
      // } else {
      //   virarEsquerdaGiro90();
      // }
    }

    moverTrasPor(1000);
    pararMotores();
    abrirGarra();
    descerGarra();
  }
}

void varredura() {
  // esta função só termina quando encontrar área, parede ou saída
  // enquanto não encontrar, verifica e recolhe vítimas
  moverFrenteLento();
  while (true) {

    // reconhecer vitima
    if (lerLaserGarraNaoBloquante()) {
      if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
        pararMotores();
        pegarVitima();
        moverTrasPor(200);
        pararMotores();
        abrirGarra();
        descerGarra();
        moverFrenteLento();
      }
    }

    // botao da parede bateu
    lerBtnParede();
    if (btnParedeEsq || btnParedeDir) {
      encontrouParede();
      abrirGarra();
      descerGarra();
      break;
    }

    // botao da area bateu
    lerBtnArea();
    if (btnAreaEsq || btnAreaDir) {
      encontrouArea();
      abrirGarra();
      descerGarra();
      break;
    }

    // reconhecer saída
  }

  contadorVarreduras++;
  if (contadorVarreduras == 4) {
    pararMotores();
    ligarLed(AMBOS, VERDE);
    delayInfinito();
  }
  varredura();
}

void pegarVitima() {

  fecharGarra();

  // dupla verificação para ver se realmente há vitima na garra
  while (true) {
    if (lerLaserGarraNaoBloquante()) {
      if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
        break;
      } else {
        ligarLed(AMBOS, VERMELHO, 1000);
        return;
      }
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

void subirGarraVerificaVitima() {
  vitimaGarraViva = false;
  uint8_t posicaoAtual = servoSubirGarra.read();
  while (posicaoAtual < 160) {
    posicaoAtual++;
    servoSubirGarra.write(posicaoAtual);
    delay(7);
    lerBtnVitima();
    if (btnVitima) {
      vitimaGarraViva = true;
    }
  }
}

void identificarCorArea() {
  lerTcsFrente();
  lerTcsFrente();

  if (rgbTcsFrente[0] > rgbTcsFrente[1] && rgbTcsFrente[0] > rgbTcsFrente[2]) {
    ligarLed(AMBOS, VERMELHO, 1000);
    corAreaVermelha = true;
  } else {
    ligarLed(AMBOS, VERDE, 1000);
    corAreaVermelha = false;
  }
}

void encontrouArea() {
  moverTrasPor(500);
  pararMotores();

  fecharGarra();
  subirGarraVerificaVitima();

  // se houver alguma vitima na garra, parar para ler
  // se nao, continuar a varredura

  // if (contadorVitimas == 0) {
  //   if (direcaoVarredura) {
  //     virarEsquerdaGiro90();
  //   } else {
  //     virarDireitaGiro90();
  //   }
  //   pararMotores();

  //   return;
  // }

  moverFrentePor(1100);

  if (btnAreaEsq) {
    virarEsquerdaGiro45();
  } else {
    virarDireitaGiro45();
  }

  moverFrentePor(1200);
  pararMotores();

  if (contadorVitimas != 0) {
    identificarCorArea();
  }

  moverTrasPor(500);

  // if direcao
  virarEsquerdaGiro180();
  // virarDireitaGiro180();

  moverTrasPor(1500);
  pararMotores();

  // entregar
  if (contadorVitimas != 0) {
    if (!corAreaVermelha && contCacambaVivas > 0) {
      abrirCancelaDir();
      contTotalVivas = contCacambaVivas;
      contCacambaVivas = 0;
    }
    if (corAreaVermelha && contCacambaMortas > 0) {
      abrirCancelaEsq();
      contTotalMortas = contCacambaMortas;
      contCacambaMortas = 0;
    }
    delay(500);
    moverFrentePor(300);
    moverTrasRapidoPor(400);
    moverFrentePor(300);
    moverTrasRapidoPor(400);
    pararMotores();
    delay(500);
    if (!corAreaVermelha) {
      fecharCancelaDir();
    } else {
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
  moverTrasPor(500);
  pararMotores();

  fecharGarra();
  subirGarraVerificaVitima();

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
  if (direcaoVarredura) {
    virarEsquerdaGiro90();
  } else {
    virarDireitaGiro90();
  }
  moverTrasRapidoPor(1000);
  pararMotores();
}