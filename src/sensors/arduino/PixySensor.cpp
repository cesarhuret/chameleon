#include "PixySensor.h"

int8_t PixySensor::init()
{
    int8_t status = pixy.init();
    return status;
}

const Types::DetectedBlock *PixySensor::getBlocks(uint8_t &count)
{
    int8_t status = pixy.ccc.getBlocks(true);

    if (status < 0)
    {
        count = 0;
        return nullptr;
    }

    count = pixy.ccc.numBlocks;

    // Convert Pixy2 blocks to our Block type
    for (uint8_t i = 0; i < count && i < PIXY_MAX_BLOCKS; i++)
    {
        // Use Pixy2 namespace for Pixy2's Block struct
        const Pixy::Block &pixyBlock = pixy.ccc.blocks[i];
        m_blocks[i].x = pixyBlock.m_x;
        m_blocks[i].y = pixyBlock.m_y;
        m_blocks[i].width = pixyBlock.m_width;
        m_blocks[i].height = pixyBlock.m_height;
        m_blocks[i].signature = pixyBlock.m_signature;
        m_blocks[i].area = pixyBlock.m_width * pixyBlock.m_height;
        m_blocks[i].age = pixyBlock.m_age;
        m_blocks[i].index = pixyBlock.m_index;
        m_blocks[i].angle = pixyBlock.m_angle;
    }

    return m_blocks;
}

Types::DetectedBlock PixySensor::getBlock(uint8_t index)
{
    int8_t status = pixy.ccc.getBlocks(true);

    if (status < 0)
    {
        return Types::DetectedBlock();
    }

    if (index < 0 || index > 255)
    {
        return Types::DetectedBlock(); // Return empty block
    }

    for (uint8_t i = 0; i < pixy.ccc.numBlocks; i++)
    {
        if (pixy.ccc.blocks[i].m_index == index) {
            const Pixy::Block &pixyBlock = pixy.ccc.blocks[i];

            Types::DetectedBlock b;
            b.x = pixyBlock.m_x;
            b.y = pixyBlock.m_y;
            b.width = pixyBlock.m_width;
            b.height = pixyBlock.m_height;
            b.signature = pixyBlock.m_signature;
            b.area = pixyBlock.m_width * pixyBlock.m_height;
            b.age = pixyBlock.m_age;
            b.index = pixyBlock.m_index;
            b.angle = pixyBlock.m_angle;
            return b;
        }
    }

    return Types::DetectedBlock(); // Return empty block if not found
}