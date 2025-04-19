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

const int joystickXPin = A8; // Left/Right
const int joystickYPin = A9; // Up/Down

const int threshold = 50;

const int motorLeftRight1 = 2;
const int motorLeftRight2 = 3;
const int motorUpDown1 = 4;
const int motorUpDown2 = 5;

bool hasBeenPrinted = false; //is the print complete

int currLocX = 0, currLocY = 0;

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
const int backlightA = 25; // Backlight anode (PWM-capable pin)
const int backlightK = 23; // Backlight cathode

const int MSPerMMX = 85; //approx val for the amount of ms at 250/255 power it takes to move one mm X
const int MSPerMMY = 90; //approx val for the amount of ms at 250/255 power it takes to move one mm Y

LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

enum DIR {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  STOPPED
};


String gcode[] = { //based on a 100mm^2 space, where toolhead is located at (0,0) at start; goal: draw a smiley face
  "G91", //relative positioning
  "G21", //units:mm

  "G0 X15 Y15", //smile

  "G1 E10", //start extruding

  "G0 X85 Y15", //end smile

  "G1 E0", //stop extruding

  "G0 X25 Y90", //start left eye

  "G1 E10", //start extruding

  "G0 X25 Y50", // end left eye

  "G1 E0", //stop extruding

  "G0 X75 Y90", //start right eye

  "G1 E10", //start extruding

  "G0 X75 Y50", //end right eye

  "G1 E0", //stop extruding

  "G0 X0 Y0" //home (could be g28 but i dont have switches)
};

void setup() {
  pinMode(motorLeftRight1, OUTPUT);
  pinMode(motorLeftRight2, OUTPUT);
  pinMode(motorUpDown1, OUTPUT);
  pinMode(motorUpDown2, OUTPUT);

  Serial.begin(9600);
  pinMode(joystickXPin, INPUT);
  pinMode(joystickYPin, INPUT);
  Serial.println("Starting setup...");

  Serial.println("Configuring power pins...");
  pinMode(vss, OUTPUT);
  pinMode(vdd, OUTPUT);
  pinMode(vo, OUTPUT);
  pinMode(rw, OUTPUT);
  pinMode(backlightA, OUTPUT); // Configure backlight anode pin
  pinMode(backlightK, OUTPUT); // Configure backlight cathode pin

  Serial.println("Setting power states...");
  digitalWrite(vss, LOW);    // VSS to GND
  digitalWrite(vdd, HIGH);   // VDD to 5V
  digitalWrite(rw, LOW);     // RW to write mode
  analogWrite(vo, 5);      // VO to ~50% contrast (PWM)
  digitalWrite(backlightK, LOW); // Backlight cathode to GND
  analogWrite(backlightA, 120);  // Backlight anode to 50% brightness
  Serial.println("Power states set successfully.");

  delay(1000);
  Serial.println("Initializing LCD...");
  lcd.begin(16, 2);
  lcd.print("BioprinterV1!");
  Serial.println("LCD initialized and message displayed.");
}

void setPowerForGantry(DIR MOTOR_A_LEFT_RIGHT, DIR MOTOR_B_UP_DOWN) {
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

void controlGantry(int targetX, int targetY) {
  DIR xMovementDirection;

  if ((currLocX - targetX) < 0) { //dir is positive
    xMovementDirection = LEFT;
  } else if ((currLocX - targetX) == 0) { //already at loc
    xMovementDirection = STOPPED;
  } else if ((currLocX - targetX) > 0) { // dir is negative
    xMovementDirection = RIGHT;
  }

  int mmToMoveX = abs(currLocX - targetX);

  DIR yMovementDirection;

  if ((currLocY - targetY) > 0) { // dir is negative
    yMovementDirection = DOWN;
  } else if ((currLocY - targetY) == 0) { // already at loc
    yMovementDirection = STOPPED;
  } else if ((currLocY - targetY) < 0) { // dir is positive
    yMovementDirection = UP;
  }

  int mmToMoveY = abs(currLocY - targetY);

  for (int mmX = 1; mmX <= (mmToMoveX); mmX++) { //move one mm per loop until all mm movement is complete 
    setPowerForGantry(xMovementDirection, STOPPED);
    delay(MSPerMMX); //x moves slightly faster than y
    setPowerForGantry(STOPPED, STOPPED);
    Serial.print("moved1mmX \n");
  }

  for (int mmY = 1; mmY <= (mmToMoveY); mmY++) { // move one mm per loop until all mm movement is complete
    setPowerForGantry(STOPPED, yMovementDirection);
    delay(MSPerMMY);
    setPowerForGantry(STOPPED, STOPPED);
    Serial.print("moved1mmY \n");
  }

  currLocX = targetX;
  currLocY = targetY;
}

void loop() {
  if (hasBeenPrinted == false) { // if it hasn't been printed, print it
    for (int lineNum = 0; lineNum < (sizeof(gcode) / sizeof(gcode[0])); lineNum++) {
      String lineContent = gcode[lineNum];
      if (lineContent.startsWith("G0")) {
        int xIndex = lineContent.indexOf('X');
        int yIndex = lineContent.indexOf('Y');

        int xVal = 0;
        int yVal = 0;

        if (xIndex != -1) {
        int spaceAfterX = lineContent.indexOf(' ', xIndex);
        xVal = lineContent.substring(xIndex + 1, spaceAfterX == -1 ? lineContent.length() : spaceAfterX).toInt();
        }

        if (yIndex != -1) {
        int spaceAfterY = lineContent.indexOf(' ', yIndex);
        yVal = lineContent.substring(yIndex + 1, spaceAfterY == -1 ? lineContent.length() : spaceAfterY).toInt();
        }

        controlGantry(xVal, yVal);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("X: " + String(currLocX));
        lcd.setCursor(0, 1);
        lcd.print("Y: " + String(currLocY));
      } else if (lineContent.startsWith("G1")) {
        int eIndex = lineContent.indexOf('E');

        int eVal = 0;

        if (eIndex != -1) {
          int spaceAfterE = lineContent.indexOf(' ', eIndex);
          eVal = lineContent.substring(eIndex + 1, spaceAfterE == -1 ? lineContent.length() : spaceAfterE).toInt();
        }

        // Handle extrusion logic here based on eVal
        if (eVal > 0) {
          Serial.println("Extruding...");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Extruding...");
          //TODO: Add extrusion logic
        } else {
          Serial.println("Stopping extrusion...");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Stopping...");
          //TODO: Add stop extrusion logic
        }
      }
      delay(500); //prevent overload
    }
    hasBeenPrinted = true;
  } else { // if it has been printed, switch to joystick control
    int xValue = analogRead(joystickXPin);
    int yValue = analogRead(joystickYPin);

    Serial.print(xValue);
    Serial.print("\n");
    Serial.print(yValue);
    Serial.print("\n");

    if (xValue > (500 + threshold) && yValue > (500 + threshold + 50)) {
      setPowerForGantry(RIGHT, UP);
      Serial.println("Moving Right and Up");
    } else if (xValue < (500 - threshold) && yValue > (500 + threshold + 50)) {
      setPowerForGantry(LEFT, UP);
      Serial.println("Moving Left and Up");
    } else if (xValue < (500 - threshold) && yValue < (500 - threshold - 50)) {
      setPowerForGantry(LEFT, DOWN);
      Serial.println("Moving Left and Down");
    } else if (xValue > (500 + threshold) && yValue < (500 - threshold - 50)) {
      setPowerForGantry(RIGHT, DOWN);
      Serial.println("Moving Right and Down");
    } else if (xValue > (500 + threshold) && yValue < (500 + threshold + 50) && yValue > (500 - threshold - 50)) {
      setPowerForGantry(RIGHT, STOPPED);
      Serial.println("Moving Right");
    } else if (xValue < (500 - threshold) && yValue < (500 + threshold + 50) && yValue > (500 - threshold - 50)) {
      setPowerForGantry(LEFT, STOPPED);
      Serial.println("Moving Left");
    } else if (yValue > (500 + threshold) && xValue < (500 + threshold + 50) && xValue > (500 - threshold - 50)) {
      setPowerForGantry(STOPPED, UP);
      Serial.println("Moving Up");
    } else if (yValue < (500 - threshold) && xValue < (500 + threshold + 50) && xValue > (500 - threshold - 50)) {
      setPowerForGantry(STOPPED, DOWN);
      Serial.println("Moving Down");
    } else { 
      setPowerForGantry(STOPPED, STOPPED);
      Serial.println("Stopped");
    }
  }
}
