/*
  Pixy2 Camera - Colored Ball Recognition
  Arduino Uno with Pixy Camera v2.1
  
  Purpose: Detects and tracks colored balls (5cm diameter) using Pixy2 camera
  Author: Auto-generated setup
  Date: February 2026
  
  Hardware:
  - Arduino Uno microcontroller
  - Pixy Camera 2.1
  - USB connection or serial connection
  
  Wire connections:
  - Pixy2 SPI CLK  -> Arduino Pin 13 (SCK)
  - Pixy2 SPI MOSI -> Arduino Pin 11 (MOSI)
  - Pixy2 SPI MISO -> Arduino Pin 12 (MISO)
  - Pixy2 SPI SS   -> Arduino Pin 10 (CS/SS)
  - Pixy2 GND      -> Arduino GND
  - Pixy2 5V       -> Arduino 5V
*/

#include <SPI.h>

#include "Pixy2.h"

// Create Pixy2 object
Pixy2 pixy;

// Constants for ball detection (can be overridden in config)
#ifndef BALL_MIN_WIDTH
#define BALL_MIN_WIDTH 20
#endif
#ifndef BALL_MAX_WIDTH
#define BALL_MAX_WIDTH 150
#endif
#ifndef BALL_MIN_HEIGHT
#define BALL_MIN_HEIGHT 20
#endif
#ifndef BALL_MAX_HEIGHT
#define BALL_MAX_HEIGHT 150
#endif
#ifndef MIN_BLOCK_AREA
#define MIN_BLOCK_AREA 400
#endif
#ifndef MAX_BLOCK_AREA
#define MAX_BLOCK_AREA 22500
#endif

// Signature IDs for colored balls (1-7 available in Pixy2)
// You'll need to train these in PixyMon
#ifndef SIG_RED_BALL
#define SIG_RED_BALL 1
#endif
#ifndef SIG_GREEN_BALL
#define SIG_GREEN_BALL 2
#endif
#ifndef SIG_BLUE_BALL
#define SIG_BLUE_BALL 3
#endif

// Structure to store detected ball information
struct Ball {
  uint16_t x;           // X position
  uint16_t y;           // Y position
  uint16_t width;       // Width in pixels
  uint16_t height;      // Height in pixels
  uint8_t signature;    // Color signature
  uint16_t area;        // Estimated area
};

Ball detected_balls[10];  // Array to store up to 10 detected balls
uint8_t ball_count = 0;   // Number of balls detected

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  delay(100);
  
  Serial.println("========================================");
  Serial.println("Pixy2 Ball Detection System Initialized");
  Serial.println("========================================");
  Serial.println();
  
  // Initialize SPI and Pixy2
  pixy.init();
  
  // Set camera parameters for optimal ball detection
  Serial.println("Configuring Pixy2 camera...");
  
  // Optional: Configure Pixy2 brightness if available
  // Higher brightness helps with darker colors
  
  Serial.println("Pixy2 ready for ball detection!");
  Serial.println();
  Serial.println("Expected colored balls: Red, Green, Blue (5cm diameter)");
  Serial.println("Adjust signatures in PixyMon if needed.");
  Serial.println();
}

void loop() {
  // Get latest blocks (color signatures) from Pixy2
  pixy.ccc.getBlocks();
  
  ball_count = 0;
  
  if (pixy.ccc.numBlocks > 0) {
    // Process detected blocks
    for (uint8_t i = 0; i < pixy.ccc.numBlocks; i++) {
      Block block = pixy.ccc.blocks[i];
      
      // Calculate block area
      uint16_t block_area = block.m_width * block.m_height;
      
      // Filter blocks by size to match 5cm ball at various distances
      if (block_area >= MIN_BLOCK_AREA && block_area <= MAX_BLOCK_AREA) {
        // Check if block signature matches our trained ball colors
        if (block.m_signature == SIG_RED_BALL || 
            block.m_signature == SIG_GREEN_BALL || 
            block.m_signature == SIG_BLUE_BALL) {
          
          // Valid ball detected
          if (ball_count < 10) {
            detected_balls[ball_count].x = block.m_x;
            detected_balls[ball_count].y = block.m_y;
            detected_balls[ball_count].width = block.m_width;
            detected_balls[ball_count].height = block.m_height;
            detected_balls[ball_count].signature = block.m_signature;
            detected_balls[ball_count].area = block_area;
            
            ball_count++;
          }
        }
      }
    }
  }
  
  // Display results
  if (ball_count > 0) {
    Serial.print("Balls detected: ");
    Serial.println(ball_count);
    
    for (uint8_t i = 0; i < ball_count; i++) {
      Serial.print("  Ball ");
      Serial.print(i + 1);
      Serial.print(": [");
      Serial.print(getColorName(detected_balls[i].signature));
      Serial.print("] Position(");
      Serial.print(detected_balls[i].x);
      Serial.print(",");
      Serial.print(detected_balls[i].y);
      Serial.print(") Size(");
      Serial.print(detected_balls[i].width);
      Serial.print("x");
      Serial.print(detected_balls[i].height);
      Serial.print(") Area=");
      Serial.println(detected_balls[i].area);
    }
  } else {
    Serial.println("No balls detected.");
  }
  
  Serial.println("---");
  
  // Delay before next scan (adjust as needed)
  delay(100);
}

// Helper function to get color name from signature
String getColorName(uint8_t signature) {
  switch (signature) {
    case SIG_RED_BALL:
      return "RED";
    case SIG_GREEN_BALL:
      return "GREEN";
    case SIG_BLUE_BALL:
      return "BLUE";
    default:
      return "UNKNOWN";
  }
}

// Optional: Function to get distance estimate based on block area
// This is a rough approximation for a 5cm diameter ball
float estimateDistance(uint16_t area) {
  // Calibration needed: adjust constants based on your camera
  // Distance is inversely proportional to area
  // Formula: distance = K / sqrt(area), where K is calibration constant
  const float CALIBRATION_K = 100.0; // Adjust this value empirically
  
  if (area == 0) return 0.0;
  return CALIBRATION_K / sqrt(area);
}

// Optional: Function to get ball diameter in cm
// Requires camera calibration
float estimateDiameter(uint16_t pixel_width) {
  // Calibration needed: adjust based on camera specifications
  // Pixy2 has 316x208 resolution
  // FOV and focal length determine pixel-to-cm conversion
  const float PIXEL_TO_CM = 0.15; // Adjust empirically
  
  return pixel_width * PIXEL_TO_CM;
}
