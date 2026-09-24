 🏠 Bluetooth-Controlled Home Automation System

An Embedded Systems project based on **ATmega32** microcontroller using **Layered Software Architecture** (MCAL, HAL, APP).



 📄 Project Documentation
You can check the full technical documentation, schematics, and flowcharts here:
👉 **[View Full Project Documentation (PDF)](./Smart%20Home%20Documentation.pdf)**



 🏗️ Software Architecture
- **APP Layer:** System main logic and configuration files.
- **HAL Layer:** LCD, Keypad, Servo Motor, Motors, EEPROM, Buzzer, Temp Sensor, LEDs.
- **MCAL Layer:** DIO, ADC, UART, TWI (I2C), Timers, SPI, EXTI.



 ⚙️ Features
- **Password Security & Servo Door Lock:** 4-digit password entry via Keypad with lock mechanism on wrong attempts.
- **Dual-Room Lighting Control:** Independent control using relays and transistors.
- **Live Temperature Display & Auto AC Control:** Real-time monitoring via LM35 & automatic cooling activation at 30°C.
- **Hybrid Control:** Local operation via buttons and wireless remote control via Bluetooth.
- **EEPROM Logging:** Event history logging over I2C.
