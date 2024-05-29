const int sensorPin = 3; // Replace with the appropriate pin number

void setup() {
    Serial.begin(9600); // Initialize serial communication
    pinMode(sensorPin, INPUT); // Set the sensor pin as input
}

void loop() {
    int sensorValue = digitalRead(sensorPin); // Read the sensor value

    // Print the sensor value to the serial monitor
    Serial.print("Receptor: ");
    Serial.println(sensorValue ? "fechado" : "aberto");

    delay(100); // Wait for 1 second before reading again
}