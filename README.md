# Accident Detection Alert System

Accident detection system using an MPU6050 motion sensor and an HC-SR04 ultrasonic sensor. It shows collision warnings with an RGB LED and buzzer, and sends a Telegram message when a sudden impact is detected.

## How it works

**Obstacle warning (HC-SR04)**

| Distance | LED | Buzzer | Status |
|---|---|---|---|
| More than 100 cm | Green | Off | SAFE |
| 20 – 100 cm | Blue | On | COLLISION POSSIBLE |
| Less than 20 cm | Red | On | DANGER |

**Accident detection (MPU6050)**

- The total acceleration on the X, Y and Z axes is checked every loop.
- If it goes above **25**, a Telegram alert is sent: "🚨 Accident detected! Sudden movement detected by MPU6050."
- Only one alert is sent per event. It resets once the movement drops below **12**.

## Components

- ESP32 development board
- MPU6050 accelerometer / gyroscope
- HC-SR04 ultrasonic sensor
- RGB LED + 3 resistors (220 Ω)
- Buzzer
- Jumper wires, breadboard

## Wiring

| Part | Pin | ESP32 pin |
|---|---|---|
| HC-SR04 | TRIG | 5 |
| HC-SR04 | ECHO | 18 |
| RGB LED | Red | 25 |
| RGB LED | Green | 26 |
| RGB LED | Blue | 27 |
| Buzzer | + | 14 |
| MPU6050 | SDA | 21 |
| MPU6050 | SCL | 22 |

## Libraries

- UniversalTelegramBot
- ArduinoJson (needed by UniversalTelegramBot)
- Adafruit MPU6050
- Adafruit Unified Sensor

## Setup

Fill in these values in the code before uploading:

```cpp
const char* ssid = "..";       // WiFi name
const char* password = "..";   // WiFi password
#define BOT_TOKEN ".."         // Telegram bot token from @BotFather
#define CHAT_ID ".."           // Your Telegram chat ID
```

## How to run

1. Install the libraries above from the Library Manager.
2. Select an ESP32 board in the Arduino IDE.
3. Fill in the WiFi and Telegram details.
4. Upload and open the Serial Monitor at 115200 baud.
