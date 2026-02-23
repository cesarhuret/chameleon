# Project Structure

## Directory Layout

```
c:\Dev\UNSWT1\
├── .github/
│   └── copilot-instructions.md    # Copilot workspace instructions
├── include/
│   └── pixy_config.h               # Configuration header (PlatformIO standard)
├── src/
│   └── pixy_ball_detector.ino      # Main Arduino sketch
├── config/
│   └── pixy_config.h               # Configuration parameters (reference)
├── docs/
│   ├── QUICK_START.md              # Getting started guide
│   ├── TECHNICAL_SPECS.md          # Detailed technical specifications
│   ├── CALIBRATION.md              # Complete calibration guide
│   ├── LIBRARIES.md                # Setup & library installation guide
│   └── QUICK_START.md              # Quick start
├── platformio.ini                   # PlatformIO configuration
├── README.md                        # Complete project documentation
└── PROJECT_STRUCTURE.md             # This file
```

## File Descriptions

### Core Implementation

**[src/pixy_ball_detector.ino](src/pixy_ball_detector.ino)**
- Main Arduino sketch
- ~300 lines of code
- Implements ball detection algorithm
- Handles SPI communication with Pixy2
- Outputs detection data via Serial Monitor
- Can be directly uploaded to Arduino Uno

**[config/pixy_config.h](config/pixy_config.h)**
- Configuration header file (reference copy)
- ~200 lines of definitions
- All tunable parameters
- Color signature definitions
- Size filter thresholds
- Calibration constants
- Camera parameters

**[include/pixy_config.h](include/pixy_config.h)**
- Configuration header file (PlatformIO standard location)
- Same content as config/pixy_config.h
- Automatically included by sketch when needed
- Defines all tunable detection parameters

**[platformio.ini](platformio.ini)**
- PlatformIO build configuration
- Specifies Arduino Uno board (atmelavr)
- Lists library dependencies (Pixy2)
- Configures serial monitor (115200 baud)
- Sets build flags and compiler options

### Documentation

**[README.md](README.md)**
- Complete project documentation
- Hardware setup instructions
- Pin connection diagram
- Software installation steps
- Operational instructions
- Troubleshooting guide
- Performance specifications

**[docs/QUICK_START.md](docs/QUICK_START.md)**
- 5-minute quick start guide
- Step-by-step setup instructions
- Wiring diagram
- Expected output examples
- Common issues and quick fixes

**[docs/TECHNICAL_SPECS.md](docs/TECHNICAL_SPECS.md)**
- Detailed technical specifications
- Camera specifications (Pixy2 v2.1)
- Arduino Uno specifications
- SPI protocol details
- Detection algorithm explanation
- Distance estimation mathematics
- Performance metrics
- Power consumption analysis
- Environmental constraints
- Future enhancement ideas

**[docs/CALIBRATION.md](docs/CALIBRATION.md)**
- Complete calibration guide
- Color signature training procedure
- Distance calibration methodology
- Detection parameter tuning
- Troubleshooting calibration issues
- Calibration data sheet template
- Mathematical formulas

**[docs/LIBRARIES.md](docs/LIBRARIES.md)**
- Library installation instructions
- Required libraries (Pixy2, SPI)
- Optional libraries
- Installation methods (3 options)
- Verification procedures
- Version compatibility table
- Memory constraints
- Update information

**[.github/copilot-instructions.md](.github/copilot-instructions.md)**
- Copilot AI workspace instructions
- Project overview
- Hardware setup summary
- Key features list
- Software requirements
- Workflow summary
- Quick reference for configuration
- Expected output format

### Project Metadata

**[PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)**
- This file
- Directory layout
- File descriptions
- Feature checklist

---

## Quick Reference

### For First-Time Setup
1. Start with **[docs/QUICK_START.md](docs/QUICK_START.md)** (5 minutes)
2. Follow **[README.md](README.md)** for detailed setup
3. Use **[docs/CALIBRATION.md](docs/CALIBRATION.md)** for fine-tuning

### For Implementation Details
1. Review **[config/pixy_config.h](config/pixy_config.h)** for settings
2. Edit **[src/pixy_ball_detector.ino](src/pixy_ball_detector.ino)** for code
3. Consult **[docs/TECHNICAL_SPECS.md](docs/TECHNICAL_SPECS.md)** for reference

### For Library Setup
- Check **[docs/LIBRARIES.md](docs/LIBRARIES.md)** for PlatformIO installation
- Create `platformio.ini` with library dependencies

---

## Features Implemented

- [x] Real-time colored ball detection (red, green, blue)
- [x] Position tracking (X, Y coordinates)
- [x] Size measurement (width, height, area)
- [x] Distance estimation (0-200cm range)
- [x] SPI communication with Pixy2 camera
- [x] Serial output at 115200 baud
- [x] Configurable detection parameters
- [x] Block area filtering
- [x] Aspect ratio filtering (optional)
- [x] Up to 10 simultaneous ball tracking
- [x] Color name identification
- [x] Full documentation

---

## Hardware Compatibility

| Component | Version | Status |
|-----------|---------|--------|
| Arduino Uno | Any revision | ✓ Tested |
| Arduino Nano | With adjustments | ⚠️ Possible |
| Arduino Mega | With adjustments | ⚠️ Possible |
| Pixy2 Camera | v2.0+ (v2.1 recommended) | ✓ Tested |
| USB Cable | Standard USB-A to Micro-USB | ✓ Required |

---

## Software Compatibility

| Software | Version | Status | Purpose |
|----------|---------|--------|---------|
| VS Code | 1.80+ | ✓ Tested | Editor |
| PlatformIO | 6.0+ | ✓ Tested | Build/Upload |
| Pixy2 Library | 2.0+ | ✓ Tested | Camera control |
| PixyMon | Latest | ✓ Required | Calibration |

---

## Getting Started Checklist

### Initial Setup
- [ ] Download VS Code
- [ ] Install PlatformIO IDE extension
- [ ] Clone/download this project
- [ ] Create platformio.ini with library dependencies
- [ ] Review [docs/QUICK_START.md](docs/QUICK_START.md)
- [ ] Review [docs/QUICK_START.md](docs/QUICK_START.md)

### Hardware Assembly
- [ ] Connect Pixy2 to Arduino (see wiring diagram in README)
- [ ] Verify all connections
- [ ] Test USB connection

### Camera Calibration
- [ ] Download PixyMon
- [ ] Train red ball signature
- [ ] Train green ball signature
- [ ] Train blue ball signature

### Code Build & Upload
- [ ] Create `platformio.ini` in project root
- [ ] Open terminal (Ctrl+`)
- [ ] Build: `python -m platformio run`
- [ ] Verify "SUCCESS" in output
- [ ] Connect Arduino Uno via USB
- [ ] Upload: `python -m platformio run --target upload`
- [ ] Wait for "Upload SUCCESSFUL" message

### Testing & Validation
- [ ] Click **Serial Monitor** in PlatformIO sidebar (Ctrl+Shift+A)
- [ ] Verify baud rate is **115200**
- [ ] Place ball in front of camera
- [ ] Verify detection and output messages
- [ ] Test at various distances (20cm, 50cm, 100cm)

### Calibration & Tuning
- [ ] Follow [docs/CALIBRATION.md](docs/CALIBRATION.md)
- [ ] Verify distance estimates
- [ ] Optimize detection parameters
- [ ] Test edge cases

---

## Troubleshooting Quick Links

| Issue | Resource |
|-------|----------|
| Upload fails | [README.md - Troubleshooting](README.md#troubleshooting) |
| No balls detected | [README.md - Troubleshooting](README.md#troubleshooting) |
| Wrong library | [docs/LIBRARIES.md](docs/LIBRARIES.md#troubleshooting) |
| Distance inaccurate | [docs/CALIBRATION.md](docs/CALIBRATION.md#calibration-fine-tuning) |
| Hardware questions | [docs/TECHNICAL_SPECS.md](docs/TECHNICAL_SPECS.md) |

---

## Next Steps

After successful setup:

1. **Advanced Calibration**: Use [docs/CALIBRATION.md](docs/CALIBRATION.md) for precision
2. **Code Customization**: Modify [src/pixy_ball_detector.ino](src/pixy_ball_detector.ino) for your needs
3. **Additional Colors**: Train additional signatures (up to 7 total)
4. **Integration**: Connect output to robotics or automation systems
5. **Enhancement**: Refer to [docs/TECHNICAL_SPECS.md - Future Enhancements](docs/TECHNICAL_SPECS.md#future-enhancements)

---

## Support & Resources

- **Pixy2 Official Docs**: https://docs.pixycam.com/
- **Arduino Official Site**: https://www.arduino.cc/
- **GitHub Repository**: https://github.com/charmedlabs/pixy2
- **Forums**: https://pixycam.com/forums/forum/pixy2/

---

**Project Created**: February 2026  
**Version**: 1.0  
**Status**: Ready for use
