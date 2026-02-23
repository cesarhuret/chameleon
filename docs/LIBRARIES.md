# Arduino Libraries & Project Setup

## Quick Summary

This project uses **PlatformIO with VS Code** for compilation and upload.

### Minimal Setup (2 steps):
1. ✓ Install VS Code + PlatformIO IDE extension
2. ✓ Create `platformio.ini` with: `lib_deps = Pixy2` (see Step 2 below)
3. ✓ Run: `python -m platformio run` to build

**Current Build Status**: ✅ SUCCESS
- RAM: 33.5% used (686/2048 bytes)
- Flash: 16.5% used (5336/32256 bytes)

---

## Required Libraries

### 1. Pixy2 Library
- **Name**: Pixy2
- **Author**: Charmed Labs
- **Version**: Latest (compatible with v2.x)
- **Installation**: VS Code + PlatformIO (add to `platformio.ini`)
- **GitHub**: https://github.com/charmedlabs/pixy2
- **Status**: ✓ REQUIRED

**What it provides:**
- Pixy2 class for camera communication
- Block detection data structures
- SPI protocol implementation
- Color signature handling

### 2. SPI Library
- **Name**: SPI
- **Author**: Arduino
- **Status**: ✓ Automatic (included with Arduino framework)
- **Installation**: No separate installation needed

**What it provides:**
- Hardware SPI communication
- Master/Slave mode support
- Clock speed configuration

## Optional Libraries

### For Enhanced Functionality

1. **Adafruit GFX Library** (if using display)
   - Purpose: 2D graphics library
   - When needed: If adding LCD/OLED display

2. **WiFi101** (if using WiFi connectivity)
   - Purpose: WiFi communication
   - When needed: Remote ball detection monitoring

3. **SD Library** (if logging to SD card)
   - Purpose: SD card file operations
   - When needed: Recording detection data

## Installation Steps

### Step 1: Install VS Code & PlatformIO

1. Download and install **VS Code** from: https://code.visualstudio.com/
2. Install **PlatformIO IDE** extension:
   - Open VS Code Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Click Install
   - Restart VS Code

### Step 2: Configure platformio.ini

1. Ensure `platformio.ini` exists in project root with:
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

**Important Notes:**
- Do NOT include `SPI` in `lib_deps` - it's automatic with Arduino framework
- Keep configuration simple - avoid `monitor_dtr`, `monitor_rts`, or unknown options
- `lib_deps` should ONLY contain `Pixy2`

### Step 3: Build the Project

1. Open terminal in VS Code (Ctrl+`)
2. Run:
   ```bash
   python -m platformio run
   ```
   Or from PlatformIO sidebar: Click **Build** button

3. Expected successful output:
   ```
   ========================= [SUCCESS] Took XX.XX seconds =========================
   ```

### Step 4: Upload to Arduino

1. Connect Arduino Uno via USB
2. Run:
   ```bash
   python -m platformio run --target upload
   ```
   Or from PlatformIO sidebar: Click **Upload** button

### Manual Installation (Alternative)

If you prefer manual management:

1. Create `lib` folder in project root
2. Download from: https://github.com/charmedlabs/pixy2/archive/master.zip
3. Extract and move to: `lib/Pixy2/`
4. Run `platformio run` to compile

## Verification

### Check if Libraries are Installed

1. Open VS Code
2. Open [platformio.ini](../platformio.ini)
3. Verify it contains:
   ```ini
   lib_deps =
       Pixy2
       SPI
   ```
4. Check `.pio/libdeps/` folder (created after first build)

### Compile Test

1. Open [src/pixy_ball_detector.ino](../src/pixy_ball_detector.ino)
2. Build using one of these methods:
   - Click **Build** button in PlatformIO sidebar
   - Run in terminal: `python -m platformio run`

3. Expected success message:
   ```
   ========================= [SUCCESS] Took 14.65 seconds =========================
   RAM: [===       ] 33.5% (used 686 bytes from 2048 bytes)
   Flash: [==        ] 16.5% (used 5336 bytes from 32256 bytes)
   ```

4. Verify:
   - No error messages in output
   - `.pio/build/uno/firmware.elf` is created
   - RAM and Flash usage are within limits

## Troubleshooting

### "Pixy2.h: No such file or directory"

**Cause**: Library not installed or missing from registry download

**Solutions**:
1. Verify `platformio.ini` has `lib_deps = Pixy2` (note: NO `SPI`)
2. Delete `.pio/` folder: `rm -r .pio` or `rmdir /s .pio`
3. Rebuild: `python -m platformio run`
4. Check internet connection (libraries downloaded from PlatformIO registry)
5. Verify platformio.ini is in project root

### "SPI.h: No such file or directory"

**Cause**: Usually indicates framework issue

**Solutions**:
1. Verify `platformio.ini` has `framework = arduino`
2. Verify board is: `board = uno`
3. Do NOT add SPI to `lib_deps`
4. Delete `.pio/` and rebuild
5. Ensure `platform = atmelavr` is set

### Build Fails with Errors

**Solution**:
1. Open Terminal in VS Code (Ctrl+`)
2. Clean build: `python -m platformio clean`
3. Check configuration: `python -m platformio run --verbose`
4. Verify `.pio/libdeps/uno/pixy2/` exists

### TypeError or Invalid Configuration

**Cause**: Invalid platformio.ini options (e.g., `monitor_dtr = off`)

**Solution**:
1. Remove any invalid options from `platformio.ini`
2. Keep only: `platform`, `board`, `framework`, `lib_deps`, `monitor_speed`
3. Delete `.pio/` and rebuild
4. See the correct configuration in **Step 2** above

### Library Conflicts

If using multiple SPI devices:
1. Ensure only one SS (chip select) pin per device
2. Use different pins for different devices
3. Keep `lib_deps` minimal - only add `Pixy2`

## Version Compatibility

| Component | Version | Compatibility |
|-----------|---------|---|
| VS Code | 1.80+ | ✓ Recommended |
| PlatformIO | 6.0+ | ✓ Latest preferred |
| Pixy2 Library | 2.0+ | ✓ Recommended |
| Arduino Framework | atmelavr | ✓ Compatible |
| Arduino Uno | Any revision | ✓ Compatible |
| ATmega328P | Stock | ✓ No changes needed |

## Examples Included in Pixy2 Library

After successful build, example files are located in:
```
.pio/libdeps/uno/pixy2/examples/
```

Available examples:
- `hello_world.cpp` - Basic color block detection
- `line_tracking.cpp` - Line following example
- `video_blocks.cpp` - Bar-code detection

**Note**: The [pixy_ball_detector.ino](../src/pixy_ball_detector.ino) is an extended example with calibration features.

## Memory Constraints

### Actual Memory Usage (from successful build)

```
RAM:   [===       ]  33.5% (used 686 bytes from 2048 bytes)
Flash: [==        ]  16.5% (used 5336 bytes from 32256 bytes)
```

### Flash Memory (32KB total on Arduino Uno)

Layout:
```
Bootloader:        2 KB (fixed)
Arduino Framework: ~12 KB
Pixy2 Library:     ~8 KB
Sketch Code:       ~5.3 KB (current)
Available:         ~4.7 KB
```

**Note**: Current sketch uses only 16.5% of flash. Safe to add more features.

### RAM (2KB total on Arduino Uno)

Actual usage (686 bytes):
```
Global Variables:  ~100 bytes
Ball Array:        ~120 bytes (10 balls)
SPI & Libraries:   ~256 bytes
Stack (available): ~1500+ bytes
```

**Note**: Current sketch uses only 33.5% of RAM. Safe for additional features.

## Update Information

### Checking Library Version

1. Check installed libraries:
   ```bash
   python -m platformio lib list
   ```

2. View in Terminal output:
   ```
   Library Manager: pixy2@1.0.4 has been installed!
   ```

### Updating Libraries

1. Update all libraries:
   ```bash
   python -m platformio lib update
   ```

2. Update specific library:
   ```bash
   python -m platformio lib update Pixy2
   ```

3. Clean and rebuild after update:
   ```bash
   python -m platformio clean
   python -m platformio run
   ```

## Additional Resources

- **Pixy2 Docs**: https://docs.pixycam.com/wiki/doku.php
- **Arduino Libraries Guide**: https://www.arduino.cc/en/guide/libraries
- **GitHub Releases**: https://github.com/charmedlabs/pixy2/releases
