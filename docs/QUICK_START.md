# Quick Start Guide

## Getting Started

**Time required:**
- Initial setup: 5-10 minutes (one-time)
- First build & upload: 15-20 seconds (downloads libraries on first build)
- Calibration: 10-15 minutes (optional but recommended)

### Step 1: Hardware Assembly (2 minutes)
1. Connect Pixy2 pins to Arduino Uno:
   - CLK → Pin 13
   - MOSI → Pin 11
   - MISO → Pin 12
   - SS → Pin 10
   - GND → GND
   - 5V → 5V

### Step 2: Software Installation (1 minute)
1. Install **PlatformIO IDE** extension in VS Code
   - Open Extensions (Ctrl+Shift+X)
   - Search "PlatformIO IDE"
   - Click Install
   - Restart VS Code
2. Create `platformio.ini` in project root:
   ```ini
   [env:uno]
   platform = atmelavr
   board = uno
   framework = arduino
   lib_deps =
       Pixy2
   monitor_speed = 115200
   build_flags = -Wall
   ```
   **Important**: Only `Pixy2` in lib_deps (SPI is automatic)

### Step 3: Build & Upload Code (2 minutes)
1. Open terminal in VS Code (Ctrl+`)
2. Build the project:
   ```bash
   python -m platformio run
   ```
   Expected output: `========================= [SUCCESS] Took XX.XX seconds =========================`

3. Connect Arduino Uno via USB
4. Upload to Arduino:
   ```bash
   python -m platformio run --target upload
   ```
   Expected output: `Upload SUCCESSFUL`

   *Or use PlatformIO sidebar:*
   - Click **Build** button, wait for success
   - Click **Upload** button, wait for completion

### Step 4: Train Signatures
1. Download PixyMon from https://pixycam.com/downloads-pixy2/
2. Connect Pixy2 via USB
3. Open PixyMon
4. Point camera at red ball, click to train signature 1
5. Repeat for green (signature 2) and blue (signature 3)

### Step 5: Monitor Output
1. Click **Serial Monitor** in PlatformIO sidebar (or press Ctrl+Shift+A)
2. Verify baud rate is **115200** (shown at bottom)
3. Point red/green/blue balls at camera
4. See ball detections in real-time!

---

## Wiring Diagram

```
      Arduino Uno      Pixy2 Camera
    ┌─────────────┐    ┌──────────┐
    │   PIN 13    ├────┤ CLK      │
    │   PIN 11    ├────┤ MOSI     │
    │   PIN 12    ├────┤ MISO     │
    │   PIN 10    ├────┤ SS       │
    │   GND       ├────┤ GND      │
    │   5V        ├────┤ 5V       │
    └─────────────┘    └──────────┘
```

---

## Expected Output

```
========================================
Pixy2 Ball Detection System Initialized
========================================

Configuring Pixy2 camera...
Pixy2 ready for ball detection!

Expected colored balls: Red, Green, Blue (5cm diameter)
Adjust signatures in PixyMon if needed.

---
No balls detected.
---
Balls detected: 1
  Ball 1: [RED] Position(158,104) Size(45x48) Area=2160
---
```

---

## Common Issues

| Issue | Solution |
|-------|----------|
| Upload fails | Check board/port selection |
| No serial output | Verify USB cable, check port |
| "pixy.h not found" | Install Pixy2 library |
| No detection | Train signatures in PixyMon |
| Blurry output | Clean camera lens |

---

## Next Steps

1. ✅ Test with predefined colors (red, green, blue)
2. 📏 Calibrate distance measurement
3. 🎯 Train additional color signatures
4. 🔧 Fine-tune detection parameters in [config/pixy_config.h](config/pixy_config.h)
5. 🚀 Integrate with robotics/automation systems

See [README.md](README.md) for detailed documentation.
