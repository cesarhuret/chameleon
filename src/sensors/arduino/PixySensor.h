#ifndef PIXYSENSOR_H
#define PIXYSENSOR_H

#include <Arduino.h>
#include <stdint.h>

#include "Pixy2.h"
#include "sensors/interfaces/IPixySensor.h"

// Create namespace alias for Pixy2 library types
namespace Pixy {
    using Block = ::Block;
}

#define PIXY_MAX_BLOCKS 25

class PixySensor : public IPixySensor
{
private:
    Pixy2 pixy;
    Types::Block m_blocks[PIXY_MAX_BLOCKS];  // Internal buffer for converted blocks

public:
    int8_t init() override;
    const Types::Block *getBlocks(uint8_t &count) override;
    Types::Block getBlock(int index) override;
};

#endif
