# Single Button Game Console with Arduino 🎮

A minimalist handheld game console powered by an Arduino Nano and operated using just a single button. This project is based on the original tutorial from [Instructables](https://www.instructables.com/Single-Button-Game-Console-With-Arduino-and-PCB/).

---

## 🧰 Overview

This tiny console plays a Flappy Bird-style game with just one input — perfect for retro gaming enthusiasts and Arduino beginners. The console uses an OLED screen, a push button, and a small number of components soldered onto a custom PCB or perfboard.

---

## 🧠 Features

- One-button gameplay
- OLED screen (SSD1306) support
- 3D-printable case and optional PCB
- Portable and rechargeable (if using LiPo)

---

## 🧾 Bill of Materials

- Arduino Nano
- SSD1306 OLED Display (I2C)
- Tactile push button
- Resistors and capacitor (as needed)
- Battery or USB power
- Optional: 3D printed enclosure and custom PCB

---

## 📲 Controls

- **Single Button**: Tap to jump, avoid pipes!

---

## ⚙️ Setup Instructions

1. Assemble the components according to the circuit shown in the original guide.
2. Upload the `FlappyBird.ino` sketch in the `arduino/` folder using Arduino IDE.
3. Ensure the OLED is wired via I2C (A4/A5 on Nano).
4. Press the button to start playing.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 🙌 Credits

Originally published on [Instructables](https://www.instructables.com/Single-Button-Game-Console-With-Arduino-and-PCB/)

Created by Ioannis Mastoras
