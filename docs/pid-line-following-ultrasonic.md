# PID line following with IR and ultrasonic sensors

This tutorial programs the ESP32 car to follow a dark line on a light floor
using two infrared sensors facing down. A forward-facing ultrasonic sensor stops
the car when something is too close.

## What the controller does

The car reads:

- **Left IR reflectance sensor** pointed at the floor
- **Right IR reflectance sensor** pointed at the floor
- **HC-SR04P ultrasonic sensor** pointed forward

The two IR sensors tell the ESP32 where the line is:

- Left sensor sees more line than right sensor: turn left
- Right sensor sees more line than left sensor: turn right
- Both sensors are balanced: drive straight

The PID controller turns that difference into a smooth steering correction.

## Wiring

Use ESP32 ADC1 pins for the IR sensors. ADC2 pins conflict with Wi-Fi on the
ESP32, so avoid ADC2 pins for analog sensors.

If terms like **VCC**, **GND**, **GPIO**, **AO**, or **DO** are new to you,
read [Beginner wiring: VCC, GND, GPIO, and choosing pins](beginner-wiring-vcc-gnd-gpio.md)
before connecting the car.

### Sensor wiring

| Part | Module pin | ESP32 pin |
| --- | --- | --- |
| Left TCRT5000 sensor | AO | GPIO34 |
| Right TCRT5000 sensor | AO | GPIO35 |
| Both TCRT5000 sensors | VCC | 3V3 |
| Both TCRT5000 sensors | GND | GND |
| HC-SR04P ultrasonic | TRIG | GPIO5 |
| HC-SR04P ultrasonic | ECHO | GPIO18 |
| HC-SR04P ultrasonic | VCC | 3V3 |
| HC-SR04P ultrasonic | GND | GND |

Use the **AO** analog output pins on the TCRT5000 modules, not the **DO**
digital output pins. PID needs changing analog values, not only on/off readings.

### TB6612FNG motor driver wiring

| TB6612FNG pin | ESP32 pin |
| --- | --- |
| PWMA | GPIO25 |
| AIN1 | GPIO26 |
| AIN2 | GPIO27 |
| PWMB | GPIO14 |
| BIN1 | GPIO12 |
| BIN2 | GPIO13 |
| STBY | GPIO33 |
| VCC | 3V3 |
| GND | GND |

Connect **VM** on the TB6612FNG to the motor battery positive wire. Connect the
motor battery ground to ESP32 ground so both circuits share a common reference.
Do not power motors from the ESP32.

## How PID works

The controller calculates an error:

```text
error = right_sensor - left_sensor
```

If the line is more under the left sensor, the error becomes negative and the
car turns left. If the line is more under the right sensor, the error becomes
positive and the car turns right.

PID has three parts:

- **P, proportional:** reacts to the current error
- **I, integral:** reacts to error that has built up over time
- **D, derivative:** reacts to how quickly the error is changing

For a beginner line-following car, start with **P** and **D**. Keep **I** at
`0.0` until the car already follows the line fairly well.

## Programming steps

1. Mount the two IR sensors at the front bottom of the car.
2. Put the sensors about 1-2 cm above the floor.
3. Space the sensors so the black line sits between them when the car is
   centered.
4. Mount the ultrasonic sensor facing forward.
5. Upload `examples/esp32_pid_line_ultrasonic/esp32_pid_line_ultrasonic.ino`.
6. Open the Serial Monitor at `115200` baud.
7. Move the sensors over the floor and the black line to check the readings.
8. Tune the PID values.

## Tuning the PID values

Start with these values in the sketch:

```cpp
float kp = 80.0;
float ki = 0.00;
float kd = 8.0;
```

Tune in this order:

1. Set `ki = 0.0` and `kd = 0.0`.
2. Increase `kp` until the car follows the line but wiggles too much.
3. Add `kd` a little at a time until the wiggle becomes smoother.
4. Only add a small `ki` value if the car always drifts to one side.

Common fixes:

| Problem | Try this |
| --- | --- |
| Car drives away from the line | Swap the left/right sensor pins or invert the error calculation. |
| Car wiggles hard | Lower `kp` or increase `kd` slightly. |
| Car reacts too slowly | Increase `kp` or raise `baseSpeed`. |
| Car loses the line on turns | Lower `baseSpeed` or move the sensors farther forward. |
| Car stops randomly | Lower `obstacleStopCm` or check the ultrasonic wiring. |

## Obstacle behavior

The example stops both motors when the ultrasonic sensor sees an object closer
than `obstacleStopCm`.

```cpp
const float obstacleStopCm = 20.0;
```

After the object is removed, the car continues line following.

## Safety checks

- Set the LM2596S buck converter to 5V before connecting it to ESP32 VIN/5V.
- Use 3.3V logic sensors with ESP32. The HC-SR04P is preferred over HC-SR04.
- Keep motor power separate from ESP32 logic power.
- Always connect all grounds together.
- Lift the car wheels off the table during the first motor test.
