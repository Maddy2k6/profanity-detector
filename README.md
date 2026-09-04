# 🚨 AI Profanity Detection System

An IoT-based real-time profanity detection and alert system that connects AI-based text detection with physical hardware using an ESP32.

The system detects whether profanity is present and sends the result to an ESP32 over Wi-Fi. When profanity is detected, the hardware provides visual, audio, and mechanical feedback while maintaining a live web dashboard and detection counter.

---

# ✨ Features

- 🤖 AI-based profanity detection
- 🌐 Real-time communication with ESP32 over Wi-Fi
- 📊 Live web dashboard
- 🔴 RGB LED alert when profanity is detected
- 🟢 Safe input indication
- 🔊 Buzzer alert pattern
- ⚙️ Servo motor movement on detection
- 📟 I2C LCD profanity counter
- 📈 Live activity graph with detection spikes
- 📜 Detection history
- ⏱️ Session uptime tracking
- 🕒 Last detection timestamp
- 🔌 ESP32 connection and hardware status
- 🔄 Session reset functionality
- 🧹 Reset synchronizes the dashboard and ESP32 LCD counter

---

# 🏗️ System Architecture

```text
AI / Profanity Detection
          │
          ▼
   Tampermonkey Script
          │
          ▼
    Flask Receiver
          │
     ┌────┴────┐
     ▼         ▼
Web Dashboard  ESP32
                  │
        ┌─────────┼─────────┐
        ▼         ▼         ▼
      RGB LED   Buzzer    Servo
        │
        ▼
      I2C LCD
