# 🔋 Bidirectional Onboard EV Charger using Interleaved Boost Converter and PSFB Converter

> A college-level EV Onboard Charger (OBC) prototype implementing an **Interleaved Boost Converter** and **Phase Shifted Full Bridge (PSFB)** topology using Arduino, IR2110 gate drivers, and high-frequency transformers.

---

# 📌 Project Overview

This project demonstrates a simplified **Onboard Charger (OBC)** architecture used in Electric Vehicles (EVs).

The system consists of two major power stages:

## ✅ Stage 1 — Interleaved Boost Converter
Converts low-voltage DC into a regulated high-voltage DC bus.

## ✅ Stage 2 — Phase Shifted Full Bridge (PSFB)
Converts the high-voltage DC bus into isolated DC suitable for battery charging.

---

# ⚡ System Architecture

```text
12V DC Input
      ↓
Interleaved Boost Converter
      ↓
100V–150V DC Bus
      ↓
PSFB Converter
      ↓
High Frequency Transformer (ETD34)
      ↓
Rectifier + Filter
      ↓
Battery Charging Output
```

---

# 🧠 Main Features

- Interleaved Boost topology
- Reduced ripple current
- High-frequency PSFB converter
- Transformer isolation
- IR2110 MOSFET gate driving
- Arduino-based PWM generation
- Soft switching concept (PSFB)
- LCD + Blynk IoT monitoring
- Bidirectional DAB experimental control
- Battery voltage monitoring

---

# 🔧 Hardware Components

## 🟢 Control Section

| Component | Quantity |
|-----------|----------|
| Arduino UNO | 1 |
| ESP32 | 1 |
| IR2110 Gate Driver | 2–3 |
| SG3525 Board | Optional |

---

## 🔵 Power Section

| Component | Quantity |
|-----------|----------|
| IRFP240 MOSFET | 4–6 |
| Interleaved Inductor | 2 |
| ETD34 Ferrite Core | 1 |
| High Voltage Capacitors | Multiple |
| Fast Recovery Diodes | Multiple |
| Relay Module | 2 |

---

# 🔷 Interleaved Boost Converter

# 📖 Working Principle

The interleaved boost converter consists of two boost phases operating 180° out of phase.

Each phase contains:
- MOSFET
- Inductor
- Diode

The two phases share current equally.

---

# 🔥 Advantages of Interleaving

- Reduced input current ripple
- Reduced output ripple
- Improved thermal performance
- Higher efficiency
- Better power handling capability

---

# 📊 Boost Converter Equation

```math
Vout = Vin / (1 - D)
```

Where:

- `Vin` = Input voltage
- `D` = Duty cycle

---

# 🔌 Interleaved Boost Connections

## Arduino UNO

| Pin | Function |
|-----|----------|
| D9 | Phase-A PWM |
| D10 | Phase-B PWM |

---

## IR2110

| IR2110 Pin | Connection |
|------------|------------|
| LIN | Arduino PWM |
| LO | MOSFET Gate |
| COM | GND |

---

# 🧾 Interleaved Boost PWM Code

```cpp
/*
  INTERLEAVED BOOST CONVERTER
  Arduino UNO
*/

#define DUTY_PERCENT 80

void setup() {

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Fast PWM Mode 14
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);

  // D9 normal
  TCCR1A |= (1 << COM1A1);

  // D10 inverted
  TCCR1A |= (1 << COM1B1) | (1 << COM1B0);

  // 20kHz
  ICR1 = 799;

  uint16_t duty = (ICR1 * DUTY_PERCENT) / 100;

  OCR1A = duty;
  OCR1B = duty;

  TCCR1B |= (1 << CS10);
}

void loop() {}
```

---

# 🔴 Phase Shifted Full Bridge (PSFB)

# 📖 Working Principle

The PSFB converter converts the high-voltage DC bus into isolated high-frequency AC.

The converter uses:
- 4 MOSFET full bridge
- High-frequency transformer
- Phase-shift control

Diagonal MOSFET pairs switch alternately:

```text
Q1 + Q4 → Positive cycle
Q2 + Q3 → Negative cycle
```

---

# ⚡ Advantages of PSFB

- Transformer isolation
- High efficiency
- Soft switching capability
- Reduced switching losses
- High power density

---

# 🔧 Transformer Details

| Parameter | Value |
|-----------|------|
| Core | ETD34 |
| Primary Turns | 30 Turns |
| Secondary Turns | 10 Turns |
| Ratio | 3:1 |
| Primary Wire | 0.8 mm |
| Secondary Wire | 1 mm |
| Frequency | 62.5 kHz |

---

# 🔌 PSFB Connections

## Arduino UNO

| Pin | Function |
|-----|----------|
| D3 | HIN |
| D11 | LIN |

---

## IR2110

| IR2110 Pin | Connection |
|------------|------------|
| HIN | Arduino PWM |
| LIN | Complementary PWM |
| HO | High-side MOSFET |
| LO | Low-side MOSFET |

---

# 🧾 PSFB PWM Code

```cpp
/*
  PSFB FULL BRIDGE
*/

void setup() {

  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  // Fast PWM
  TCCR2A |= (1 << WGM21) | (1 << WGM20);

  // Complementary outputs
  TCCR2A |= (1 << COM2B1);
  TCCR2A |= (1 << COM2A1) | (1 << COM2A0);

  // ~31kHz
  TCCR2B |= (1 << CS20);

  // 50% duty
  OCR2B = 128;
  OCR2A = 128;
}

void loop() {}
```

---

# 🔥 MOSFET Snubber Circuit

Each MOSFET uses an RC snubber:

```text
Drain ── R ── C ── Source
```

Recommended values:

| Component | Value |
|-----------|------|
| Resistor | 100Ω |
| Capacitor | 1nF–2.2nF |

---

# 📡 IoT Monitoring

The project uses:
- ESP32
- Blynk IoT
- LCD Display

Features:
- Voltage monitoring
- SOC estimation
- Direction control
- Wireless monitoring

---

# 📈 Experimental Results

| Parameter | Result |
|-----------|-------|
| Boost Output | 100V–150V |
| Switching Frequency | 20kHz–62.5kHz |
| Transformer Output | ~40V AC |
| Topology | Interleaved + PSFB |

---

# ⚠️ Safety Precautions

- Use isolated supply during testing
- Start with low voltage
- Use heatsinks for MOSFETs
- Verify gate signals before high voltage testing
- Use proper grounding
- Avoid direct mains testing initially

---

# 🎓 Educational Concepts Covered

- DC-DC Converters
- Interleaving
- PWM Generation
- PSFB Topology
- Transformer Design
- MOSFET Gate Driving
- Soft Switching
- EV Charging Systems
- High Frequency Magnetics

---

# 🚀 Future Improvements

- Closed-loop voltage regulation
- Current feedback control
- DSP-based control
- GaN MOSFET implementation
- PFC front-end integration
- CAN communication
- BMS integration

---

# 📚 References

1. EV Onboard Charger Research Papers
2. IR2110 Datasheet
3. PSFB Converter Design Notes
4. Interleaved Boost Converter Theory
5. Arduino Timer PWM Documentation

---

# 👨‍💻 Author

## Abhishek Kaladharan

Embedded & Power Electronics Enthusiast

- EV Systems
- Embedded Systems
- Power Electronics
- Automotive Electronics

---

# ⭐ Project Status

✅ Prototype Completed  
✅ PWM Generation Verified  
✅ Transformer Tested  
✅ Interleaved Boost Working  
✅ PSFB Working  
🔄 Closed-loop control under development

---
