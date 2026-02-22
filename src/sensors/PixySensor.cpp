#include "PixySensor.h"
#include "Pixy2.h"

int8_t PixySensor::init()
{
    int8_t status = pixy.init();
    return status;
}

std::vector<Block> PixySensor::getBlocks()
{
    int8_t status = pixy.ccc.getBlocks();

    if (status < 0)
    {
        return std::vector<Block>();
    }

    std::vector<Block> blocks;
    for (int i = 0; i < pixy.ccc.numBlocks; i++)
    {
        blocks.push_back(pixy.ccc.blocks[i]);
    }
    return blocks;
}

Block PixySensor::getBlock(int index)
{
    if (index < 0 || index >= pixy.ccc.numBlocks)
    {
        return Block(); // Return an empty block if index is invalid
    }
    return pixy.ccc.blocks[index];
}