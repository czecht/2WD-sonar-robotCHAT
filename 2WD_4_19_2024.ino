#include <MotorDriver.h>
#include <Servo.h>

MotorDriver m;
Servo servo;

#define TRIG_PIN A2
#define ECHO_PIN A1
#define DISTANCE_THRESHOLD 20 // Threshold for obstacle detection in centimeters
#define TURN_ANGLE 90 // Angle to turn when obstacle is detected
#define TURN_SPEED 75 // Speed of the motors during turning (adjust as needed)

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servo.attach(9); // Attach servo to pin 9
}

void loop() {
  long duration, distance;

  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Set the trigger pin high for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin, and calculate the distance
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check for obstacles
  if (distance < DISTANCE_THRESHOLD) {
    // Obstacle detected, stop both motors
    m.motor(3, RELEASE, 1); // Stop left motor
    m.motor(4, RELEASE, 1); // Stop right motor
    Serial.println("Obstacle detected. Stopping motors.");

    // Turn until path is clear
    turnUntilClear();
  } else {
    // No obstacle, continue moving forward
    m.motor(3, FORWARD, 100); // Adjust speed as needed
    m.motor(4, FORWARD, 100); // Adjust speed as needed
    Serial.println("No obstacle detected. Moving forward.");
  }
}

void turnUntilClear() {
  // Turn right
  Serial.println("Turning right...");
  m.motor(3, BACKWARD, TURN_SPEED); // Turn left motor backward
  m.motor(4, FORWARD, TURN_SPEED); // Turn right motor forward
  
  // Delay for turning
  delay(1500); // Adjust delay time as needed

  // Stop turning
  m.motor(3, RELEASE, 1); // Stop left motor
  m.motor(4, RELEASE, 1); // Stop right motor

  // Perform scan after turning
  long duration, distance;

  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Set the trigger pin high for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin, and calculate the distance
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check for obstacles after turning
  if (distance >= DISTANCE_THRESHOLD) {
    // No obstacle detected, continue moving forward
    m.motor(3, FORWARD, 100); // Adjust speed as needed
    m.motor(4, FORWARD, 100); // Adjust speed as needed
    Serial.println("No obstacle detected after turning. Continuing forward.");
  } else {
    // Obstacle still detected, turn in the opposite direction
    Serial.println("Obstacle detected after turning. Turning in the opposite direction.");
    m.motor(3, FORWARD, TURN_SPEED); // Turn left motor forward
    m.motor(4, BACKWARD, TURN_SPEED); // Turn right motor backward
    
    // Delay for turning
    delay(1500); // Adjust delay time as needed

    // Stop turning
    m.motor(3, RELEASE, 1); // Stop left motor
    m.motor(4, RELEASE, 1); // Stop right motor
  }
}
