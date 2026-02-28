#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>

namespace Types
{
  struct DetectedBlock
  {
    uint16_t x;        // X position
    uint16_t y;        // Y position
    uint16_t width;    // Width in pixels
    uint16_t height;   // Height in pixels
    uint8_t signature; // Color signature
    uint16_t area;     // Estimated area
    uint8_t age;       // Number of frames since the block was first seen
    uint8_t index;     // Index of the block in the list of blocks in the current frame
    int16_t angle;
  };
}

#endif