# Pixy2 Camera Calibration Guide

This guide walks you through the complete process of calibrating your Pixy2 camera for accurate colored ball detection.

## Calibration Overview

Calibration involves three main steps:

1. **Color Signature Training** - Teach Pixy2 to recognize specific ball colors
2. **Distance Calibration** - Fine-tune distance estimation accuracy
3. **Detection Parameter Tuning** - Optimize size filtering for reliable detection

---

## Part 1: Color Signature Training

### Tools Needed
- PixyMon software (download from https://pixycam.com/downloads-pixy2/)
- USB cable (USB-A to Micro-USB)
- Colored balls (red, green, blue - 5cm diameter minimum)
- Computer with PixyMon installed

### Step-by-Step: Training Signatures

#### 1. Launch PixyMon
1. Connect Pixy2 to computer via USB cable
2. Open PixyMon application
3. You should see the live camera feed

#### 2. Train Red Ball Signature
1. Hold a **red ball** in front of the camera (15-20cm distance)
2. Make sure the ball is centered and clearly visible
3. In PixyMon menu, select **Pixy2 Menu → Program → Configure**
4. Click the **red ball** in the video feed once to start signature recording
5. Slowly cover the entire surface of the ball with your finger
   - Move your finger to show different parts of the ball's color
   - This teaches the system about color variations (shadows, highlights)
   - Take 5-10 seconds to cover the entire ball
6. Click the ball again to stop recording
7. Set signature number to **1** (for `SIG_RED_BALL`)
8. Click **Save**

#### 3. Train Green Ball Signature
1. Repeat the above process with a **green ball**
2. Set signature number to **2** (for `SIG_GREEN_BALL`)

#### 4. Train Blue Ball Signature
1. Repeat the above process with a **blue ball**
2. Set signature number to **3** (for `SIG_BLUE_BALL`)

#### 5. Test Signatures
1. Hold each ball in front of camera
2. You should see the ball highlighted in PixyMon
3. The signature number should appear in the feed
4. If colors look different than expected, retrain that signature

### Tips for Signature Training

✓ **Best Practices:**
- Use consistent lighting when training (replicate your use environment)
- Cover the full ball surface during training (15+ seconds per ball)
- Train at the expected detection distance (15-50cm)
- Avoid shadows on the ball while training

✗ **Common Mistakes:**
- Training too quickly (insufficient color data)
- Training only one spot on the ball
- Training in different lighting conditions
- Using washed-out or very dim ball colors

---

## Part 2: Distance Calibration

Distance calibration allows the system to estimate how far each ball is from the camera.

### Required Equipment
- 3 reference balls (preferably same size/color)
- Measuring tape (at least 1 meter)
- Stable surface for setup
- PlatformIO Serial Monitor (in VS Code)

### Calibration Procedure

#### 1. Setup

1. Place colored ball on a flat surface
2. Set up measurement tape perpendicular to camera view
3. Position camera so ball is centered

#### 2. Measure Reference Distances

Measure ball block area at three known distances:

| Distance | Measured Area | Notes |
|----------|---|---|
| 20 cm | _____ px² | Close distance |
| 50 cm | _____ px² | Medium distance |
| 100 cm | _____ px² | Far distance |

**How to measure:**
1. Build and upload code using PlatformIO
2. Click **Serial Monitor** in PlatformIO sidebar (Ctrl+Shift+A)
3. Place ball at 20cm distance
4. Read the "Area" value from serial output
5. Record the area value
6. Repeat at 50cm and 100cm

#### 3. Calculate Calibration Constant

Formula: $K = \sqrt{Area_{at\_known\_distance}} \times Distance_{in\_cm}$

**Example Calculation:**
```
Distance 1: 20cm, Area = 2500 px²
√2500 = 50
K₁ = 50 × 20 = 1000

Distance 2: 50cm, Area = 400 px²
√400 = 20
K₂ = 20 × 50 = 1000

Distance 3: 100cm, Area = 100 px²
√100 = 10
K₃ = 10 × 100 = 1000

Average K = (1000 + 1000 + 1000) / 3 = 1000
```

#### 4. Update Configuration

1. Open [config/pixy_config.h](../config/pixy_config.h)
2. Find line: `#define CALIBRATION_CONSTANT  4700.0`
3. Replace with your calculated K value:
   ```cpp
   #define CALIBRATION_CONSTANT  1000.0
   ```
4. Save file
5. Upload code to Arduino

#### 5. Validate Calibration

1. Click **Serial Monitor** in PlatformIO sidebar
2. Place ball at known distance (e.g., 50cm)
3. Check Serial Monitor output
4. Verify estimated distance matches actual distance
5. If off by >10%, adjust `CALIBRATION_CONSTANT`:
   - If estimated distance is too large: **decrease** constant
   - If estimated distance is too small: **increase** constant

### Calibration Formula

$$\text{EstimatedDistance} = \frac{\text{CALIBRATION_CONSTANT}}{\sqrt{\text{Area}}}$$

### Fine-Tuning

If initial estimate is off:

**Adjustment method:**
$$K_{new} = K_{old} \times \frac{\text{ActualDistance}}{\text{EstimatedDistance}}$$

**Example:**
- Actual distance: 50cm
- Estimated distance: 45cm
- Current K: 1000
- New K = 1000 × (50/45) = 1111.1

---

## Part 3: Detection Parameter Tuning

Fine-tune size filters for optimal detection.

### Step 1: Determine Actual Ball Pixel Size

At your expected operating distances, what are the ball sizes?

| Distance | Expected Width | Expected Height | Expected Area |
|----------|---|---|---|
| 20 cm (close) | _____ px | _____ px | _____ px² |
| 50 cm (medium) | _____ px | _____ px | _____ px² |
| 100 cm (far) | _____ px | _____ px | _____ px² |

**How to measure:**
1. Build and upload code using PlatformIO
2. Click **Serial Monitor** in PlatformIO sidebar (Ctrl+Shift+A)
3. Place ball at distance
4. Note the Width, Height, and Area values
5. Record them in the table above

### Step 2: Set Size Thresholds

In [config/pixy_config.h](../config/pixy_config.h), adjust:

```cpp
// Use the smallest and largest values from Step 1
#define BALL_MIN_WIDTH        20       // Smallest width observed
#define BALL_MAX_WIDTH        150      // Largest width observed

#define MIN_BLOCK_AREA        400      // Smallest area observed
#define MAX_BLOCK_AREA        22500    // Largest area observed
```

### Step 3: Test and Refine

1. Upload updated code
2. Place ball at various distances
3. Verify detection occurs at all distances
4. If no detection at some distances:
   - **At close range**: Increase `BALL_MAX_WIDTH` and `MAX_BLOCK_AREA`
   - **At far range**: Decrease `BALL_MIN_WIDTH` and `MIN_BLOCK_AREA`

### Step 4: Optimize Aspect Ratio (Optional)

Balls should be roughly circular (width ≈ height):

```cpp
#define MIN_ASPECT_RATIO      0.7      // Allow 30% distortion
#define MAX_ASPECT_RATIO      1.3      // Allow 30% distortion
```

Adjust if you see many false positives (elongated blobs).

---

## Complete Calibration Checklist

- [ ] **Signatures Trained**
  - [ ] Red ball trained (Signature 1)
  - [ ] Green ball trained (Signature 2)
  - [ ] Blue ball trained (Signature 3)
  - [ ] PixyMon shows correct colors

- [ ] **Distance Calibration**
  - [ ] Area measured at 3 distances
  - [ ] CALIBRATION_CONSTANT calculated
  - [ ] Config file updated
  - [ ] Distance estimates verified (within 10%)

- [ ] **Size Filtering**
  - [ ] Minimum width/height set
  - [ ] Maximum width/height set
  - [ ] Area thresholds configured
  - [ ] Detection works at all distances

- [ ] **Final Testing**
  - [ ] All 3 colored balls detected
  - [ ] Position tracking accurate (±2-3 pixels)
  - [ ] Distance estimation accurate (within 10%)
  - [ ] No false positives (background noise)

---

## Troubleshooting Calibration Issues

### Issue: Signature appears different colors in PixyMon

**Cause**: Inconsistent training

**Solution**:
1. Delete signature: Pixy2 Menu → Configure → Click signature X to clear
2. Retrain: Follow Step-by-Step training procedure
3. Train 10+ seconds, covering entire ball surface

### Issue: Ball detected at close range but not far range

**Cause**: Thresholds too strict

**Solution**:
1. Increase `MAX_BLOCK_AREA` in config
2. Decrease `BALL_MIN_WIDTH` and `BALL_MIN_HEIGHT`
3. Re-upload and test

### Issue: Distance estimate consistently wrong

**Cause**: Incorrect CALIBRATION_CONSTANT

**Solution**:
1. Recalibrate using 3-point method
2. If off by factor of 2: CALIBRATION_CONSTANT = original × 2 (or ÷ 2)
3. Test at middle distance (50cm) first

### Issue: Many false positives (non-balls detected)

**Cause**: Size thresholds too permissive

**Solution**:
1. Decrease `MAX_BLOCK_AREA`
2. Increase `BALL_MIN_WIDTH` and `BALL_MIN_HEIGHT`
3. Adjust aspect ratio filters

---

## Calibration Data Sheet

**Project Information:**
- Date: _______
- Location: _______
- Lighting Conditions: _______
- Ball Color/Type: _______

**Measured Values:**
```
Distance: 20cm  → Area: _______ px²
Distance: 50cm  → Area: _______ px²
Distance: 100cm → Area: _______ px²

Calculated CALIBRATION_CONSTANT: _______
```

**Size Range:**
```
Min Width: _______ px
Max Width: _______ px
Min Height: _______ px
Max Height: _______ px
Min Area: _______ px²
Max Area: _______ px²
```

**Validation:**
```
Test Distance: 50cm
Actual: 50cm, Estimated: _______cm, Error: _______%

Test Distance: 75cm
Actual: 75cm, Estimated: _______cm, Error: _______%
```

---

## Resources

- PixyMon Manual: https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:pixymon_running_pixymon
- Signature Training: https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:teach_pixy_an_object
- Distance Estimation White Paper: https://docs.pixycam.com/wiki/doku.php
