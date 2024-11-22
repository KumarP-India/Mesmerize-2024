/*
 * @Author: Prabhas Kumar
 * @Assistant: OpenAI ChatGPT (GPT 4o)

 * @Created: November 22'24
 * @Updated: None

 * @Project: Mesmerize'24 [Line Following Competition] 
 * @File: ArduinoNanoSketch.ino [simplified C++ script]
 */


// IR sensors
#define leftSensor 6
#define centerSensor 7
#define rightSensor 8

// Motor one
#define EnableA 9
#define INA1 2
#define INA2 3

// Motor two
#define EnableB 10
#define INB1 4
#define INB2 5

int motorSpeed = 120; // motorSpeed of this robot

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(leftSensor, INPUT);
  pinMode(centerSensor, INPUT);
  pinMode(rightSensor, INPUT);

  pinMode(EnableA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(EnableB, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  bool leftValue = digitalRead(leftSensor);
  bool centerValue = digitalRead(centerSensor);
  bool rightValue = digitalRead(rightSensor);

  // Serial.println(rightValue);

  if (leftValue == 0 && centerValue == 0 && rightValue == 0){
    carStop();
  } else if (leftValue == 0 && centerValue == 0 && rightValue == 1){
    carForward();
  } else if (leftValue == 0 && centerValue == 1 && rightValue == 0){
    // Fuck
  } else if (leftValue == 0 && centerValue == 1 && rightValue == 1){
    carLeft();
  } else if (leftValue == 1 && centerValue == 0 && rightValue == 0){
    carForward();
  } else if (leftValue == 1 && centerValue == 0 && rightValue == 1){
    carForward();
  } else if (leftValue == 1 && centerValue == 1 && rightValue == 0){
    carRight();
  } else {
    // Fuck
  }

}

void carForward() {
  analogWrite(EnableA, motorSpeed);
  analogWrite(EnableB, motorSpeed);
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
}

void carLeft() {
  analogWrite(EnableA, motorSpeed);
  analogWrite(EnableB, motorSpeed);
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
}

void carRight() {
  analogWrite(EnableA, motorSpeed);
  analogWrite(EnableB, motorSpeed);
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
}

void carStop() {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, LOW);
}

