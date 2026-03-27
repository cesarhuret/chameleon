#include "PixySensor.h"

// change to uint8_t error codes + 30
uint8_t PixySensor::init()
{
    int8_t status = pixy.init();

    return status;
}

Types::PixyArrayResult PixySensor::getBlocks()
{
    int8_t status = pixy.ccc.getBlocks(true);

    Serial.print("getBlocks status: ");
    Serial.println(status);


    if (status < 0)
    {
        return Types::PixyArrayResult{uint8_t(status), nullptr, 0};
    }

    uint8_t count = pixy.ccc.numBlocks;
    if (count > PIXY_MAX_BLOCKS)
    {
        count = PIXY_MAX_BLOCKS;
    }

    // Convert Pixy2 blocks to our Block type
    for (uint8_t i = 0; i < count; i++)
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

    Serial.print("Number of blocks detected: ");
    Serial.println(count);

    return Types::PixyArrayResult{uint8_t(status), m_blocks, count};
}

Types::PixyResult PixySensor::getBlock(uint8_t index)
{
    int8_t status = pixy.ccc.getBlocks(true);

    if (status < 0)
    {
        return Types::PixyResult{uint8_t(status), Types::EMPTY_BLOCK};
    }

    uint8_t count = pixy.ccc.numBlocks;
    if (count > PIXY_MAX_BLOCKS)
    {
        count = PIXY_MAX_BLOCKS;
    }

    for (uint8_t i = 0; i < count; i++)
    {
        if (pixy.ccc.blocks[i].m_index == index)
        {
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
            return Types::PixyResult{Codes::SUCCESS, b};
        }
    }

    return Types::PixyResult{Codes::PIXY_BLOCK_NOT_FOUND, Types::EMPTY_BLOCK};
};