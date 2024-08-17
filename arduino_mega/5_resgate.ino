bool entradaDirecaoEsq;  // false = esquerda, true = direita
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
bool podeSair = false;
uint8_t numero = 0;
bool deveSair = false;
bool entregarQualquerBase = false;
uint8_t numeroVarreduraEnt = 5;
// 4 se for entrada canto, outros valores ajustar
bool lerEntradaAntes = true;

// MUDAR COR PARA QUANDO ENCONTRAR VÍTIMA

void salaDeResgate() {
  entrarSalaResgate();

  // varreduraMeioEntradaMeio(); // apenas para entrada no meio

  if (!deveSair) varredura(1);

  if (!deveSair) varredura(2);

  if (!deveSair) varredura(3);

  if (!deveSair) varredura(4);

  if (!deveSair) varreduraMeio(false);

  // 2º VARREDURA - 

  if (!deveSair) varredura(1);

  if (!deveSair) varredura(2);

  if (!deveSair) varredura(3);

  if (!deveSair) varredura(4);

  
  // 3º VARREDURA - ENTREGA QUALQUER BASE E PODE SAIR SE ENCONTRAR

  entregarQualquerBase = true;
  podeSair = true;

  if (!deveSair) varredura(1);

  if (!deveSair) varredura(2);

  if (!deveSair) varredura(3);

  if (!deveSair) varredura(4);

  ligarLed(AMBOS, ROXO, 500);
  delay(500);
  ligarLed(AMBOS, ROXO, 500);
}

void entrarSalaResgate() {
  ligarLed(AMBOS, ROXO, 500);

  // anexar servos
  servoPaGarra.attach(SERVO_PA_GARRA_PIN);
  servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
  servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
  servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
  servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

  laserGarra.startContinuous();
  delay(10);

  if (lerEntradaAntes == false) {
    lerUltraEsq();
    lerUltraDir();
  }

  if (distanciaUltraEsq > 20 && distanciaUltraDir > 20) {
    entradaNoMeio = true;
    // entradaDirecaoEsq = true; // AJUSTE DE ACORDO COM A DIREÇÃO

    ligarLed(AMBOS, AZUL);

    // moverTrasPor(100);
    // virarDirGiro90();
    moverTrasPor(1000);
    pararMotores();

    desligarLed(AMBOS);

  } else {
    entradaNoMeio = false;
    if (distanciaUltraEsq < distanciaUltraDir) {
      // PAREDE ESTA NA ESQUERDA
      entradaDirecaoEsq = true; //true
      ligarLed(ESQ, AZUL);

      virarDirGiro45();
      moverFrentePor(80);
      virarDirGiro45();
    } else {
      // PAREDE ESTA NA DIREITA
      entradaDirecaoEsq = false; // false
      ligarLed(DIR, AZUL);

      virarEsqGiro45();
      moverFrentePor(80);
      virarEsqGiro45();
    }

    alinharComFc(1000);
    pararMotores();

    // MUDAR AQUI ENTRADA
    moverFrentePor(500);
    virarEsqGiro45();
    moverFrentePor(80);
    virarEsqGiro45();
    moverTrasPor(100);
    pararMotores();
    
    desligarLed(AMBOS);
  }
}

void varredura(uint8_t num) {
  // esta função só termina quando encontrar área, parede ou saída
  // enquanto não encontrar, verifica e recolhe vítimas
  numero = num;
  abrirGarra();
  descerGarra();
  laserGarra.startContinuous();
  delay(10);
  moverFrenteLento();

  unsigned long tempu = millis();

  while (true) {
    moverFrenteLento();
    lerLaserGarra();
    // Serial.println(distanciaLaserGarra);
    if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
      ligarLed(AMBOS, BRANCO);
      pararMotores();
      verificarPegarVitima();
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
      encontrouParede(true);
      break;
    }

    // botao da area bateu
    lerBtnArea();
    if (btnAreaEsq || btnAreaDir) {
      encontrouArea();
      break;
    }

    // reconhecer saída
    if (millis() - tempu > 2000 || numero != 0) { // trocar para 1 para reconhecer saida mesmo
      if (entradaDirecaoEsq == 0) {
        lerUltraDir();
        if (distanciaUltraDir > 60) {
          verificarSaida(DIR);
          if (deveSair) break;
          if (numero == numeroVarreduraEnt) break;
        }
      } else {
        lerUltraEsq();
        if (distanciaUltraEsq > 60) {
          verificarSaida(ESQ);
          if (deveSair) break;
          if (numero == numeroVarreduraEnt) break;
        }
      }
    }
  }

  laserGarra.stopContinuous();
}

void verificarPegarVitima() {

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

void encontrouArea() {
  moverTrasPor(150);
  pararMotores();

  verificarPegarVitima();
  subirGarra();

  moverFrentePor(1050);

  if (entradaDirecaoEsq) virarEsqGiro45();
  else virarDirGiro45();

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

  if (entradaDirecaoEsq) virarDirGiro180();
  else virarEsqGiro180();

  desligarLed(AMBOS);

  alinharComFc(0);
  pararMotores();

  // entregar
  if (contadorVitimas != 0) {
    if (!entregarQualquerBase) {
      if (!corAreaVermelha && contCacambaVivas > 0) {
        abrirCancelaDir();
        contTotalVivas = contCacambaVivas;
        contCacambaVivas = 0;
        delay(750);
        // moverFrentePor(250);
        // alinharComFc(0);
        // moverFrentePor(250);
        // alinharComFc(0);
        // pararMotores();
        // delay(500);
        fecharCancelaDir();
      }
      if (corAreaVermelha && contCacambaMortas > 0) {
        abrirCancelaEsq();
        contTotalMortas = contCacambaMortas;
        contCacambaMortas = 0;
        delay(750);
        // moverFrentePor(250);
        // alinharComFc(0);
        // moverFrentePor(250);
        // alinharComFc(0);
        // pararMotores();
        // delay(500);
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

  if (btnAreaEsq) virarEsqGiro45();
  else virarDirGiro45();
  
  pararMotores();
}

void encontrouParede(bool alinharNoFinal) {
  ligarLed(AMBOS, CIANO);
  moverTrasPor(300);
  pararMotores();
  desligarLed(AMBOS);

  verificarPegarVitima();
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

    if (millis() - tempoInicial > 1500) break;
  }

  pararMotores();
  moverTrasPor(700);
  if (entradaDirecaoEsq) virarDirGiro90();
  else virarEsqGiro90();

  if (alinharNoFinal) {
    if (numero == numeroVarreduraEnt) alinharComFc(0);
    else moverTrasPor(650);
  }
  pararMotores();

  laserGarra.stopContinuous();
}

// VARREDURAS NO MEIO DA SALA -----

void varreduraMeio(bool entregarQualquerBase) {
  // INICIO VARREDURA MEIO
  moverFrentePor(500);
  virarDirGiro90();
  pararMotores();

  abrirGarra();
  descerGarra();
  laserGarra.startContinuous();
  delay(10);

  unsigned long tempo = millis();

  do {
    moverFrenteLento();
    lerLaserGarra();

    if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
      ligarLed(AMBOS, BRANCO);
      pararMotores();
      verificarPegarVitima();
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
      moverTrasPor(50);
      pararMotores();
      break;
    }

    // botao da area bateu
    lerBtnArea();
    if (btnAreaEsq || btnAreaDir) {
      moverTrasPor(50);
      pararMotores();
      break;
    }
  } while (millis() - tempo <= 2000); // AJUSTAR ESSE VALOR

  pararMotores();
  verificarPegarVitima();
  subirGarra();

  moverFrenteLentoPor(500); // 1000
  pararMotores();

  virarEsqGiro90(); // adicionar outra direcao aqui

  alinharComFc(1000);
  
  pararMotores();
  abrirGarra();
  descerGarra();

  // varre o meio da sala
  while (true) {
    moverFrenteLento();
    lerLaserGarra();
    // Serial.println(distanciaLaserGarra);
    if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
      ligarLed(AMBOS, BRANCO);
      pararMotores();
      verificarPegarVitima();
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
      encontrouParede(false);
      break;
    }

    // // botao da area bateu
    // lerBtnArea();
    // if (btnAreaEsq || btnAreaDir) {
    //   encontrouArea();
    //   break;
    // }
  }

  pararMotores();
  verificarPegarVitima();
  subirGarra();

  virarEsqGiro90(); // mudar aqui

  alinharComFc(1000);

  pararMotores();
  abrirGarra();
  descerGarra();
  
  while (true) // voltar para parede inicial
  {
    moverFrenteLento();
    lerLaserGarra();
    // Serial.println(distanciaLaserGarra);
    if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
      ligarLed(AMBOS, BRANCO);
      pararMotores();
      verificarPegarVitima();
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
      encontrouParede(false);
      break;
    }

    // botao da area bateu
    lerBtnArea();
    if (btnAreaEsq || btnAreaDir) {
      encontrouArea();
      break;
    }
  }
  moverTrasPor(400);
  pararMotores();

  // FINAL VARREDURA
  virarEsqGiro90();
  pararMotores();

  laserGarra.stopContinuous();

}

// NÃO USAR SE ENTRADA E SAÍDA ESTIVEREM NA MESMA RETA
void varreduraMeioEntradaMeio() {
  abrirGarra();
  descerGarra();
  laserGarra.startContinuous();
  delay(10);
  
   while (true) {
    moverFrenteLento();
    lerLaserGarra();
    // Serial.println(distanciaLaserGarra);
    if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
      ligarLed(AMBOS, BRANCO);
      pararMotores();
      verificarPegarVitima();
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
      encontrouParede(false);
      break;
    }

    // botao da area bateu
    lerBtnArea();
    if (btnAreaEsq || btnAreaDir) {
      // encontrouArea();
      encontrouParede(false);
      break;
    }
  }

  moverTrasPor(400);
  pararMotores();
  
  laserGarra.stopContinuous();
}

// SAIDAS -----
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
        virarEsqGiro90();
      }
    }
    if (distanciaUltraDir > 100) {
      pararMotores();
      lerUltraDir();
      if (distanciaUltraDir > 100) {
        virarDirGiro90();
      }
    }

    lerLaserGarra();
    // Serial.println(distanciaLaserGarra);
    if (distanciaLaserGarra < DIST_LASER_GARRA_VIT) {
      pararMotores();
      verificarPegarVitima();
      moverTrasPor(200);
      pararMotores();
      abrirGarra();
      descerGarra();
      moverFrenteLento();
    }

    // botao da parede bateu
    lerBtnParede();
    if (btnParedeEsq || btnParedeDir) {
      encontrouParede(true);
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

void verificarSaida(bool esq, bool dir) {
  pararMotores();
  if (numero == numeroVarreduraEnt) { // MUDAR ESSE NUMERO DE ACORDO COM A SAÍDA (4 SE ENTRADA FOR CANTO)
    ligarLed(esq, dir, VERDE);
    fecharGarra();
    subirGarra();
    moverTrasPor(1500);

    if (entradaDirecaoEsq) {
      virarDirGiro45();
      moverFrentePor(80);
      virarDirGiro45();
    } else {
      virarEsqGiro45();
      moverFrentePor(80);
      virarEsqGiro45();
    }
    desligarLed(esq, dir);
    alinharComFc(0);
    pararMotores();
  } else {
    ligarLed(esq, dir, VERMELHO);
    if (contadorVitimas >= 3 || podeSair) {
      for (uint8_t i = 0; i < 3; i++) {
        ligarLed(esq, dir, VERMELHO, 250);
        delay(250);
      }
      sairSalaResgate();
    }
  }
}

void sairSalaResgate() {
  
  fecharGarraVerificaBotao();
  subirGarra();
  moverFrentePor(900);

  if (entradaDirecaoEsq) virarEsqGiro90();
  else virarDirGiro90();

  do {
    moverFrentePor(1);
    lerReflPrincipal();
  } while (se3 < LUZ_PRETO && se2 < LUZ_PRETO && se1 < LUZ_PRETO && sm < LUZ_PRETO && sd1 < LUZ_PRETO && sd2 < LUZ_PRETO && sd3 < LUZ_PRETO);
  moverFrentePor(250);
  pararMotores();
  deveSair = true;
}

// FUNÇÕES PARA GARRA -------

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

    if (btnVitima) vitimaGarraViva = true;
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

// OUTRAS

void alinharComFc(unsigned long tempoMax) {
  // tempoMax -> 0 se não houver tempo máximo de ré
  unsigned long tempo = millis();
  do {
    lerBtnFc();
    if (!btnFcEsq && !btnFcDir) {
      moverTrasPor(1);
    } else {
      if (!btnFcEsq && btnFcDir) {
        analogWrite(MOTOR_ESQ_T_PIN, CONVERT_8B_DEC(VEL_MOTOR_TRAS));
        analogWrite(MOTOR_DIR_T_PIN, 0);
        delay(1);
      }
      if (btnFcEsq && !btnFcDir) {
        analogWrite(MOTOR_ESQ_T_PIN, 0);
        analogWrite(MOTOR_DIR_T_PIN, CONVERT_8B_DEC(VEL_MOTOR_TRAS));
        delay(1);
      }
    }

    if (tempoMax != 0 && millis() - tempo >= tempoMax) break;

  } while ((!btnFcEsq || !btnFcDir));

  moverTrasPor(65);
}
