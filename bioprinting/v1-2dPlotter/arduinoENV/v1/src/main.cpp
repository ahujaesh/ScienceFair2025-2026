#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
//JOYSTICK WIRES FACE RIGHT
//A is LEFTRIGHT
//B is UPDOWN
//GREEN IS UP
//BLUE IS DOWN
//GREEN IS LEFT
//BLUE IS RIGHT

// Motor pins
const int motorLeftRight1 = 2;
const int motorLeftRight2 = 3;
const int motorUpDown1 = 4;
const int motorUpDown2 = 5;

// Joystick pins
const int joystickXPin = A8; // Left/Right
const int joystickYPin = A9; // Up/Down

// Threshold for joystick deadzone
const int threshold = 50;

// Pin assignments
const int vss = 53;   // Ground (LOW)
const int vdd = 51;   // Power (HIGH)
const int vo = 49;    // Contrast (PWM)
const int rs = 47;    // Register Select
const int rw = 45;    // Read/Write (set LOW for write)
const int enable = 43; // Enable
const int d4 = 33;    // Data line 4
const int d5 = 31;    // Data line 5
const int d6 = 29;    // Data line 6
const int d7 = 27;    // Data line 7

// Backlight control pins
const int backlightA = 25; // Backlight anode (PWM-capable pin)
const int backlightK = 23; // Backlight cathode

// Initialize the LiquidCrystal object with control pins
LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

enum DIR {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  STOPPED
};

void setup() {
  pinMode(motorLeftRight1, OUTPUT);
  pinMode(motorLeftRight2, OUTPUT);
  pinMode(motorUpDown1, OUTPUT);
  pinMode(motorUpDown2, OUTPUT);

  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Starting setup...");

  // Configure power pins
  Serial.println("Configuring power pins...");
  pinMode(vss, OUTPUT);
  pinMode(vdd, OUTPUT);
  pinMode(vo, OUTPUT);
  pinMode(rw, OUTPUT);
  pinMode(backlightA, OUTPUT); // Configure backlight anode pin
  pinMode(backlightK, OUTPUT); // Configure backlight cathode pin

  // Set power states
  Serial.println("Setting power states...");
  digitalWrite(vss, LOW);    // VSS to GND
  digitalWrite(vdd, HIGH);   // VDD to 5V
  digitalWrite(rw, LOW);     // RW to write mode
  analogWrite(vo, 5);      // VO to ~50% contrast (PWM)
  digitalWrite(backlightK, LOW); // Backlight cathode to GND
  analogWrite(backlightA, 120);  // Backlight anode to 50% brightness
  Serial.println("Power states set successfully.");

  // Initialize the LCD (16x2 display)
  delay(1000);
  Serial.println("Initializing LCD...");
  lcd.begin(16, 2);
  lcd.print("Hello, World!");
  Serial.println("LCD initialized and message displayed.");
}

void controlMotor(DIR MOTOR_A_LEFT_RIGHT, DIR MOTOR_B_UP_DOWN) {
  if (MOTOR_A_LEFT_RIGHT == RIGHT && MOTOR_B_UP_DOWN == DOWN){
    digitalWrite(2,HIGH); //both blue
    analogWrite(5,250);
    digitalWrite(4,LOW);
    analogWrite(6,250);
  } else if (MOTOR_A_LEFT_RIGHT == LEFT && MOTOR_B_UP_DOWN == UP) {
    digitalWrite(2,LOW); //both green
    analogWrite(5,250);
    digitalWrite(4,HIGH);
    analogWrite(6,250);
  } else if (MOTOR_A_LEFT_RIGHT == LEFT && MOTOR_B_UP_DOWN == DOWN) {
    digitalWrite(2,LOW);//a green b blue
    analogWrite(5,250);
    digitalWrite(4,LOW);
    analogWrite(6,250);
  } else if (MOTOR_A_LEFT_RIGHT == RIGHT && MOTOR_B_UP_DOWN == UP) {
    digitalWrite(2,HIGH);//a blue and b green
    analogWrite(5,250);
    digitalWrite(4,HIGH);
    analogWrite(6,250);
  } else if (MOTOR_A_LEFT_RIGHT == LEFT && MOTOR_B_UP_DOWN == STOPPED) {
    digitalWrite(2,LOW); // Left only
    analogWrite(5,250);
    digitalWrite(4,LOW);
    analogWrite(6,0);
  } else if (MOTOR_A_LEFT_RIGHT == RIGHT && MOTOR_B_UP_DOWN == STOPPED) {
    digitalWrite(2,HIGH); // Right only
    analogWrite(5,250);
    digitalWrite(4,LOW);
    analogWrite(6,0);
  } else if (MOTOR_A_LEFT_RIGHT == STOPPED && MOTOR_B_UP_DOWN == UP) {
    digitalWrite(2,LOW); // Up only
    analogWrite(5,0);
    digitalWrite(4,HIGH);
    analogWrite(6,250);
  } else if (MOTOR_A_LEFT_RIGHT == STOPPED && MOTOR_B_UP_DOWN == DOWN) {
    digitalWrite(2,LOW); // Down only
    analogWrite(5,0);
    digitalWrite(4,LOW);
    analogWrite(6,250);
  } else if (MOTOR_A_LEFT_RIGHT == STOPPED && MOTOR_B_UP_DOWN == STOPPED) {
    analogWrite(5, 0); // Stop both motors
    analogWrite(6, 0);
  }
}

void loop() {
  int xValue = analogRead(joystickXPin);
  int yValue = analogRead(joystickYPin);

  Serial.print(xValue);
  Serial.print("\n");
  Serial.print(yValue);
  Serial.print("\n");

  if (xValue > (500 + threshold) && yValue > (500 + threshold + 50)) {
    controlMotor(RIGHT, UP);
    Serial.println("Moving Right and Up");
  } else if (xValue < (500 - threshold) && yValue > (500 + threshold + 50)) {
    controlMotor(LEFT, UP);
    Serial.println("Moving Left and Up");
  } else if (xValue < (500 - threshold) && yValue < (500 - threshold - 50)) {
    controlMotor(LEFT, DOWN);
    Serial.println("Moving Left and Down");
  } else if (xValue > (500 + threshold) && yValue < (500 - threshold - 50)) {
    controlMotor(RIGHT, DOWN);
    Serial.println("Moving Right and Down");
  } else if (xValue > (500 + threshold) && yValue < (500 + threshold + 50) && yValue > (500 - threshold - 50)) {
    controlMotor(RIGHT, STOPPED);
    Serial.println("Moving Right");
  } else if (xValue < (500 - threshold) && yValue < (500 + threshold + 50) && yValue > (500 - threshold - 50)) {
    controlMotor(LEFT, STOPPED);
    Serial.println("Moving Left");
  } else if (yValue > (500 + threshold) && xValue < (500 + threshold + 50) && xValue > (500 - threshold - 50)) {
    controlMotor(STOPPED, UP);
    Serial.println("Moving Up");
  } else if (yValue < (500 - threshold) && xValue < (500 + threshold + 50) && xValue > (500 - threshold - 50)) {
    controlMotor(STOPPED, DOWN);
    Serial.println("Moving Down");
  } else { 
    controlMotor(STOPPED, STOPPED);
    Serial.println("Stopped");
  }
  
  delay(100);
}


/*
  // Servo code for future use:
  // #include <Servo.h>
  // Servo servoX;
  // Servo servoY;
  // servoX.attach(9);
  // servoY.attach(10);
  // int xAngle = map(xValue, 0, 1023, 0, 180);
  // int yAngle = map(yValue, 0, 1023, 0, 180);
  // servoX.write(xAngle);
  // servoY.write(yAngle);
*/
