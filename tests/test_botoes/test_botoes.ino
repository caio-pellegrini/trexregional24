const int buttonPin = 33;   // Pin connected to the button
int buttonState;     // Current state of the button

void setup()
{
    pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
    Serial.begin(9600);               // Initialize serial communication
}

void loop()
{
    buttonState = !digitalRead(buttonPin); // Read the button state

    if (buttonState == LOW)
    {
        Serial.println("0");
        // Perform any desired actions when the button is pressed
    }
    else
    {
        Serial.println("1");
        // Perform any desired actions when the button is released
    }
}