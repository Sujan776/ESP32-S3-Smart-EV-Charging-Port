# ESP32-S3 Based Smart EV Charging Port & Safety Monitoring System

Low-voltage educational simulation of a smart EV charging interface using ESP32-S3.

## Features
- Battery SOC simulation
- Charging current simulation
- LM35 temperature monitoring
- Vehicle connect/disconnect toggle
- Relay charging control
- Servo connector locking
- OLED dashboard
- Green/yellow/red status LEDs
- Piezo safety alarm
- Automatic over-current and over-temperature shutdown

> Educational simulation/prototype only. Do not connect to mains, high-voltage EV batteries, or real EV charging equipment.

## Pin Configuration

| Component | GPIO |
|---|---:|
| SOC Trimmer | 1 |
| Current Trimmer | 2 |
| LM35 | 3 |
| Pushbutton | 4 |
| Green LED | 5 |
| Yellow LED | 6 |
| Red LED | 7 |
| OLED SDA | 8 |
| OLED SCL | 9 |
| Piezo | 10 |
| Relay K1 | 11 |
| Servo PWM | 12 |

## Safety Thresholds
- Over-current: **>= 7.5 A**
- Over-temperature: **>= 60 °C**
- Temperature warning: **>= 50 °C**

## Project Structure
```text
ESP32-S3-Smart-EV-Charging-Port/
├── src/
│   └── main.cpp
├── images/
├── platformio.ini
└── README.md
```
