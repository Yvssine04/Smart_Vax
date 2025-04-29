#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_ROTATE 0
#define SERVO_UPDOWN1 1
#define SERVO_UPDOWN2 2
#define SERVO_GRAB 3
#define SERVO_FREQ 50

const int ROTATE_MIN = 0;
const int ROTATE_MAX = 180;
const int UPDOWN_MIN = 0;
const int UPDOWN_MAX = 90;
const int GRAB_OPEN_PWM = 400;
const int GRAB_CLOSE_PWM = 250;

// Display pins
const int segmentPins[] = {2,3,4,5,6,7,8};
const int digitPins[] = {9,10,11};
const byte digitPatterns[10] = {
  B00111111, B00000110, B01011011, B01001111, B01100110,
  B01101101, B01111101, B00000111, B01111111, B01101111
};

int currentQuantity = 0;
int currentRotate = 90;
int currentUpDown = 50;
bool isGrabOpen = false;
unsigned long lastCommandTime = 0;

// Home position
const int HOME_ROTATE = 40;
const int HOME_UPDOWN = 50;

// Reference positions
int referenceRotate = HOME_ROTATE;
int referenceUpDown = HOME_UPDOWN;
bool newReferenceReceived = false;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  
  // Initialize display
  for(int i=0; i<7; i++) pinMode(segmentPins[i], OUTPUT);
  for(int i=0; i<3; i++) pinMode(digitPins[i], OUTPUT);
  
  // Initialize servos to home position
  moveToHomePosition();
  closeGrabber();
  
  Serial.println("System ready - maintains position until new reference");
}

void loop() {
  displayNumber(currentQuantity);
  
  if(Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    handleCommand(input);
    lastCommandTime = millis();
  }
  
  // If new reference received, move to home then to reference
  if(newReferenceReceived) {
    pwm.wakeup();
    
    // 1. Return to home position
    moveToHomePosition();
    delay(500);
    
    // 2. Move to reference position
    moveServo(SERVO_ROTATE, referenceRotate);
    moveServo(SERVO_UPDOWN1, referenceUpDown);
    moveServo(SERVO_UPDOWN2, referenceUpDown);
    currentRotate = referenceRotate;
    currentUpDown = referenceUpDown;
    
    Serial.print("Moved to reference - Rotate: ");
    Serial.print(referenceRotate);
    Serial.print("° UpDown: ");
    Serial.println(referenceUpDown);
    
    newReferenceReceived = false;
  }
  
  if(millis() - lastCommandTime > 5000) pwm.sleep();
}

void handleCommand(String cmd) {
  Serial.print("Executing: ");
  Serial.println(cmd);
  
  if(cmd.startsWith("Q:")) {
    currentQuantity = cmd.substring(2).toInt();
  } 
  else if(cmd == "1") {
    openGrabber();
  }
  else if(cmd == "2") {
    closeGrabber();
  }
  else if(cmd == "H") {
    moveToHomePosition();
  }
  else if(cmd.startsWith("REF:")) {
    // New reference format: "REF:rotate,updown"
    int commaIndex = cmd.indexOf(',');
    if(commaIndex > 0) {
      referenceRotate = cmd.substring(4, commaIndex).toInt();
      referenceUpDown = cmd.substring(commaIndex+1).toInt();
      
      // Constrain values
      referenceRotate = constrain(referenceRotate, ROTATE_MIN, ROTATE_MAX);
      referenceUpDown = constrain(referenceUpDown, UPDOWN_MIN, UPDOWN_MAX);
      
      newReferenceReceived = true;
      
      Serial.print("New reference received - Rotate: ");
      Serial.print(referenceRotate);
      Serial.print("° UpDown: ");
      Serial.println(referenceUpDown);
    }
  }
  else if(cmd.length() == 4) {
    // Manual control mode
    pwm.wakeup();
    
    int moveRight = cmd.charAt(0) - '0';
    int moveLeft = cmd.charAt(1) - '0';
    int moveUp = cmd.charAt(2) - '0';
    int moveDown = cmd.charAt(3) - '0';

    currentRotate += (moveRight - moveLeft) * 10;
    currentRotate = constrain(currentRotate, ROTATE_MIN, ROTATE_MAX);
    
    currentUpDown += (moveUp - moveDown) * 10;
    currentUpDown = constrain(currentUpDown, UPDOWN_MIN, UPDOWN_MAX);
    
    moveServo(SERVO_ROTATE, currentRotate);
    moveServo(SERVO_UPDOWN1, currentUpDown);
    moveServo(SERVO_UPDOWN2, currentUpDown);
    
    Serial.print("Manual position - Rotate: ");
    Serial.print(currentRotate);
    Serial.print("° UpDown: ");
    Serial.println(currentUpDown);
  }
}

void moveToHomePosition() {
  pwm.wakeup();
  Serial.println("Returning to home position");
  
  // Move up/down first
  moveServo(SERVO_UPDOWN1, HOME_UPDOWN);
  moveServo(SERVO_UPDOWN2, HOME_UPDOWN);
  delay(300);
  
  // Then move rotate
  moveServo(SERVO_ROTATE, HOME_ROTATE);
  
  currentRotate = HOME_ROTATE;
  currentUpDown = HOME_UPDOWN;
}

void moveServo(int servo, int angle) {
  angle = constrain(angle, 0, 180);
  int pulse = map(angle, 0, 180, 150, 600);
  pwm.setPWM(servo, 0, pulse);
  delay(15);
}

void openGrabber() {
  pwm.wakeup();
  for(int pos = GRAB_CLOSE_PWM; pos <= GRAB_OPEN_PWM; pos += 5) {
    pwm.setPWM(SERVO_GRAB, 0, pos);
    delay(20);
  }
  isGrabOpen = true;
  Serial.println("Grabber opened");
}

void closeGrabber() {
  pwm.wakeup();
  for(int pos = GRAB_OPEN_PWM; pos >= GRAB_CLOSE_PWM + 30; pos -= 5) {
    pwm.setPWM(SERVO_GRAB, 0, pos);
    delay(20);
  }
  isGrabOpen = false;
  Serial.println("Grabber closed");
}

void displayNumber(int number) {
  number = constrain(number, 0, 999);
  int digits[3] = {number/100, (number/10)%10, number%10};

  for(int d=0; d<3; d++) {
    for(int i=0; i<3; i++) digitalWrite(digitPins[i], HIGH);
    byte pattern = digitPatterns[digits[d]];
    for(int s=0; s<7; s++) digitalWrite(segmentPins[s], bitRead(pattern, s));
    digitalWrite(digitPins[d], LOW);
    delay(5);
  }
}
