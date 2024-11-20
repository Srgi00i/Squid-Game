const int buttonPin = 23; // Pin connected to the button
const int relayPin = 27;  // Pin connected to the relay

void setup()
{
    pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
    pinMode(relayPin, OUTPUT);        // Set relay pin as output
    digitalWrite(relayPin, LOW);      // Ensure relay is off at start
}

void loop()
{
    if (digitalRead(buttonPin) == LOW)
    {                                 // Check if button is pressed (LOW due to pull-up)
        digitalWrite(relayPin, HIGH); // Activate relay
        delay(1000);                  // Keep relay on for 1 second
        digitalWrite(relayPin, LOW);  // Deactivate relay
        delay(50);                    // Small debounce delay to avoid multiple triggers
    }
}
