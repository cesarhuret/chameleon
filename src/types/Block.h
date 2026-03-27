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

  struct PackedBlock
  {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint8_t signature;
    uint8_t age;
  };

  inline PackedBlock packBlock(const DetectedBlock &block)
  {
    return PackedBlock{block.x, block.y, block.width, block.height, block.signature, block.age};
  }

  static constexpr DetectedBlock EMPTY_BLOCK{};

  struct PixyResult
  {
    uint8_t status; // 0 = success, <0 = error
    Types::DetectedBlock block;
  };

  struct PixyArrayResult
  {
    uint8_t status;                     // 0 = success, <0 = error
    const Types::DetectedBlock *blocks; // pointer to internal array
    uint8_t count;                      // number of valid blocks
  };

  struct UltraSonicResult
  {
    uint8_t status; // 0 = success, <0 = error
    uint16_t distanceCm;
  };

  struct UltraSonicWithinResult
  {
    uint8_t status; // 0 = success, <0 = error
    bool isWithinThreshold;
  };

  struct LogEntry
  {
    uint32_t timestamp;
    uint8_t level;
    uint8_t type;        // e.g., 0 = numeric, 1 = NumericBlock, 2 = other struct
    uint8_t size;        // payload size
    uint8_t payload[10]; // MAX PAYLOAD SIZE
  };
}

#endif