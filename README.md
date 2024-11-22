# Mesmerize’24: Line Following Robot

This repository contains the Arduino Nano-based sketch for the Mesmerize’24 Line Following Competition. The robot uses IR sensors to detect and follow a line, controlling two DC motors for navigation.

### Features

- IR Sensors: Three sensors for line detection (left, center, right).
- Motor Control: H-bridge motor drivers to manage forward movement, turning, and stopping.
- Customizable Speed: Adjustable motor speed via the motorSpeed variable in the code.

### Hardware Requirements

- Arduino Nano (or equivalent microcontroller)
- 3 IR Sensors
- Dual H-bridge Motor Driver (e.g., L298N)
- 2 DC Motors
- Power Supply (suitable for motors and Arduino)

### Pin Connections

| Component       | Pin Number |
|------------------|------------|
| Left Sensor      | 6          |
| Center Sensor    | 7          |
| Right Sensor     | 8          |
| Motor A Enable   | 9          |
| Motor A Input 1  | 2          |
| Motor A Input 2  | 3          |
| Motor B Enable   | 10         |
| Motor B Input 1  | 4          |
| Motor B Input 2  | 5          |

### Usage

1. Hardware Setup: Connect components as per the pin configuration.
2. Upload the Code: Load the sketch onto your Arduino Nano using the Arduino IDE.
3. Test and Adjust: Place the robot on the line and observe its behavior. Modify motorSpeed or tweak the logic for better performance.

### License

This project is open-source and free for educational and competition use. Contributions and suggestions are welcome!

[Full License is here](./LICENSE)