#ifndef IPIXYSENSOR_H
#define IPIXYSENSOR_H

#include <stdint.h>

#include "types/Block.h"
#include "types/Codes.h"

class IPixySensor {
public:
    virtual ~IPixySensor() = default;

    virtual uint8_t init() = 0;

    // Return pointer to internal blocks buffer and set count
    virtual Types::PixyArrayResult getBlocks() = 0;

    virtual Types::PixyResult getBlock(uint8_t index) = 0;
};

#endif