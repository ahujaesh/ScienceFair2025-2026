// #include <Arduino.h>
// #include <Servo.h>

// // Define servo object
// Servo myServo;

// // Pin configuration
// const int extrusionServoDataPin = 46;


// void setup() {
//   pinMode(extrusionServoDataPin, OUTPUT);

//     myServo.attach(extrusionServoDataPin);
//     Serial.begin(9600);
//     Serial.println("Servo initialized.");
// }

// void loop() {
//     if (Serial.available() > 0) {
//         int position = Serial.parseInt(); // Read the position from serial input
//         if (position >= 0 && position <= 180) { // Ensure the position is within valid range
//             myServo.write(position); // Move servo to the specified position
//             Serial.print("Servo moved to position: ");
//             Serial.println(position);
//         } else {
//             Serial.println("Invalid position. Enter a value between 0 and 180.");
//         }
//     }
// }
