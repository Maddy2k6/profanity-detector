# 🤬 Profanity Detection System

A hardware + software system that detects profanity in real time and reacts with a **physical alert** — RGB lighting, an LCD readout, a servo, and a buzzer — while a **live web dashboard** tracks the running count.

![Status](https://img.shields.io/badge/status-active-brightgreen)
![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Backend](https://img.shields.io/badge/backend-Flask-lightgrey)

---

## 🧠 Overview

This project is a three-part pipeline:

1. **Detection source** — any speech-to-text / language-moderation pipeline (e.g. a live transcription + LLM moderation script) that classifies incoming speech as containing profanity or not, and reports the result as `FOUND` or `NOTFOUND`.
2. **Receiver server (`reciever.py`)** — a Flask server that receives that state, logs it, forwards it to the ESP32, and serves a live dashboard.
3. **ESP32 hardware (`profanity_detector.ino`)** — a microcontroller that reacts physically to each detection: an RGB LED changes color, an LCD shows the live count, a servo "flags" the event, and a buzzer plays an alert chime.

```
[Speech/Text Source] → POST /state → [Flask Receiver] → HTTP GET → [ESP32 Hardware]
                                            │
                                            └──> Serves live dashboard (index.html)
```

---

## ✨ Features

- 🔴🟢🟣 **RGB LED feedback** — red on detection, green when clear, violet when idle
- 📟 **16x2 LCD display** showing live status and running count
- 🔊 **Buzzer alert** with a short two-tap + closing beep pattern
- 🦾 **Servo "flag"** that moves on detection and resets automatically
- 🖥️ **Live web dashboard** (auto-refreshes every second) showing:
  - Total curses detected
  - Current detection status
  - Receiver/ESP32 connection status
- 🔁 **One-click session reset** — resets both the dashboard counter and the physical ESP32 counter
- 🌐 **Simple REST-style HTTP API** connecting all three components

---

## 📁 Repository Structure

```
.
├── profanity_detector.ino   # ESP32 firmware (WiFi, LCD, RGB LED, servo, buzzer)
├── reciever.py              # Flask receiver server + REST API + dashboard host
└── index.html               # Live web dashboard UI
```

---

## 🛠️ Hardware Requirements

| Component | Notes |
|---|---|
| ESP32 dev board | Any board with WiFi support |
| 16x2 I2C LCD | Default address `0x27` |
| RGB LED (common cathode) | Connected to pins 25 (R), 26 (G), 27 (B) |
| Servo motor | Signal pin 18 |
| Buzzer | Active-low, pin 19 |

### Pin Reference

| Function | GPIO |
|---|---|
| RGB Red | 25 |
| RGB Green | 26 |
| RGB Blue | 27 |
| Servo signal | 18 |
| Buzzer | 19 |
| I2C SDA / SCL (LCD) | 21 / 22 |

---

## 💻 Software Requirements

**Arduino / ESP32 firmware:**
- [Arduino IDE](https://www.arduino.cc/en/software) or PlatformIO
- Libraries: `WiFi`, `WebServer`, `Wire`, `LiquidCrystal_I2C`, `ESP32Servo`

**Python receiver:**
- Python 3.8+
- `flask`
- `flask-cors`
- `requests`

Install the Python dependencies:

```bash
pip install flask flask-cors requests
```

---

## 🚀 Setup

### 1. Flash the ESP32

1. Open `profanity_detector.ino` in the Arduino IDE.
2. Install the required libraries via Library Manager.
3. Update your WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
4. Wire up the LCD, RGB LED, servo, and buzzer per the pin table above.
5. Upload the sketch, then open the Serial Monitor (115200 baud) to find the ESP32's IP address once it connects to WiFi.

### 2. Configure the receiver

In `reciever.py`, set the ESP32's IP address to match what was printed to Serial:

```python
ESP32_IP = "192.168.x.x"
```

### 3. Run the receiver server

```bash
python reciever.py
```

The dashboard will be available at:

```
http://127.0.0.1:5000
```

### 4. Feed it detections

Send a `POST` request to `/state` with a JSON body whenever your speech/text pipeline classifies an utterance:

```bash
curl -X POST http://127.0.0.1:5000/state \
  -H "Content-Type: application/json" \
  -d '{"state": "FOUND"}'
```

Valid values for `state` are `FOUND` and `NOTFOUND`.

---

## 🔌 API Reference

| Endpoint | Method | Description |
|---|---|---|
| `/` | GET | Serves the dashboard (`index.html`) |
| `/state` | POST | Reports a new detection state (`FOUND` / `NOTFOUND`) and relays it to the ESP32 |
| `/api/status` | GET | Returns current counts, latest state, and recent event history (used by the dashboard) |
| `/api/reset` | POST | Resets the counter on both the dashboard and the ESP32 |

**ESP32 endpoints** (called internally by the receiver):

| Endpoint | Description |
|---|---|
| `/found` | Triggers the alert sequence and increments the counter |
| `/notfound` | Clears any alert state |
| `/reset` | Resets the ESP32's counter to zero |

---

## 📸 Dashboard Preview

The dashboard shows a large live counter, a color-coded status pill (waiting / detected / clear), a connection indicator for the receiver, and a reset button.

---

## 🧩 Notes

- This repo does **not** include the actual speech-to-text / profanity-classification component — bring your own (e.g. a live transcription script paired with a moderation model) that `POST`s to `/state`.
- The ESP32 and the machine running `reciever.py` must be on the **same local network** so the receiver can reach the ESP32's IP directly.
- If the ESP32 is unreachable, the receiver still tracks state and updates the dashboard — it just reports `esp32_success: false` in the response.

---

## 📄 License

Add a license of your choice (e.g. MIT) — none is currently specified for this repository.
