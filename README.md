# Arduino Uno + Pixy Camera 2.1: Colored Ball Detection System

## Project Overview

This project implements a real-time colored ball detection system using:
- **Microcontroller**: Arduino Uno
- **Camera**: Pixy Camera 2.1
- **Target Object**: Colored balls with 5cm diameter
- **Purpose**: Detect, locate, and track colored balls in the camera's field of view

### Key Features
✅ Real-time detection of colored balls (red, green, blue)  
✅ Position tracking (X, Y coordinates)  
✅ Size estimation and distance approximation  
✅ SPI communication with Pixy2 camera  
✅ Serial output for debugging and monitoring  
✅ Configurable detection parameters  

---

## Hardware Requirements

### Components
1. **Arduino Uno Microcontroller**
   - 5V operation
   - 2KB SRAM, 32KB Flash
   - SPI interface

2. **Pixy Camera 2.1**
   - 316×208 resolution
   - SPI interface
   - USB power input (optional)
   - 75° horizontal FOV

3. **Wiring Components**
   - Jumper wires (6x)
   - USB cable for programming
   - Optional: Power supply (if not using USB)

### Pin Connections

| Pixy2 Pin | Arduino Uno Pin | Function |
|-----------|-----------------|----------|
| SPI CLK   | 13              | Serial Clock (SCK) |
| SPI MOSI  | 11              | Master Out Slave In |
| SPI MISO  | 12              | Master In Slave Out |
| SPI SS    | 10              | Chip Select |
| GND       | GND             | Ground |
| 5V        | 5V              | Power (5V) |

**Diagram:**
```
Pixy2 Camera
    |
    +--- Pin CLK  -------> Arduino 13 (SCK)
    +--- Pin MOSI -------> Arduino 11 (MOSI)
    +--- Pin MISO -------> Arduino 12 (MISO)
    +--- Pin SS   -------> Arduino 10 (CS)
    +--- Pin GND  -------> Arduino GND
    +--- Pin 5V   -------> Arduino 5V
```

---

## Software Setup

### 1. Install VS Code & PlatformIO

1. Download VS Code from: https://code.visualstudio.com/
2. Install **PlatformIO IDE** extension:
   - Open Extensions (Ctrl+Shift+X)
   - Search "PlatformIO IDE"
   - Click Install
   - Restart VS Code

### 2. Configure PlatformIO

Create a `platformio.ini` file in the project root with:
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

**Important**: Only include `Pixy2` in lib_deps - SPI is automatic with the Arduino framework.

### 3. Build & Upload the Code

**Option A: Using Terminal (Recommended)**
1. Open terminal in VS Code (Ctrl+`)
2. Build the project:
   ```bash
   python -m platformio run
   ```
3. Connect Arduino Uno via USB
4. Upload to Arduino:
   ```bash
   python -m platformio run --target upload
   ```
5. Wait for "Upload SUCCESSFUL" message

**Option B: Using PlatformIO Sidebar**
1. Click **Build** button in PlatformIO sidebar
2. Wait for "SUCCESS" message
3. Connect Arduino Uno via USB
4. Click **Upload** button in PlatformIO sidebar
5. Wait for completion

---

## Camera Calibration

### Step 1: Train Color Signatures

**Using PixyMon Software:**
1. Download PixyMon from: https://pixycam.com/downloads-pixy2/
2. Connect Pixy2 to computer via USB
3. Open PixyMon
4. Place a **red ball** in front of camera
5. Go to **Pixy2 Menu → Program → Configure**
6. Click on ball to start recording
7. Cover the ball with your finger to teach the color
8. Set signature number to **1** (RED_BALL)
9. Repeat for **green ball** (signature 2) and **blue ball** (signature 3)

### Step 2: Optimize Detection Parameters

1. Open [config/pixy_config.h](config/pixy_config.h)
2. Adjust these parameters based on testing:
   - `BALL_MIN_WIDTH` / `BALL_MAX_WIDTH`
   - `MIN_BLOCK_AREA` / `MAX_BLOCK_AREA`
   - `CALIBRATION_CONSTANT` (for distance estimation)

### Step 3: Test and Validate
1. Click **Serial Monitor** in PlatformIO sidebar (or press Ctrl+Shift+A)
2. Verify baud rate is **115200** (shown at bottom)
3. Hold different colored balls at various distances
4. Observe output to verify detection

---

## Operational Instructions

### Starting the System
1. Connect Arduino Uno to computer via USB
2. Click **Serial Monitor** in PlatformIO sidebar (Ctrl+Shift+A)
3. Camera automatically initializes on startup
4. Look for "Pixy2 ready for ball detection!" message

### Detection Output
The system outputs real-time detection information:

```
Balls detected: 2
  Ball 1: [RED] Position(158,104) Size(45x48) Area=2160
  Ball 2: [GREEN] Position(245,85) Size(38x41) Area=1558
---
```

**Output Fields:**
- **Position(X,Y)**: Ball center coordinates in pixels
- **Size(W×H)**: Ball width and height in pixels
- **Area**: Total block area (width × height)
- **Signature**: Detected color (RED/GREEN/BLUE)

### Serial Output Fields Explained
- **X, Y**: Horizontal and vertical position (0-316, 0-208)
- **Width, Height**: Size in pixels (larger = closer, smaller = farther)
- **Area**: Used for distance estimation

---

## Advanced Features

### Distance Estimation
The code includes distance estimation based on block area:

```cpp
float estimateDistance(uint16_t area) {
  const float CALIBRATION_K = 100.0;
  return CALIBRATION_K / sqrt(area);
}
```

**To calibrate:**
1. Place ball at known distances (20cm, 50cm, 100cm)
2. Record observed areas
3. Adjust `CALIBRATION_K` until distances match

### Diameter Estimation
Estimate ball diameter from pixel width:

```cpp
float estimateDiameter(uint16_t pixel_width) {
  const float PIXEL_TO_CM = 0.15;
  return pixel_width * PIXEL_TO_CM;
}
```

---

## Troubleshooting

### No Balls Detected
- **Check**: Verify Pixy2 signatures are trained (use PixyMon)
- **Check**: Ensure adequate lighting (Pixy2 works best in 50-500 lux)
- **Check**: Confirm ball size is within configured range
- **Solution**: Retrain signatures or adjust `MIN_BLOCK_AREA` / `MAX_BLOCK_AREA`

### "pixy.h: No such file or directory"
- **Solution**: Verify `platformio.ini` has `lib_deps = Pixy2`
- **Solution**: Delete `.pio/` folder and rebuild: `python -m platformio run`
- **Verify**: File should download to `.pio/libdeps/uno/pixy2/`
- **Check**: Ensure internet connection is active (libraries downloaded from registry)

### Serial Monitor Shows Garbled Text
- **Solution**: Ensure baud rate is **115200** (shown in PlatformIO Serial Monitor)
- **Solution**: Check USB cable connection and verify Arduino Uno is detected
- **Solution**: Try: `python -m platformio device list` to verify COM port
- **Check**: Restart VS Code and try opening Serial Monitor again (Ctrl+Shift+A)

### Intermittent Detection
- **Cause**: Lighting conditions may be changing
- **Solution**: Improve and stabilize lighting (avoid shadows/reflections)
- **Solution**: Increase `CALIBRATION_CONSTANT` to be less sensitive to size variations

### Poor Distance Accuracy
- **Cause**: `CALIBRATION_CONSTANT` needs adjustment
- **Process**:
  1. Place ball at fixed distance (e.g., 50cm)
  2. Record displayed estimated distance
  3. Adjust constant: `K_new = K_old * (actual_distance / estimated_distance)`

---

## Configuration Reference

Key configuration parameters in [config/pixy_config.h](config/pixy_config.h):

| Parameter | Default | Purpose |
|-----------|---------|---------|
| `BALL_MIN_WIDTH` | 20 | Minimum ball width (pixels) |
| `BALL_MAX_WIDTH` | 150 | Maximum ball width (pixels) |
| `MIN_BLOCK_AREA` | 400 | Minimum detection area |
| `MAX_BLOCK_AREA` | 22500 | Maximum detection area |
| `SIG_RED_BALL` | 1 | Red signature ID |
| `SIG_GREEN_BALL` | 2 | Green signature ID |
| `SIG_BLUE_BALL` | 3 | Blue signature ID |

---

## Expected Performance

### Detection Capabilities
- **Object**: 5cm diameter colored balls
- **Colors**: Red, Green, Blue (trainable to other colors)
- **Range**: 15-100 cm (configurable)
- **Accuracy**: ±2-3 pixels position, ±10% size error
- **Update Rate**: ~10 fps (100ms intervals)

### Limitations
- Performance degrades in low light (<50 lux)
- Requires color training via PixyMon
- Limited to 7 distinct color signatures
- SPI speed limits update rate to ~10 fps

---

## Resources

### Official Documentation
- **Pixy2 Documentation**: https://docs.pixycam.com/wiki/doku.php
- **PixyMon**: https://pixycam.com/downloads-pixy2/
- **Arduino SPI Reference**: https://www.arduino.cc/en/reference/SPI

### Useful Links
- GitHub - Pixy2 Arduino Library: https://github.com/charmedlabs/pixy2
- Forum: https://pixycam.com/forums/forum/pixy2/

---

## License & Attribution

This project uses:
- VS Code (MIT)
- PlatformIO (Apache 2.0)
- Pixy2 Library by Charmed Labs (Apache 2.0)

---

## Notes

- Always power Pixy2 from regulated 5V supply
- Avoid strong IR light sources (interferes with image sensor)
- Proper lighting is critical for accurate color detection
- Keep camera lens clean for best performance

## Build Information

**Latest successful build:**
- RAM: 33.5% used (686 bytes from 2048 bytes)
- Flash: 16.5% used (5336 bytes from 32256 bytes)
- Build time: ~14.65 seconds
- Status: ✅ Compiles successfully with no errors

See [docs/LIBRARIES.md](docs/LIBRARIES.md) for detailed build output and memory usage information.
