const int sensorPin = A6; // Replace with the appropriate pin number

void setup() {
    Serial.begin(9600); // Initialize serial communication
    pinMode(sensorPin, INPUT); // Set the sensor pin as input
}

void loop() {
    int sensorValue = analogRead(sensorPin); // Read the sensor value

    // Print the sensor value to the serial monitor
    Serial.print("Receptor: ");
    Serial.println(sensorValue);

    delay(100); // Wait for 1 second before reading again
}