#include <Servo.h>

int servoPin = 8; // Change to the appropriate pin for your setup
int servoPosition = 45;   // Start at 45 degrees
int servoIncrement = 30;  // The amount to increment or decrement the servo position
int delayTime = 1000;    // The delay between each increment in milliseconds (1 second)
Servo myServo;

void setup()
{
  Serial.begin(9600);
  myServo.attach(servoPin);
}

void loop()
{
  // Increment the servo position by 5 degrees
  servoPosition += servoIncrement;

  // Check if the servo position is within the allowed range (0-180 degrees)
  if (servoPosition < 0) {
    servoPosition = 0;
    servoIncrement = -servoIncrement; // Change direction when reaching the lower limit
  } else if (servoPosition > 180) {
    servoPosition = 180;
    servoIncrement = -servoIncrement; // Change direction when reaching the upper limit
  }

  // Set the servo to the new position
  myServo.write(servoPosition);

  // Delay for the specified time before the next increment
  delay(delayTime);
}
