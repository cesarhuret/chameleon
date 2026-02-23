#ifndef IPIXYSENSOR_H
#define IPIXYSENSOR_H


#include <cstdint>
#include <vector>

#include "Pixy2.h"

class IPixySensor {
public:
    virtual ~IPixySensor() = default;

    virtual int8_t init() = 0;
    virtual std::vector<Block> getBlocks() = 0;
    virtual Block getBlock(int index) = 0;
};

#endif