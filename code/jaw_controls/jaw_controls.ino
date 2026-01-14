#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo configuration
#define SERVO_0_CHANNEL 2
#define SERVO_1_CHANNEL 1

// Servo pulse values for different angles (adjust based on your servos)
#define SERVOMIN  150     // Minimum pulse length count (out of 4096)
#define SERVOMAX  600     // Maximum pulse length count (out of 4096)
#define SERVO_CENTER 375  // Center position (90 degrees)

// Angle limits for each servo
float servo0_min_angle = -40;
float servo0_max_angle = 0;
float servo1_min_angle = -40;
float servo1_max_angle = 0;

// Movement parameters for each servo
float servo0_angle = 0;
float servo1_angle = 0;
float servo0_speed = 1;  // degrees per cycle
float servo1_speed = 1;  // different speed for async movement
int servo0_direction = 1;   // 1 for increasing, -1 for decreasing
int servo1_direction = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("Two Servo Async Test");
  delay(200);

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz
  
  // Start both servos at center position
  moveServo(SERVO_0_CHANNEL, 0);
  moveServo(SERVO_1_CHANNEL, 0);
  delay(500);
}

void loop() {
  while (Serial.available() > 0)    {
    char command = Serial.read();  // Gets ASCII value (ex: '1' = 49)
    moveServo(SERVO_0_CHANNEL, command);  // Passes 49 as angle—way out of range
    if (command == 's') {
      Serial.println("Stopping servos.");
      moveServo(SERVO_0_CHANNEL, 0);
      moveServo(SERVO_1_CHANNEL, 0);
      while (true);  // Halt further movement
    } else if (command == 'q') {
      Serial.println("Exiting loop.");
      return;  // Exit the loop function
    } else {
      moveServo(SERVO_0_CHANNEL, command);
      moveServo(SERVO_1_CHANNEL, command);
    }
  }

  // Update servo 0 angle (0 to 15 degrees)
  servo0_angle += servo0_speed * servo0_direction;
  if (servo0_angle >= servo0_max_angle) {
    servo0_angle = servo0_max_angle;
    servo0_direction = -1;
  } else if (servo0_angle <= servo0_min_angle) {
    servo0_angle = servo0_min_angle;
    servo0_direction = 1;
  }

  // Update servo 1 angle (0 to -15 degrees)
  servo1_angle += servo1_speed * servo1_direction;
  if (servo1_angle >= servo1_max_angle) {
    servo1_angle = servo1_max_angle;
    servo1_direction = -1;
  } else if (servo1_angle <= servo1_min_angle) {
    servo1_angle = servo1_min_angle;
    servo1_direction = 1;
  }

  // Move servos
  moveServo(SERVO_0_CHANNEL, servo0_angle);
  moveServo(SERVO_1_CHANNEL, servo1_angle);

  // Print current positions
  Serial.print("Servo 0: ");
  Serial.print(servo0_angle);
  Serial.print("°  Servo 1: ");
  Serial.print(servo1_angle);
  Serial.println("°");

  delay(20);  // Control update rate
}

// Convert angle to pulse width and move servo
void moveServo(uint8_t channel, float angle) {
  // Convert angle to pulse width
  // 0 degrees = SERVO_CENTER
  // positive angles move one direction, negative the other
  float pulseWidth = SERVO_CENTER + (angle * (SERVOMAX - SERVO_CENTER) / 90.0);
  
  // Constrain to valid range
  pulseWidth = constrain(pulseWidth, SERVOMIN, SERVOMAX);
  
  pwm.setPWM(channel, 0, (uint16_t)pulseWidth);
}
