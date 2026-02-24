#include "PixySensor.h"

int8_t PixySensor::init()
{
    int8_t status = pixy.init();
    return status;
}

const Types::Block* PixySensor::getBlocks(uint8_t &count)
{
    int8_t status = pixy.ccc.getBlocks(false);

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
        const Pixy::Block& pixyBlock = pixy.ccc.blocks[i];
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

Types::Block PixySensor::getBlock(int index)
{
    if (index < 0 || index >= pixy.ccc.numBlocks || index >= PIXY_MAX_BLOCKS)
    {
        return Types::Block();  // Return empty block
    }
    
    // Convert and return - use Pixy namespace for Pixy's Block
    const Pixy::Block& pixyBlock = pixy.ccc.blocks[index];
    Types::Block b;
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