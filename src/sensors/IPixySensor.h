#ifndef IPIXYSENSOR_H
#define IPIXYSENSOR_H

#include <Arduino.h>
#include <stdint.h>

#include "Pixy2.h"

class IPixySensor {
public:
    virtual ~IPixySensor() = default;

    virtual int8_t init() = 0;

    // Return pointer to internal blocks buffer and set count
    virtual const Block* getBlocks(uint8_t& count) = 0;

    virtual Block getBlock(int index) = 0;
};

#endif