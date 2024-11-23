/*
 * @Author: Prabhas Kumar
 * @Assistant: OpenAI ChatGPT (GPT 4o)
 * @Created: November 22'24
 * @Updated: November 23'24
 * @Project: Mesmerize'24 [Line Following Competition]
 * @File: ArduinoNanoSketch.ino [Simplified C++ Script]
 */

// IR sensors
#define leftSensor A7
#define centerSensor A4
#define rightSensor A1

// Motor one
#define EnableA 3
#define INA1 4
#define INA2 5

// Motor two
#define EnableB 10
#define INB1 9
#define INB2 8

// Test/Competition Mode Selector
#define modePin 7 // If HIGH, enter test mode

// Global Variables
int motorSpeed = 120; // PWM duty cycle to control speed of the robot (1-255)
int threshold = 500; // IR sensor threshold, calibrated dynamically
int lastError = 0;
int P = 0, D = 0;
long I = 0;

// Track turn preference
bool preferRight = true;

// PID Constants
float Kp = 1.0, Ki = 0, Kd = 0.5;

// Struct for Profile Management
struct Profile {
  float Kp, Ki, Kd;
  int motorSpeed;
  int threshold;
};

// Preset Profiles
Profile defaultProfile = {1.0, 0.0, 0.5, 120, 500};
Profile sharpTurnsProfile = {1.0, 0.0, 0.5, 100, 500};
Profile highSpeedProfile = {1.0, 0.0, 0.5, 150, 550};

void setup() {
  Serial.begin(9600);

  // Initialize pins
  pinMode(leftSensor, INPUT);
  pinMode(centerSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(EnableA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(EnableB, OUTPUT);
  pinMode(modePin, INPUT_PULLUP); // Use internal pull-up for mode selection
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);

  // Mode Selection
  if (digitalRead(modePin) == HIGH) {
    enterTestMode(); // Perform calibration and testing
  } else {
    loadProfile(defaultProfile); // Load operational profile
  }
}

void loop() {
  // Read sensor values
  int leftValue = analogRead(leftSensor) > threshold ? 1 : 0;
  int centerValue = analogRead(centerSensor) > threshold ? 1 : 0;
  int rightValue = analogRead(rightSensor) > threshold ? 1 : 0;

  // Compute error based on sensor readings
  int error = (leftValue * -1) + (centerValue * 0) + (rightValue * 1);

  // Handle special cases
  if (leftValue == 0 && centerValue == 0 && rightValue == 0) {
    if (lastError > 0) {
      adjustMotors(motorSpeed / 2); // Turn right
    } else {
      adjustMotors(-motorSpeed / 2); // Turn left
    }
    return; // Exit loop early
  } else if (leftValue == 1 && centerValue == 1 && rightValue == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
    carStop();
    return; // Exit loop
  } else if (leftValue == 1 && centerValue == 0 && rightValue == 1) {
    if (preferRight) {
      leftValue = 0;
      centerValue = 0;
      rightValue = 1;
    } else {
      leftValue = 1;
      centerValue = 0;
      rightValue = 0;
    }
  }


  // PID Calculations
  P = error;
  I += error;
  D = error - lastError;

  int correction = (int)(Kp * P + Ki * (float)I + Kd * D);
  adjustMotors(correction);

  lastError = error;
}

// Calibrate Sensors Manually (Black and White Reference)
void calibrateSensors() {
  int blackValue = 0, whiteValue = 1023;

  Serial.println("Place one sensor on black and another on white. Starting calibration...");
  delay(3000); // Wait for user to position the robot

  // Collect data
  for (int i = 0; i < 100; i++) {
    int left = analogRead(leftSensor);
    int center = analogRead(centerSensor);
    int right = analogRead(rightSensor);

    blackValue = max(blackValue, max(left, max(center, right)));
    whiteValue = min(whiteValue, min(left, min(center, right)));
    delay(10); // Stability delay
  }

  threshold = (blackValue + whiteValue) / 2;

  Serial.print("Calibration complete. Black Value: ");
  Serial.println(blackValue);
  Serial.print("White Value: ");
  Serial.println(whiteValue);
  Serial.print("Threshold: ");
  Serial.println(threshold);
}

// Adjust Motor Speeds Based on Correction
void adjustMotors(int correction) {
  int leftMotorSpeed = constrain(motorSpeed - correction, 0, 255);
  int rightMotorSpeed = constrain(motorSpeed + correction, 0, 255);

  analogWrite(EnableA, leftMotorSpeed);
  analogWrite(EnableB, rightMotorSpeed);

  if (correction > 0) {
    digitalWrite(INA1, HIGH);
    digitalWrite(INA2, LOW);
    digitalWrite(INB1, LOW);
    digitalWrite(INB2, HIGH);
  } else if (correction < 0) {
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, HIGH);
    digitalWrite(INB1, HIGH);
    digitalWrite(INB2, LOW);
  } else {
    digitalWrite(INA1, HIGH);
    digitalWrite(INA2, LOW);
    digitalWrite(INB1, HIGH);
    digitalWrite(INB2, LOW);
  }
}

// Stop the Motors
void carStop() {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, LOW);
}

// Load a Preset Profile
void loadProfile(Profile p) {
  Kp = p.Kp;
  Ki = p.Ki;
  Kd = p.Kd;
  motorSpeed = p.motorSpeed;
  threshold = p.threshold;

  Serial.println("Profile Loaded:");
  Serial.print("Kp: "); Serial.println(Kp);
  Serial.print("Ki: "); Serial.println(Ki);
  Serial.print("Kd: "); Serial.println(Kd);
  Serial.print("Motor Speed: "); Serial.println(motorSpeed);
  Serial.print("Threshold: "); Serial.println(threshold);
}

// Enter Test Mode for Calibration and Sensor Testing
void enterTestMode() {
  Serial.println("Entering Test Mode...");
  calibrateSensors(); // Perform calibration

  // Output sensor values continuously for debugging
  while (true) {
    Serial.print("Left: "); Serial.print(analogRead(leftSensor));
    Serial.print(" | Center: "); Serial.print(analogRead(centerSensor));
    Serial.print(" | Right: "); Serial.println(analogRead(rightSensor));
    delay(500);
  }
}