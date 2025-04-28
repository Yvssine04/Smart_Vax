#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_ROTATE 0     // Motor for rotating the arm
#define SERVO_UPDOWN1 1    // First motor for up/down movement
#define SERVO_UPDOWN2 2    // Second motor for up/down movement
#define SERVO_GRAB 3       // Motor for grab open/close
#define SERVO_FREQ 50
#define DELAY_BETWEEN_MOVEMENTS 1000 

bool hasReceivedCommand = false;
bool executionComplete = false;

const int DEGREE_MIN[4] = {0, 0, 0, 0};       // Updated for grab servo
const int DEGREE_MAX[4] = {180, 180, 90, 180}; // Updated for grab servo
const int SERVO_MIN[4] = {150, 150, 200, 150};
const int SERVO_MAX[4] = {600, 600, 400, 600};

int currentRotateDegree = 90; // Start at neutral position
int currentUpDownDegree = 90; // Start at neutral position

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  pwm.sleep();
  Serial.println("Arduino ready to receive commands.");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    Serial.print("Received command: ");
    Serial.println(input);

    if (input == "0") {  // END
      executionComplete = true;
    } 
    else if (input == "1") {  // GRAB_OPEN
      pwm.wakeup();
      pwm.setPWM(SERVO_GRAB, 0, mapDegreeToPWM(SERVO_GRAB, 150)); // Open position
      Serial.println("Grab opened.");
    } 
    else if (input == "2") {  // GRAB_CLOSE
      pwm.setPWM(SERVO_GRAB, 0, mapDegreeToPWM(SERVO_GRAB, 0)); // Close position
      Serial.println("Grab closed.");
    }
    else if (input.length() == 4) {
      pwm.wakeup();
      hasReceivedCommand = true;

      int moveRight = input.charAt(0) - '0'; // First digit for right movement
      int moveLeft = input.charAt(1) - '0';  // Second digit for left movement
      int moveUp = input.charAt(2) - '0';    // Third digit for up movement
      int moveDown = input.charAt(3) - '0';  // Fourth digit for down movement

      // Apply each movement
      moveServoRotate(moveRight, moveLeft);
      delay(DELAY_BETWEEN_MOVEMENTS);
      moveServoUpDown(moveUp, moveDown);
      delay(DELAY_BETWEEN_MOVEMENTS);
    }
  }

  if (executionComplete) {
    shutdownServos();
  }
}

void moveServoRotate(int moveRight, int moveLeft) {
  currentRotateDegree += moveRight * 10; // Move right by 10 degrees for each 1 in the input
  currentRotateDegree -= moveLeft * 10;  // Move left by 10 degrees for each 1 in the input

  // Clamp within the min and max degrees for rotation
  if (currentRotateDegree > DEGREE_MAX[SERVO_ROTATE]) currentRotateDegree = DEGREE_MAX[SERVO_ROTATE];
  if (currentRotateDegree < DEGREE_MIN[SERVO_ROTATE]) currentRotateDegree = DEGREE_MIN[SERVO_ROTATE];

  pwm.setPWM(SERVO_ROTATE, 0, mapDegreeToPWM(SERVO_ROTATE, currentRotateDegree));
  Serial.print("Rotated to degree: ");
  Serial.println(currentRotateDegree);
}

void moveServoUpDown(int moveUp, int moveDown) {
  currentUpDownDegree += moveUp * 10; // Move up by 10 degrees for each 1 in the input
  currentUpDownDegree -= moveDown * 10; // Move down by 10 degrees for each 1 in the input

  // Clamp within the min and max degrees for up/down
  if (currentUpDownDegree > DEGREE_MAX[SERVO_UPDOWN1]) currentUpDownDegree = DEGREE_MAX[SERVO_UPDOWN1];
  if (currentUpDownDegree < DEGREE_MIN[SERVO_UPDOWN1]) currentUpDownDegree = DEGREE_MIN[SERVO_UPDOWN1];

  // Move BOTH up/down servos
  pwm.setPWM(SERVO_UPDOWN1, 0, mapDegreeToPWM(SERVO_UPDOWN1, currentUpDownDegree));
  pwm.setPWM(SERVO_UPDOWN2, 0, mapDegreeToPWM(SERVO_UPDOWN2, currentUpDownDegree));

  Serial.print("Moved Up/Down to degree: ");
  Serial.println(currentUpDownDegree);
}

int mapDegreeToPWM(int servo, int degree) {
  return map(degree, DEGREE_MIN[servo], DEGREE_MAX[servo], SERVO_MIN[servo], SERVO_MAX[servo]);
}

void shutdownServos() {
  pwm.sleep();
  Serial.println("Servos shut down.");
  executionComplete = false;
}
