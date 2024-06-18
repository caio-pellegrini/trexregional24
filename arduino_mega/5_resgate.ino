bool entradaDirecao; // false = esquerda, true = direita
bool entradaNoMeio;
bool saidaDirecao; // false = diferente da entrada, true = mesma direção da
                   // entrada
uint8_t contadorVitimas = 0;
bool vitimaGarraViva = false;

void entrarSalaResgate() {

  // attach servos
  servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

  desligarLed(AMBOS); // desliga led roxo

  // reconhecerPegarVitima();

  identificarEntradaDirecao();

  abrirPas();
  descerGarra();

  moverFrente();
  // moverFrente lentamente (?)

  while (true) {
    // botao da area bateu
    lerBtnArea();
    if (btnAreaEsq || btnAreaDir) {
      pararMotores();
      // delayInfinito();
      break;
    }

    // botao da parede bateu
    lerBtnParede();
    if (btnParedeEsq || btnParedeDir) {
      pararMotores();
      delayInfinito();
      // break;
    }
  }

  moverTrasPorMS(200);
  pararMotores();

  fecharPas();
  subirGarraVerificaVitima();

  // se houver alguma vitima na garra, parar para ler
  // se nao, continuar a varredura

  if (contadorVitimas == 0) {
    pararMotores();
    delayInfinito();
  }

  moverFrentePorMS(800);

  if (btnAreaEsq) {
    // virarEsquerdaGiro90();
    virarEsquerdaGiro45();
  } else {
    // virarDireitaGiro90();
    virarDireitaGiro45();
  }

  moverFrentePorMS(1200);
  pararMotores();

  identificarCorArea();

  moverTrasPorMS(500);

  virarEsquerdaGiro180();
  // virarDireitaGiro180();

  moverTrasPorMS(1500);
  pararMotores();

  abrirCancelaDir();
  abrirCancelaEsq();
  delay(500);
  fecharCancelaDir();
  fecharCancelaEsq();
}

void pegarVitima() {
  // reconhecer
  while (true) {
    lerLaserGarra();
    if (distanciaLaserGarra < 50) {
      break;
    }
  }

  fecharPas();
  subirGarraVerificaVitima();

  if (vitimaGarraViva) {
    rotacionarGarraDir();
  } else {
    rotacionarGarraEsq();
  }

  abrirPas();
  delay(400);
  fecharPas();
  rotacionarGarraMeio();
  delay(400);
}

void identificarCorArea() {
  lerTcsFrente();
  lerTcsFrente();

  if (rgbTcsFrente[0] > rgbTcsFrente[1] && rgbTcsFrente[0] > rgbTcsFrente[2]) {
    ligarLed(AMBOS, VERMELHO, 1000);
  } else {
    ligarLed(AMBOS, VERDE, 1000);
  }
}

void identificarEntradaDirecao() {
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

      // if (distanciaUltraEsq != 0 && distanciaUltraEsq <= 2)
      // {
      //   virarDireitaGiro45();
      //   moverFrentePorMS(200);
      //   virarDireitaGiro45();
      // }
      // else
      // {
      //   virarDireitaGiro90();
      // }

    } else {
      // PAREDE ESTA NA DIREITA
      entradaDirecao = true;
      ligarLed(DIR, AZUL, 1000);

      virarEsquerdaGiro90();

      // if (distanciaUltraDir != 0 && distanciaUltraDir <= 2)
      // {
      //   virarEsquerdaGiro45();
      //   moverFrentePorMS(200);
      //   virarEsquerdaGiro45();
      // }
      // else
      // {
      //   virarEsquerdaGiro90();
      // }
    }

    moverTrasPorMS(1000);
    pararMotores();
    // delayInfinito();
  }
}