# Technical Specifications

## Camera Specifications (Pixy2 v2.1)

### Optical Characteristics
- **Resolution**: 316 × 208 pixels
- **Frame Rate**: ~50 fps (when processing)
- **Lens Type**: Fixed focus wide-angle
- **Horizontal FOV**: 75°
- **Vertical FOV**: 49°
- **Focal Length**: ~2.0mm
- **Minimum Focus Distance**: 2cm

### Physical Specifications
- **Dimensions**: 43.2 × 54.5 × 26.5 mm
- **Weight**: ~18g
- **Power Consumption**: ~150mA @ 5V
- **Interface**: SPI (1MHz typical, 2MHz max)

### Color Detection
- **Signatures**: Up to 7 distinct color signatures
- **Color Accuracy**: Relative (hue-based, distance-independent)
- **RGB Processing**: Full RGB color space

---

## Arduino Uno Specifications

### Processor
- **MCU**: ATmega328P
- **Clock Speed**: 16 MHz
- **Flash Memory**: 32 KB (program storage)
- **SRAM**: 2 KB (working memory)
- **EEPROM**: 1 KB (persistent storage)

### Communication
- **USB**: Full-speed USB (via CH340 or CP2102)
- **Serial UART**: 115200 baud typical
- **SPI**: Hardware SPI (up to 8 MHz)

### I/O Pins
- **Digital I/O**: 14 pins
- **Analog Input**: 6 pins (A0-A5)
- **PWM Output**: 6 pins

---

## SPI Communication Protocol

### Connection Specifications
- **Mode**: SPI Mode 3 (CPOL=1, CPHA=1)
- **Clock Speed**: 1 MHz (standard), up to 2 MHz
- **Data Format**: 16-bit words
- **Byte Order**: Big-endian (MSB first)

### SPI Frame Structure
```
Frame: [SYNC_BYTE] [FRAME_TYPE] [DATA...] [CHECKSUM]
```

**Sync Byte**: 0xC1 (always signals start of frame)  
**Frame Types**:
- 0x00: Color blocks (signature data)
- 0x01: Line tracking (for line-following)
- 0x02: Video blocks (for bar-code detection)

---

## Detection Algorithm

### Block Detection Process
1. **Image Capture**: Camera captures 316×208 RGB image
2. **Color Extraction**: Extract signature colors from image
3. **Blob Detection**: Find connected regions of same signature
4. **Filtering**:
   - Remove blobs smaller than `MIN_BLOCK_AREA`
   - Remove blobs larger than `MAX_BLOCK_AREA`
   - Check aspect ratio (roughly circular)
5. **Centroid Calculation**: Compute center (X,Y) of each blob
6. **Transmission**: Send block data via SPI to Arduino

### Block Data Structure
```cpp
struct Block {
  uint16_t m_signature;  // Color signature ID (1-7)
  uint16_t m_x;          // Centroid X position (0-315)
  uint16_t m_y;          // Centroid Y position (0-207)
  uint16_t m_width;      // Block width in pixels
  uint16_t m_height;     // Block height in pixels
  int16_t  m_angle;      // Angle (0-359°) for oriented blocks
};
```

---

## Distance Estimation

### Physics-Based Approach
For a known-diameter object (5cm ball):

$$\text{Distance} = \frac{K}{\sqrt{\text{Area}}}$$

Where:
- $K$ = Calibration constant
- Area = pixel area (width × height)

### Calibration Process

**Empirical Method:**
1. Place ball at distance $d_1 = 30\text{ cm}$, record $A_1$
2. Place ball at distance $d_2 = 60\text{ cm}$, record $A_2$
3. Calculate: $K = \sqrt{A_1} \times d_1 = \sqrt{A_2} \times d_2$

**Example:**
- At 30cm: Area = 900 pixels → $\sqrt{900} = 30$
- At 60cm: Area = 225 pixels → $\sqrt{225} = 15$
- $K = 30 \times 30 = 900$ (check: $15 \times 60 = 900$ ✓)

---

## Detection Performance Metrics

### Temporal Response
- **Latency**: ~100ms (100ms polling interval)
- **Update Rate**: ~10 Hz (10 updates/second)
- **Max Detection Rate**: Limited by frame rate (~50 fps)

### Spatial Accuracy
- **Position Accuracy**: ±2-3 pixels (depending on distance)
- **Size Accuracy**: ±5% (after calibration)
- **Direction Accuracy**: ±5° (for oriented blocks)

### Robustness
- **Lighting**: Works best in 50-500 lux
- **Shadows**: May reduce accuracy
- **Reflections**: Can cause false positives
- **Color Saturation**: Higher saturation = better detection

---

## Power Budget

### Current Consumption
- **Arduino Uno**: ~50mA (idle), ~100mA (active)
- **Pixy2**: ~150mA @ 5V
- **Total**: ~200-250mA @ 5V

### Power Supply Requirements
- **Voltage**: 5V DC (±5%)
- **Current**: 250mA minimum (500mA recommended)
- **Recommended Supply**: USB power or dedicated 5V regulated supply

### Power Optimization
- Reduce SPI clock speed (trades speed for lower power)
- Increase polling interval (trades latency for lower power)
- Disable unused signatures (saves processing power)

---

## Memory Usage

### Flash Memory (32 KB total)
- **Arduino Bootloader**: 2 KB
- **Pixy2 Library**: ~8 KB
- **Sketch Code**: ~4 KB
- **Available**: ~18 KB free

### RAM Usage (2 KB total)
- **Global Variables**: ~100 bytes
- **Ball Array**: ~200 bytes (for 10 balls × 20 bytes each)
- **Pixy Communication Buffer**: ~512 bytes
- **Stack**: ~300 bytes
- **Available**: ~800 bytes free

**Optimization**: Keep polling interval ≥50ms to avoid overflow

---

## Calibration Constants

### Default Values
```
CALIBRATION_CONSTANT = 4700.0  // Adjust based on your setup
PIXEL_TO_CM_RATIO = 0.15       // Empirically determined
```

### Adjustment Guidelines

**For underestimated distances** (measured > actual):
- Decrease `CALIBRATION_CONSTANT`
- Example: Change from 4700 to 4500

**For overestimated distances** (measured < actual):
- Increase `CALIBRATION_CONSTANT`
- Example: Change from 4700 to 4900

**Incremental tuning**:
- Adjust by 5-10% at a time
- Retest and verify until accuracy improves

---

## Environmental Constraints

### Operating Conditions
- **Temperature**: 0-50°C (optimal: 20-30°C)
- **Humidity**: 10-90% RH (non-condensing)
- **Altitude**: Up to 2000m sea level equivalent

### Optical Considerations
- **Lighting**: 50-500 lux recommended (avoid direct sunlight)
- **Reflective Surfaces**: Avoid shiny/reflective balls
- **Background**: High contrast with ball colors improves detection
- **Lens Cleanliness**: Critical for image quality

---

## Serial Protocol

### Serial Configuration
- **Baud Rate**: 115200 bps
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None
- **Flow Control**: None

### Output Format
```
Balls detected: 2
  Ball 1: [RED] Position(158,104) Size(45x48) Area=2160
  Ball 2: [BLUE] Position(245,162) Size(38x40) Area=1520
---
```

### Debug Output (when enabled)
Raw block data for troubleshooting:
```
Raw Block: Sig=1, X=158, Y=104, W=45, H=48, Angle=0
```

---

## Limitations & Constraints

### Hardware Limitations
- **Memory**: Standard Uno has limited RAM for complex algorithms
- **Processing Power**: 16MHz not sufficient for real-time image processing
- **I/O Pins**: Limited number of pins for additional sensors

### Detection Limitations
- **Occlusion**: Cannot detect partially hidden balls
- **Overlap**: Multiple overlapping balls may be detected as one
- **Speed**: Cannot track fast-moving objects (>30cm/s)
- **Size Range**: Must be within trained size window

### Environmental Limitations
- **Low Light**: <50 lux causes detection failures
- **Bright Light**: Direct sunlight causes color shift
- **Motion Blur**: Fast camera movement degrades accuracy
- **Reflections**: Glossy surfaces create false positives

---

## Future Enhancements

1. **Dual Pixy2 Stereo Vision**: 3D position estimation
2. **Kalman Filtering**: Temporal smoothing for position
3. **Edge Computing**: On-board image processing
4. **Machine Learning**: Neural network-based detection
5. **Wireless Integration**: Bluetooth/WiFi transmission
