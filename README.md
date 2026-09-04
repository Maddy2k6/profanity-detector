# 🚨 AI + IoT Profanity Detection System

An AI and IoT-based real-time profanity detection system that connects software-based detection with physical hardware.

When the system detects profanity, it sends a `FOUND` signal to an ESP32 over Wi-Fi. The ESP32 then activates physical alerts such as an RGB LED, buzzer, servo motor, and LCD display.

A live web dashboard displays detection statistics, activity graphs, hardware status, detection history, session uptime, and more.

---

# 📌 Table of Contents

- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Required](#hardware-required)
- [Software Required](#software-required)
- [Project Structure](#project-structure)
- [Hardware Connections](#hardware-connections)
- [Installation](#installation)
- [ESP32 Setup](#esp32-setup)
- [Python Setup](#python-setup)
- [Running the Project](#running-the-project)
- [How the System Works](#how-the-system-works)
- [Dashboard](#dashboard)
- [Reset System](#reset-system)
- [API Endpoints](#api-endpoints)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)

---

# ✨ Features

- 🤖 AI-based profanity detection
- 🌐 Real-time ESP32 communication over Wi-Fi
- 📊 Live web dashboard
- 🔴 Red alert when profanity is detected
- 🟢 Safe detection indication
- 🔊 Buzzer alert
- ⚙️ Servo motor movement
- 📟 I2C LCD profanity counter
- 📈 Live activity graph
- 📈 Detection spikes when profanity is found
- 📜 Detection history
- ⏱️ Session uptime
- 🕒 Last detection time
- 🔌 Hardware connection status
- ❤️ Connection health monitoring
- 🔄 Reset session functionality
- 🧹 ESP32 and LCD counter reset synchronization

---

# 🏗️ System Architecture

```text
AI / Detection System
        │
        ▼
 Tampermonkey / Input
        │
        ▼
   Flask Receiver
        │
        ├──────────────► Web Dashboard
        │
        ▼
       ESP32
        │
 ┌──────┼───────┬───────┐
 ▼      ▼       ▼       ▼
RGB   Buzzer  Servo    LCD
LED           Motor   Counter
