#include <AccelStepper.h>
#include <Encoder.h>

// Pin definitions
#define STEP_PIN 9
#define DIR_PIN 8
#define EN_PIN 7
//#define ENCODER_PIN_A 2
//#define ENCODER_PIN_B 3

// Initialize stepper motor and encoder
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);
//Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);

// Motor and encoder parameters
const int stepsPerRevolution = 200; // Adjust based on your motor's specs
long targetPosition = 0;
long encoderPosition = 0;
int fact = 10;

void setup() {

  Serial.begin(9600);

  pinMode(EN_PIN,OUTPUT);

  // Initialize stepper motor parameters
  stepper.setMaxSpeed(100 * fact);     // Set max speed
  stepper.setAcceleration(50 * fact); // Set acceleration

  stepper.moveTo(50*fact);

  // Zero the encoder position
  //encoder.write(0);
}

//void loop() {
//  // Read the encoder position
//  encoderPosition = encoder.read();
//
//  // Example target position control
//  if (Serial.available()) {
//    int targetSteps = Serial.parseInt();
//    targetPosition = targetSteps;
//    stepper.moveTo(targetPosition);
//  }
//
//  // Control motor to reach the target position
//  stepper.run();
//
//  // Monitor positions
//  Serial.print("Target Position: ");
//  Serial.print(targetPosition);
//  Serial.print(" | Encoder Position: ");
//  Serial.println(encoderPosition);
//
//  // Small delay to stabilize readings
//  delay(1000);
//}

void loop()
{
  digitalWrite(EN_PIN, HIGH);

  if(stepper.distanceToGo() == 0)
    stepper.moveTo(-stepper.currentPosition());

  stepper.run();
}
