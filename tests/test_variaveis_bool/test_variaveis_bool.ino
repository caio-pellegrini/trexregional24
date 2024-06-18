bool variavelNaoInicializada;
bool variavelInicializadaFalse = false;
bool variavelInicializadaTrue = true;

void setup() {
  Serial.begin(9600);

  // O valor de `variavelNaoInicializada` é indefinido
  Serial.print("variavelNaoInicializada: ");
  Serial.println(variavelNaoInicializada);

  // O valor de `variavelInicializadaFalse` é false
  Serial.print("variavelInicializadaFalse: ");
  Serial.println(variavelInicializadaFalse);

  // O valor de `variavelInicializadaTrue` é true
  Serial.print("variavelInicializadaTrue: ");
  Serial.println(variavelInicializadaTrue);
}

void loop() {
  // Não há nada no loop para este exemplo
}
