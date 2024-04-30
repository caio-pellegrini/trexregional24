void lerVerde(bool debug_mode) {
  Serial2.println("caio");
  lerSensorCor(&tcsEsq, rgbEsq);
  lerDadosSensorRemoto(rgbDir);

  if (debug_mode) {
    Serial.print("TCS ESQ: ");
    Serial.print("R:");
    Serial.print(rgbEsq[0]);
    Serial.print(",G:");
    Serial.print(rgbEsq[1]);
    Serial.print(",B:");
    Serial.print(rgbEsq[2]);

    Serial.print(" TCS DIR: ");
    Serial.print("R:");
    Serial.print(rgbDir[0]);
    Serial.print(",G:");
    Serial.print(rgbDir[1]);
    Serial.print(",B:");
    Serial.print(rgbDir[2]);
    Serial.println();
  }
}
