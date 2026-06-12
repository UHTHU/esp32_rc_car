# ESP32 RC Car

This repository is for learning ESP32 development and C++ by building a small
remote-controlled car from scratch.

## 1. Exact modules/parts to buy

Use these exact module names or part numbers when searching online. Some cheap
robot car parts are sold by many factories, so the "module number" is often the
chip or standard part name instead of one official manufacturer SKU.

| Purpose | Exact part/module to buy | Quantity | Notes |
| --- | --- | --- | --- |
| Main controller | **Espressif ESP32-DevKitC-32E** development board | 1 | Uses the **ESP32-WROOM-32E** module. This is the recommended ESP32 board for the car. |
| Motor driver | **TB6612FNG dual DC motor driver module** | 1 | Better beginner choice than L298N because it wastes less power. Drives two DC motors. |
| Chassis | **2WD smart robot car chassis kit for TT motors** | 1 | Buy a kit that includes the acrylic base plate, screws, and motor brackets. |
| Left/right drive motors | **TT DC gear motor, 3-6V, 1:48 ratio** | 2 | Usually sold as yellow plastic "TT motor" gearboxes. |
| Wheels for TT motors | **TT motor wheel, 65 mm** | 2 | Must match the TT motor shaft. |
| Front caster wheel | **Universal caster wheel, 20 mm or 25 mm** | 1 | Used for a simple 2-wheel-drive car. |
| Distance sensor | **HC-SR04P ultrasonic sensor module** | 1 | Optional at first. The HC-SR04P version supports 3.3V logic for ESP32. |
| Line sensor | **TCRT5000 infrared reflectance sensor module** | 2 | Optional, useful for line-following lessons. |
| Status LED | **5 mm LED** with **220 ohm resistor** | 1 each | Useful for beginner output tests. |
| Breadboard wiring | **Dupont jumper wires, male-to-female and male-to-male** | 1 kit | Needed to connect modules while learning. |
| Power switch | **SS12D00G3 slide switch** module or inline switch | 1 | Lets you turn the car battery on/off. |
| Battery holder | **2 x 18650 battery holder with leads** | 1 | Provides motor power. Use protected 18650 cells from a reputable seller. |
| 5V regulator | **LM2596S DC-DC buck converter module** | 1 | Set this to 5V before connecting it to the ESP32 VIN/5V pin. |
| USB cable | USB cable matching your ESP32 board, usually **USB-C** or **micro-USB** | 1 | Needed for programming from your computer. |

If you want a physical handheld controller instead of controlling the car from a
phone or web page, also buy:

| Purpose | Exact part/module to buy | Quantity | Notes |
| --- | --- | --- | --- |
| Controller ESP32 | **Espressif ESP32-DevKitC-32E** development board | 1 | A second ESP32 can act as the remote controller. |
| Joystick input | **KY-023 joystick module** | 1 | Provides X axis, Y axis, and button input. |

### Why this ESP32 board?

The **ESP32-DevKitC-32E** with **ESP32-WROOM-32E** is the best beginner choice
for an RC car because it is inexpensive, widely available, well documented, and
has enough GPIO/PWM pins to control motors, servos, LEDs, sensors, and a
wireless receiver or controller. It also has built-in Wi-Fi and Bluetooth, so
the car can later be controlled from a phone, web page, joystick, or another
ESP32.

Do not power motors directly from the ESP32 pins. The ESP32 controls the car,
but motors need the **TB6612FNG** motor driver and a separate battery power
source.
