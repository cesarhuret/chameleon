<!-- Use this file to provide workspace-specific custom instructions to Copilot. -->

## Arduino Uno + Pixy Camera 2.1 Colored Ball Detection Project

This project implements a real-time colored ball detection system using Arduino Uno and Pixy Camera 2.1.

### Project Structure

- **src/pixy_ball_detector.ino** - Main Arduino sketch for ball detection
- **config/pixy_config.h** - Configuration parameters and constants
- **docs/QUICK_START.md** - 5-minute quick start guide
- **docs/TECHNICAL_SPECS.md** - Detailed technical specifications
- **README.md** - Complete project documentation

### Key Features

- Real-time detection of colored balls (red, green, blue)
- Position and size estimation
- Distance approximation based on pixel area
- SPI communication with Pixy2 camera
- Serial output for monitoring
- Fully configurable detection parameters

### Hardware Setup

**Pixy2 → Arduino Uno Pin Connections:**
- Pixy CLK → Arduino 13 (SCK)
- Pixy MOSI → Arduino 11 (MOSI)
- Pixy MISO → Arduino 12 (MISO)
- Pixy SS → Arduino 10 (CS)
- Pixy GND → Arduino GND
- Pixy 5V → Arduino 5V

### Software Requirements

1. VS Code (Code editor)
2. PlatformIO IDE extension (embedded development)
3. Pixy2 Arduino Library (installable via PlatformIO)
4. PixyMon software (for camera calibration)

### Workflow

1. **Setup**: Install VS Code and PlatformIO IDE extension
2. **Configuration**: Create platformio.ini with Pixy2 library dependency
3. **Hardware Assembly**: Connect Pixy2 to Arduino Uno pins
4. **Calibration**: Train color signatures in PixyMon
5. **Build & Upload**: Build and upload via PlatformIO
6. **Verification**: Monitor serial output at 115200 baud via PlatformIO
7. **Tuning**: Adjust parameters in pixy_config.h as needed

### Configuration

Main adjustable parameters in config/pixy_config.h:

- **BALL_MIN_WIDTH / BALL_MAX_WIDTH** - Size range in pixels
- **MIN_BLOCK_AREA / MAX_BLOCK_AREA** - Area thresholds
- **CALIBRATION_CONSTANT** - Distance estimation calibration
- **SIG_RED_BALL / SIG_GREEN_BALL / SIG_BLUE_BALL** - Color signatures (1-7)

### Expected Output

```
Pixy2 Ball Detection System Initialized
Pixy2 ready for ball detection!

Balls detected: 2
  Ball 1: [RED] Position(158,104) Size(45x48) Area=2160
  Ball 2: [GREEN] Position(245,85) Size(38x41) Area=1558
```

### Troubleshooting

- **No detection**: Verify signatures trained in PixyMon
- **Upload fails**: Check board/port selection in Tools menu
- **Garbled serial**: Ensure 115200 baud rate
- **Poor accuracy**: Calibrate CALIBRATION_CONSTANT empirically

### Resources

- Pixy2 Documentation: https://docs.pixycam.com/
- Arduino SPI: https://www.arduino.cc/en/reference/SPI
- Pixy2 GitHub: https://github.com/charmedlabs/pixy2

### Notes

- Optimal lighting: 50-500 lux
- Ball diameter: 5cm
- Detection range: 15-100cm
- Update rate: ~10 Hz
- Power requirement: ~250mA @ 5V
