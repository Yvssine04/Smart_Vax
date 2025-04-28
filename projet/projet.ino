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
bool isGrabOpen = false;
unsigned long lastCommandTime = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  
  // Initialize display
  for(int i=0; i<7; i++) pinMode(segmentPins[i], OUTPUT);
  for(int i=0; i<3; i++) pinMode(digitPins[i], OUTPUT);
  
  // Initialize servos
  moveServo(SERVO_ROTATE, 40);
  moveServo(SERVO_UPDOWN1, 50);
  moveServo(SERVO_UPDOWN2, 50);
  closeGrabber();
  
  Serial.println("System ready - up/down follows reference exactly");
}

void loop() {
  displayNumber(currentQuantity);
  
  if(Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    handleCommand(input);
    lastCommandTime = millis();
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
  else if(cmd.length() == 4) {
    pwm.wakeup();
    
    // Extract movement values
    int moveRight = cmd.charAt(0) - '0';
    int moveLeft = cmd.charAt(1) - '0';
    int moveUp = cmd.charAt(2) - '0';
    int moveDown = cmd.charAt(3) - '0';

    // Calculate new rotate position (10° per digit)
    currentRotate += (moveRight - moveLeft) * 10;
    currentRotate = constrain(currentRotate, ROTATE_MIN, ROTATE_MAX);
    
    // Calculate up/down position directly from reference (10° per digit)
    int upDownPosition = (moveUp - moveDown) * 10 + 50; // Center at 50
    upDownPosition = constrain(upDownPosition, UPDOWN_MIN, UPDOWN_MAX);
    
    // Move servos
    moveServo(SERVO_ROTATE, currentRotate);
    moveServo(SERVO_UPDOWN1, upDownPosition);
    moveServo(SERVO_UPDOWN2, upDownPosition);
    
    Serial.print("New position - Rotate: ");
    Serial.print(currentRotate);
    Serial.print("° UpDown: ");
    Serial.print(upDownPosition);
    Serial.println("°");
  }
}

void moveServo(int servo, int angle) {
  pwm.setPWM(servo, 0, map(angle, 0, 180, 150, 600));
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
