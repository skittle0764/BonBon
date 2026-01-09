#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//Change Potentiometer Pins if necessary
#define pot0  A0
#define pot1  A1
#define pot2  A2
#define pot3  A3

uint16_t servoPulse[] = {350, 350, 350, 350};

void setup() {
  Serial.begin(9600);
  delay(200);

  pwm.begin();
  pwm.setPWMFreq(60); 

  pinMode(pot0, INPUT);
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);

}

void loop() {
  
  servoPulse[0] = map(analogRead(pot0), 0, 1023, 0, 800);
  servoPulse[1] = map(analogRead(pot1), 0, 1023, 0, 800);
  servoPulse[2] = map(analogRead(pot2), 0, 1023, 0, 800);
  servoPulse[3] = map(analogRead(pot3), 0, 1023, 0, 800);

  Serial.print("Servo 0: ");
    Serial.print(servoPulse[0]);
  Serial.print("  Servo 1: ");
    Serial.print(servoPulse[1]);
  Serial.print("  Servo 2: ");
    Serial.print(servoPulse[2]);
  Serial.print("  Servo 3: ");
    Serial.println(servoPulse[3]);

  pwm.setPWM(0,0,servoPulse[0]);
  pwm.setPWM(1,0,servoPulse[1]);
  pwm.setPWM(2,0,servoPulse[2]);
  pwm.setPWM(3,0,servoPulse[3]);

  delay(5);

}
