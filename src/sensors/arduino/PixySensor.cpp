#include "PixySensor.h"

// change to uint8_t error codes + 30
uint8_t PixySensor::init()
{
    int8_t status = pixy.init();

    return status;
}

uint8_t PixySensor::updateBlocks()
{
     int8_t status = pixy.ccc.getBlocks(true);

    if (status < 0)
    {
        return uint8_t(status);
    }

    m_blockCount = pixy.ccc.numBlocks;
    if (m_blockCount > PIXY_MAX_BLOCKS)
    {
        m_blockCount = PIXY_MAX_BLOCKS;
    }

    // Convert Pixy2 blocks to our Block type
    for (uint8_t i = 0; i < m_blockCount; i++)
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

    Serial.print("Angle: ");
    Serial.println(m_blocks[0].angle);

    return status;
}

Types::PixyArrayResult PixySensor::getBlocks()
{
    return Types::PixyArrayResult{Codes::SUCCESS, m_blocks, m_blockCount};
}

Types::PixyResult PixySensor::getBlock(uint8_t index)
{
    for (uint8_t i = 0; i < m_blockCount; i++)
    {
        if (m_blocks[i].index == index)
        {
        
            return Types::PixyResult{Codes::SUCCESS, m_blocks[i]};
        }
    }

    return Types::PixyResult{Codes::PIXY_BLOCK_NOT_FOUND, Types::EMPTY_BLOCK};
};