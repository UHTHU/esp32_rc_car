# Beginner wiring: VCC, GND, GPIO, and choosing pins

When you connect modules to the ESP32, most wiring tables use names like
**VCC**, **GND**, **AO**, **DO**, **TRIG**, **ECHO**, and **GPIO**. This guide
explains what those names mean and how to know where each wire goes.

## What is VCC?

**VCC** means the positive power input for a module.

The name comes from older electronics terminology, but on beginner modules it
usually just means "connect this pin to the module's positive supply voltage."

You can think of it as:

```text
VCC = the + power pin for the module
GND = the - / ground pin for the module
```

For this ESP32 car, most small sensor modules should use **3.3V** power because
the ESP32 uses 3.3V logic.

Examples:

| Module pin | Connect to | Why |
| --- | --- | --- |
| TCRT5000 VCC | ESP32 3V3 | Powers the IR sensor safely. |
| HC-SR04P VCC | ESP32 3V3 | The HC-SR04P supports 3.3V logic. |
| TB6612FNG VCC | ESP32 3V3 | Powers the motor driver's logic side. |
| TB6612FNG VM | Motor battery + | Powers the motors, not the ESP32. |

Important: **VCC is not always 3.3V.** It means "connect the correct positive
voltage for this module." Some modules want 3.3V, some want 5V, and some accept
both.

## What is GND?

**GND** means ground. Ground is the shared reference point for voltage.

For the RC car, all grounds must connect together:

- ESP32 GND
- Sensor GND
- TB6612FNG GND
- Motor battery negative wire
- Buck converter GND

If grounds are not connected together, the ESP32 and modules may not understand
each other's signals.

## What is GPIO?

**GPIO** means "general-purpose input/output." These are the ESP32 pins your
program can read from or write to.

Examples:

| Job | Pin type | Example from this car |
| --- | --- | --- |
| Read an analog IR sensor value | Analog input / ADC pin | GPIO34, GPIO35 |
| Send an ultrasonic trigger pulse | Digital output | GPIO5 |
| Read an ultrasonic echo pulse | Digital input | GPIO18 |
| Control motor speed | PWM output | GPIO25, GPIO14 |
| Control motor direction | Digital output | GPIO26, GPIO27, GPIO12, GPIO13 |

## How do I know what pin to connect?

Use this process every time you wire a new module.

### 1. Read the labels printed on the module

Most modules have tiny labels beside each pin. For example, a TCRT5000 sensor
often has:

```text
VCC  GND  DO  AO
```

That tells you:

- `VCC` goes to power
- `GND` goes to ground
- `AO` is analog output
- `DO` is digital output

For the PID line follower, use **AO** because PID needs smooth analog readings.

### 2. Check the module's voltage

Before connecting VCC, check whether the module supports **3.3V**.

For this project:

- Use **3.3V-compatible modules** when possible.
- The **HC-SR04P** ultrasonic module is preferred because it supports 3.3V.
- A regular **HC-SR04** often outputs a 5V ECHO signal, which is not safe for an
  ESP32 pin unless you add a voltage divider or logic level shifter.

Rule for beginners:

```text
If a signal wire goes into an ESP32 GPIO pin, keep that signal at 3.3V or less.
```

### 3. Use the tutorial wiring table

The tutorial gives the exact ESP32 pin for each module pin. For example:

| Module pin | ESP32 pin |
| --- | --- |
| Left IR AO | GPIO34 |
| Right IR AO | GPIO35 |
| Ultrasonic TRIG | GPIO5 |
| Ultrasonic ECHO | GPIO18 |

Follow the table first. Later, when you understand the pin types, you can choose
different pins and update the code.

### 4. Match the code to the wiring

The pin numbers in the code must match the wires on the car.

Example:

```cpp
const int leftIrPin = 34;
const int rightIrPin = 35;
const int trigPin = 5;
const int echoPin = 18;
```

This means:

- Left IR sensor AO must connect to GPIO34
- Right IR sensor AO must connect to GPIO35
- Ultrasonic TRIG must connect to GPIO5
- Ultrasonic ECHO must connect to GPIO18

If you move a wire to a different GPIO pin, change the matching number in the
code.

### 5. Use the correct ESP32 pin type

Not every ESP32 pin is equally useful.

For this car:

- Use **GPIO34 and GPIO35** for analog IR sensors.
- Use **GPIO25 and GPIO14** for motor PWM speed control.
- Avoid using ESP32 ADC2 pins for analog sensors if you plan to use Wi-Fi.
- Avoid pins marked only for power, ground, reset, or enable as signal pins.

## Quick wiring checklist

Before turning on power:

1. VCC goes to the correct voltage: usually 3V3 for these sensors.
2. GND from every board/module is connected together.
3. Signal pins match the tutorial wiring table.
4. Any signal entering the ESP32 is 3.3V or lower.
5. Motors are powered through the TB6612FNG motor driver, not from ESP32 pins.
