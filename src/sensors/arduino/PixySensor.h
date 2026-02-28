#ifndef PIXYSENSOR_H
#define PIXYSENSOR_H

#include <Arduino.h>
#include <stdint.h>

#include "Pixy2.h"
#include "sensors/interfaces/IPixySensor.h"

// Create namespace alias for Pixy2 library types
namespace Pixy
{
    using Block = ::Block;
}

#define PIXY_MAX_BLOCKS 25

class PixySensor : public IPixySensor
{
private:
    Pixy2 pixy;
    Types::DetectedBlock m_blocks[PIXY_MAX_BLOCKS]; // Internal buffer for converted blocks

public:
    int8_t init() override;
    const Types::DetectedBlock *getBlocks(uint8_t &count) override;
    Types::DetectedBlock getBlock(uint8_t index) override;
};

#endif
