const int sensorPin = A0; // Analog pin connected to the sensor

void setup() {
    Serial.begin(9600); // Initialize serial communication
}

void loop() {
    int sensorValue = analogRead(sensorPin) >> 2; // Read the sensor value
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue); // Print the sensor value to the serial monitor
}