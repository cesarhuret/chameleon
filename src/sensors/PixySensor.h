#ifndef PIXYSENSOR_H
#define PIXYSENSOR_H

#include <Arduino.h>
#include <stdint.h>
#include "Pixy2.h"
#include "IPixySensor.h"

class PixySensor : public IPixySensor
{
private:
    Pixy2 pixy;

public:
    int8_t init() override;
    const Block *getBlocks(uint8_t &count) override;
    Block getBlock(int index) override;
};

#endif
