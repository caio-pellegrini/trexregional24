void lerVerde() {
  Serial2.println("caio");
  lerSensorCor(&tcsEsq, rgbEsq);
  lerDadosSensorRemoto(rgbDir);
}
