void entrarSalaResgate()
{
    pararMotor();
    desligarLed(AMBOS);

    // attach servos
    servoPaGarra.attach(SERVO_PA_GARRA_PIN);
    servoSubirGarra.attach(SERVO_SUBIR_GARRA_PIN);
    servoRotacionarGarra.attach(SERVO_ROTACIONAR_GARRA_PIN);
    servoCancelaEsq.attach(SERVO_CANCELA_ESQ_PIN);
    servoCancelaDir.attach(SERVO_CANCELA_DIR_PIN);

    reconhecerPegarVitima();

    // 

}