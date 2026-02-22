#ifndef PIXYSENSOR_H
#define PIXYSENSOR_H

#include "Pixy2.h"

class PixySensor
{
private:
    Pixy2 pixy;

public:
    int8_t init();
    std::vector<Block> getBlocks();
    Block getBlock(int index);
};

#endif
