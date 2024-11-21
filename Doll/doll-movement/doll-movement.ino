#include <AccelStepper.h>
#include <Encoder.h>
#include <Wire.h>

// Pin definitions
#define DIR_PIN 8
#define STEP_PIN 9
#define EN_PIN 7

#define HALL_PIN 10
#define BUTTON_A_PIN 11
#define BUTTON_B_PIN 12

// Initialize stepper motor
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Motor parameters
const int stepsPerRevolution = 1600; // Adjust based on your motor's specs
int halfRotation = stepsPerRevolution / 2;

// Sensor parameters

// Logic parameters


void setup()
{
    Serial.begin(9600);

    pinMode(EN_PIN, OUTPUT);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(BUTTON_A_PIN, INPUT);

    // Initialize stepper motor parameters
    //stepper.setMaxSpeed(200);     // Set max speed
    //stepper.setAcceleration(200); // Set acceleration
    //digitalWrite(EN_PIN, HIGH);
    
    homing();

    //stepper.moveTo(50);
}

bool checkHoming()
{
    if (digitalRead(HALL_PIN) == LOW)
    { // Check if button is pressed (LOW due to pull-up)
        return true;
    }
    else
    {
        return false;
    }
}

void homing()
{
  digitalWrite(EN_PIN, HIGH);
  stepper.setMaxSpeed(400);     // Set max speed
  stepper.setAcceleration(200); // Set acceleration
  
  Serial.println("homing...");

  while (!checkHoming())
  {
    stepper.moveTo(stepper.currentPosition() + 10);
    stepper.run();
  }

  Serial.println("done!");
}

void turnSecuence()
{
  digitalWrite(EN_PIN, HIGH);
  stepper.setMaxSpeed(500);     // Set max speed
  stepper.setAcceleration(100); // Set acceleration

  stepper.moveTo(stepper.currentPosition()+halfRotation);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  delay(2500);

  stepper.moveTo(stepper.currentPosition()-halfRotation);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  delay(2500);
}

void loop()
{
    if(digitalRead(BUTTON_A_PIN) == HIGH)
    {
      Serial.println("Signal recieved:");
      turnSecuence();
    }
      
}
