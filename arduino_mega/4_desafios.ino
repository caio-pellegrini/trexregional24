void analisarVerde(bool isBeco, bool isVerdeEsquerdo, bool isVerdeDireito)
{
  lerTcsAmbos();
  lerTcsAmbos();

  desligarLed(AMBOS);

  bool verdeEsq = false;
  bool verdeDir = false;

  if (rgbTcsEsq[1] < CORTE_VERDE_ESQ && rgbTcsEsq[0] < CORTE_VERMELHO_CRUZ)
  {
    ligarLed(ESQ, VERDE);
    verdeEsq = true;
  }

  if (rgbTcsDir[1] != 0)
  {
    if (rgbTcsDir[1] < CORTE_VERDE_DIR && rgbTcsDir[0] < CORTE_VERMELHO_CRUZ)
    {
      ligarLed(DIR, VERDE);
      verdeDir = true;
    }
  }
  else
  {
    ligarLed(DIR, VERMELHO); // avisa que o rgbTcsDir não recebeu dados do TCS
  }

  moverFrentePorMS(TEMPO_MOVER_ANTES_CRUZ); // mover pra frente antes de virar 
  
  // Beco sem saida
  if (isBeco && verdeEsq && verdeDir)
  {
    virarEsquerdaGiro180();
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

  pararMotores();
}

void desviarObstaculo() {
    pararMotores();
    ligarLed(AMBOS, VERMELHO);
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
}

void verificarGap() {
  ligarLed(AMBOS, AMARELO);
  unsigned long tempoInicial = millis();

  moverTrasPorMS(100);
  pararMotores();
  delay(300);
  lerTcsAmbos();
  lerTcsAmbos();

  if (rgbTcsEsq[0] > rgbTcsEsq[1] && rgbTcsEsq[0] > rgbTcsEsq[2] && rgbTcsDir[0] > rgbTcsDir[1] && rgbTcsDir[0] > rgbTcsDir[2]) {
    ligarLed(AMBOS, VERMELHO);
    delayInfinito();
    return;
  }

  moverFrentePorMS(100);
  
  while (true) {
    moverFrentePorMS(1);
    lerQTRATodos();

    if (se3 > CORTE_QTR || se2 > CORTE_QTR || se1 > CORTE_QTR || se0 > CORTE_QTR || sd0 > CORTE_QTR || sd1 > CORTE_QTR || sd2 > CORTE_QTR || sd3 > CORTE_QTR) {
      desligarLed(AMBOS);
      pararMotores();
      delay(200);
      break;
    }

    if (millis() - tempoInicial >= 2000) // VALOR DE ENTRADA PARA SALA DE RESGATE
    {
      ligarLed(AMBOS, ROXO);
      pararMotores();
      delay(500);
      entrarSalaResgate();
      break;
    }
  }

}