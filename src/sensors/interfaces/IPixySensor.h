#ifndef IPIXYSENSOR_H
#define IPIXYSENSOR_H

#include <stdint.h>

#include "types/Block.h"

class IPixySensor {
public:
    virtual ~IPixySensor() = default;

    virtual int8_t init() = 0;

    // Return pointer to internal blocks buffer and set count
    virtual const Types::DetectedBlock* getBlocks(uint8_t& count) = 0;

    virtual Types::DetectedBlock getBlock(uint8_t index) = 0;
};

#endif