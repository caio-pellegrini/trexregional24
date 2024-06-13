/// @brief Ligar LEDS RGB que funcionam como setas do robô
/// @param esq 
/// @param dir 
/// @param r 
/// @param g 
/// @param b 
void ligarLed(bool esq, bool dir, uint8_t r, uint8_t g, uint8_t b)
{
  if (esq)
  {
    analogWrite(LED_ESQ_RED_PIN, r);
    analogWrite(LED_ESQ_GREEN_PIN, g);
    analogWrite(LED_ESQ_BLUE_PIN, b);
  }
  if (dir)
  {
    analogWrite(LED_DIR_RED_PIN, r);
    analogWrite(LED_DIR_GREEN_PIN, g);
    analogWrite(LED_DIR_BLUE_PIN, b);
  }
}

void ligarLed(bool esq, bool dir, uint8_t r, uint8_t g, uint8_t b, unsigned long delayTempo)
{
  ligarLed(esq, dir, r, g, b);

  delay(delayTempo);

  desligarLed(esq, dir);
}

void desligarLed(bool esq, bool dir)
{
  if (esq)
  {
    analogWrite(LED_ESQ_RED_PIN, 0);
    analogWrite(LED_ESQ_GREEN_PIN, 0);
    analogWrite(LED_ESQ_BLUE_PIN, 0);
  }
  if (dir)
  {
    analogWrite(LED_DIR_RED_PIN, 0);
    analogWrite(LED_DIR_GREEN_PIN, 0);
    analogWrite(LED_DIR_BLUE_PIN, 0);
  }
}

void printDebug(String msg)
{
  if (DEBUG)
  {
    Serial.println(msg);
  }
}

void i2c_scanner()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}

void tcaSelecionar(uint8_t i)
{
  Wire.beginTransmission(TCA_ENDERECO);
  Wire.write(1 << i); // Acessa o canal desejado
  Wire.endTransmission();
}

void tcaDesligar()
{
  Wire.beginTransmission(TCA_ENDERECO);
  Wire.write(0); // Desligar todos os canais
  Wire.endTransmission();
}