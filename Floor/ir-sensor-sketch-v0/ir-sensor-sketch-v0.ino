int IRSensor = 7; // connect IR sensor module to Arduino pin D9
int LED = 13;     // connect LED to Arduino pin 13

int SSRPin = 4;

void setup()
{
    Serial.begin(115200);             // Init Serial at 115200 Baud Rate.
    pinMode(IRSensor, INPUT);         // IR Sensor pin INPUT
    pinMode(LED, OUTPUT);             // LED Pin Output
    pinMode(SSRPin, OUTPUT);             // LED Pin Output
}

void loop()
{
    int sensorStatus = digitalRead(IRSensor); // Set the GPIO as Input
    if (sensorStatus == 1)                    // Check if the pin high or not
    {
        // if the pin is high turn off the onboard Led
        digitalWrite(LED, LOW);             // LED LOW
        Serial.println("Door's closed!"); // print Motion Detected! on the serial monitor window
        digitalWrite(SSRPin, HIGH);
    }
    else
    {
        // else turn on the onboard LED
        digitalWrite(LED, HIGH);         // LED High
        Serial.println("Door's open!");  // print Motion Ended! on the serial monitor window
        digitalWrite(SSRPin, LOW);
    }
}