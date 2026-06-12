# ESP32 RC Car

This repository is for learning ESP32 development and C++ by building a small
remote-controlled car from scratch.

## 1. Suggested ESP32 model/module to buy

For this project, buy an **ESP32 DevKit development board based on the
ESP32-WROOM-32 module**. Common names you may see online include:

- ESP32 DevKitC
- ESP32 DevKit V1
- ESP32-WROOM-32 development board

This is the best beginner choice for an RC car because it is inexpensive,
widely available, well documented, and has enough GPIO/PWM pins to control
motors, servos, LEDs, sensors, and a wireless receiver or controller. It also
has built-in Wi-Fi and Bluetooth, so the car can later be controlled from a
phone, web page, joystick, or another ESP32.

When buying one, look for:

- An ESP32-WROOM-32 module on the board
- A USB connector for programming, either USB-C or micro-USB
- Pin labels printed on the board
- A 5V/VIN pin and 3.3V pin
- 30-pin or 38-pin layout; either is fine for this project

Do not power motors directly from the ESP32 pins. The ESP32 controls the car,
but motors need a separate motor driver board and a battery power source.
