/*
  Pixy2 Camera Configuration Header
  
  This file contains all configuration parameters for Pixy2 ball detection
  Adjust these values based on your specific setup and testing
*/

#ifndef PIXY_CONFIG_H
#define PIXY_CONFIG_H

// ============================================
// CAMERA AND SENSOR PARAMETERS
// ============================================

// Pixy2 Camera Specifications
#define PIXY2_RESOLUTION_X    316      // Camera horizontal resolution (pixels)
#define PIXY2_RESOLUTION_Y    208      // Camera vertical resolution (pixels)
#define PIXY2_FOV_HORIZONTAL  75.0     // Field of view (degrees)
#define PIXY2_FOV_VERTICAL    49.0     // Field of view (degrees)

// ============================================
// BALL DETECTION PARAMETERS
// ============================================

// Physical ball specifications
#define BALL_DIAMETER_CM      5.0      // Ball diameter in centimeters
#define BALL_DIAMETER_MM      50       // Ball diameter in millimeters

// Detection distance range
#define MIN_DETECTION_DISTANCE_CM  15   // Minimum detection distance (cm)
#define MAX_DETECTION_DISTANCE_CM  100  // Maximum detection distance (cm)

// ============================================
// SIZE FILTER PARAMETERS
// ============================================

// Block (blob) size thresholds in pixels
// These are used to filter out noise and size variations
#define BALL_MIN_WIDTH        20       // Minimum block width (pixels)
#define BALL_MAX_WIDTH        150      // Maximum block width (pixels)
#define BALL_MIN_HEIGHT       20       // Minimum block height (pixels)
#define BALL_MAX_HEIGHT       150      // Maximum block height (pixels)

// Area-based filtering
#define MIN_BLOCK_AREA        400      // Minimum area (20*20, ~0.5cm at 1m)
#define MAX_BLOCK_AREA        22500    // Maximum area (150*150, ~5cm very close)

// Aspect ratio (width/height) tolerance
// Balls should be roughly circular, so ratio should be near 1.0
#define MIN_ASPECT_RATIO      0.7      // Minimum width/height ratio
#define MAX_ASPECT_RATIO      1.3      // Maximum width/height ratio

// ============================================
// COLOR SIGNATURE CONFIGURATION
// ============================================

// Color signatures (1-7 available in Pixy2)
// These must be trained using PixyMon software
#define SIG_RED_BALL          1        // Red ball signature
#define SIG_GREEN_BALL        2        // Green ball signature
#define SIG_BLUE_BALL         3        // Blue ball signature
#define SIG_YELLOW_BALL       4        // Yellow ball (optional)
#define SIG_ORANGE_BALL       5        // Orange ball (optional)

// Number of distinct color signatures to detect
#define NUM_SIGNATURES        3        // Red, Green, Blue

// ============================================
// PROCESSING PARAMETERS
// ============================================

// Maximum number of simultaneous ball detections
#define MAX_DETECTED_BALLS    10       // Store up to 10 ball detections

// Tracking parameters
#define POSITION_FILTER_SIZE  5        // Moving average filter size for position
#define ENABLE_POSITION_TRACKING true  // Enable temporal tracking

// ============================================
// CAMERA CALIBRATION PARAMETERS
// ============================================

// These need to be calibrated empirically
// Calibration process:
// 1. Place a 5cm ball at known distances (e.g., 20cm, 50cm, 100cm)
// 2. Record the pixel areas at each distance
// 3. Adjust CALIBRATION_CONSTANT to match your setup

#define CALIBRATION_CONSTANT  4700.0   // For distance = K / sqrt(area)
                                        // Adjust this value based on testing

// Pixel-to-cm conversion for ball width
#define PIXEL_TO_CM_RATIO     0.15     // pixels to cm conversion
                                        // Adjust based on camera FOV and distance

// ============================================
// COMMUNICATION PARAMETERS
// ============================================

#define SERIAL_BAUD_RATE      115200   // Serial communication speed
#define PIXY_SPI_SPEED        1000000  // SPI communication speed (1MHz)

// Output mode
#define VERBOSE_OUTPUT        true     // Enable detailed serial output
#define OUTPUT_INTERVAL_MS    100      // Output update interval (ms)

// ============================================
// PIXY2 SPI PINS (Arduino Uno)
// ============================================

#define PIXY_SPI_CLK          13       // SCK (Serial Clock)
#define PIXY_SPI_MOSI         11       // MOSI (Master Out Slave In)
#define PIXY_SPI_MISO         12       // MISO (Master In Slave Out)
#define PIXY_SPI_SS           10       // CS/SS (Chip Select)

// ============================================
// ADVANCED PARAMETERS
// ============================================

// Noise filtering
#define ENABLE_MORPHOLOGICAL_FILTER false // Enable opening/closing operations
#define MORPHOLOGICAL_KERNEL_SIZE   3    // Kernel size for filtering

// Performance tuning
#define SKIP_FRAMES           1        // Process every N frames (1 = all frames)
#define MAX_PROCESSING_TIME_MS 50      // Maximum time for processing (ms)

// Debug mode
#define DEBUG_MODE            true     // Enable debug output
#define DEBUG_BLOCK_DATA      false    // Print raw block data

#endif // PIXY_CONFIG_H
