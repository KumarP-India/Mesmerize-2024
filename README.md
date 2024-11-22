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
3. Test and Adjust: Place the robot on the line and observe its behavior. Modify accoding to below for better performance.

#### Tuning

Firstly we calibrate sensors by placing center IR on black line and other on white. We note down the `threshold` to all presets. We run this by Shorting the `modePin` with 5V and then reading serial inputs. Afterwards it also shows us IT values which we test by manually moving the IR (make sure motor is disabled.) For our convinence Robot has delay of 3 seconds becore calibrating sensors.

**1. Proportional Gain**

`Kp`: Controls how aggressively the robot corrects deviations from the line.

*Incresing `Kp`*
- Makes faster correction.
- But may cause overshooting or oscillations.

*Decreasing `Kp`*
- Slower but smoother corrections.
- Consequently may cause the robot to lag behind the line or drift away.

*Statergy*
- Start with `kp = 1.0`.
- Gradually increase it.
- If the robot reacts sluggishly (too slow or late) to deviations (changes in line or when bot is off track), keep increasing `Kp`.
- If the robot starts oscillating around the line, reduce `Kp` slightly.
- We want keep `Kp` as high as possible without causing oscillation.


**2. Integral Gain**

`Ki`: Addresses accumulated small errors over time, like drifting due to uneven surfaces or minor misalignments.

This is for past erros. If a bot, for an instance, slightly off left then it doesn't matter at that moment but as it progress the deviation would eventully trigger unwanted actions and thus by keeping track of past errors we can anticipate them prior to happening and try to prevent and correct as much as possible.

*Increasing `Ki`* 
- Helps correcting long-term drift.
- But can cause overcorrection or integral wind-up (error keeps growing uncontrollably).

*Decreasing Ki*
- Reduces long-term corrections, but may leave residual drift.

*Statergy*
- Start with `Ki = 0`.
- Only increase Ki if:
  - The robot consistently drifts off the line over time.
  - The robot doesn’t seem to “settle” on the line.
- Increase slowly (steps of 0.1) and monitor for overcorrection.


**Derivative Gain**

`Kd`: Dampens oscillations by predicting future errors based on the rate of change of error.

*Increasing `Kd`*
- Reduces oscillations.
- Slows down corrections (may feel smoother but less responsive).

*Decreasing `Kd`*
- Increases oscillations if `Kp` is high.
- Faster corrections but may feel jittery.

*Statergy*
- Start with `Kd = 0.5`.
- Increase slowly (steps of 0.1) if the robot oscillates around the line even with a stable `Kp`.
- Stop increasing if the robot becomes too slow in correcting its path.

**How to manage Tuning**
The program is built to handle multiple senerio. It manages this have precalibrated presets. The robot needs to be calibrated prior to actual run. And just before the run, robot must choose best preset based on the path.

To determine values for these we test on following conditions:

1. On straight paths:

  - If the robot drifts out of track, increase `Kp`.
  - If it wobbles, increase `Kd`.

2. On sharp turns:
  - If the robot misses turns, increase `Kp`.
  - If it overshoots, increase `Kd` or reduce `Kp`.
    - Reduce `Kp` when it is sudden reaction (sharp reacktion).
    - Increase `Kd` when it wobbling even after the trigger event.
    - Do both when reaction is too high!

3. On Curve turns: Just test it works as intended.

4. On actual path test run you may adjust acording to below.

**Path last minute correction**

1.   Run on `modePin` HIGH to calibrate sensors, note down new `threshold` to all presets.

2.	Common Observations and Fixes:
	- Robot Drifts Away from Line:
		- Cause: `Kp` is too low.
		- Fix: Increase `Kp` slightly.
	- Robot Oscillates or Wobbles:
		- Cause: `Kp` is too high or `Kd` is too low.
		- Fix: Reduce `Kp` or increase `Kd`.
	- Robot Fails on Sharp Turns:
		- Cause: Speed too high or `Kp` too low.
		- Fix: Lower speed or increase `Kp`.


3.	Adjust Parameters Based on Track Observations:
	- If the track has sharp turns:
		- Use `sharpTurnsProfile`.
	- If the track has smooth, long paths:
		- Use `highSpeedProfile`.

	- If robot performs very badly with selected profile and/or it is complicated combination then use `defaultProfile`.


### License

This project is open-source and free for educational and competition use. Contributions and suggestions are welcome!

[Full License is here](./LICENSE)