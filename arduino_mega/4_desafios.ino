void analisarVerde(bool isBeco, bool isVerdeEsquerdo, bool isVerdeDireito)
{
  lerVerde();
  lerVerde();

  desligarLed(AMBOS);

  bool verdeEsq = false;
  bool verdeDir = false;

  if (rgbEsq[1] < CORTE_VERDE_ESQ && rgbEsq[0] < CORTE_VERMELHO_CRUZ)
  {
    ligarLed(ESQ, VERDE, 0);
    verdeEsq = true;
  }

  if (rgbDir[1] != 0)
  {
    if (rgbDir[1] < CORTE_VERDE_DIR && rgbDir[0] < CORTE_VERMELHO_CRUZ)
    {
      ligarLed(DIR, VERDE, 0);
      verdeDir = true;
    }
  }
  else
  {
    ligarLed(DIR, VERMELHO, 0); // avisa que o rgbdir não recebeu dados do TCS
  }

  moverFrentePorMS(TEMPO_MOVER_ANTES_CRUZ); // mover pra frente antes de virar 
  
  // Beco sem saida
  if (isBeco && verdeEsq && verdeDir)
  {
    virarEsquerdaGiro(210);
    moverFrentePorMS(300);
  }

  // Curva à esquerda
  if (isVerdeEsquerdo && verdeEsq && !verdeDir)
  {
    virarEsquerdaGiro90();
    moverFrentePorMS(200);
  }

  // Curva à direita
  if (isVerdeDireito && !verdeEsq && verdeDir)
  {
    virarDireitaGiro90();
    moverFrentePorMS(200);
  }

  // Seguir reto
  if (!verdeEsq && !verdeDir)
  {
    moverFrentePorMS(200);
  }

  pararMotor();
}

void desviarObstaculo() {
    pararMotor();
    ligarLed(AMBOS, VERMELHO, 0);
    moverTrasPorMS(300);
    virarEsquerdaGiro90();
    moverFrentePorMS(1100);
    virarDireitaGiro90();
    moverFrentePorMS(2300);
    virarDireitaGiro90();
    moverFrentePorMS(1100);
    virarEsquerdaGiro90();
    moverTrasPorMS(100);
    desligarLed(AMBOS);
    loop();
}