#include "PixySensor.h"

int8_t PixySensor::init()
{
    int8_t status = pixy.init();
    return status;
}

const Block *PixySensor::getBlocks(uint8_t &count)
{
    int8_t status = pixy.ccc.getBlocks();

    if (status < 0)
    {
        count = 0;
        return nullptr;
    }

    count = pixy.ccc.numBlocks;
    return pixy.ccc.blocks;
}

Block PixySensor::getBlock(int index)
{
    if (index < 0 || index >= pixy.ccc.numBlocks)
    {
        return Block(); // Return an empty block if index is invalid
    }
    return pixy.ccc.blocks[index];
}