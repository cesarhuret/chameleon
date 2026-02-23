#include "MockPixySensor.h"
#include <stdexcept>

int8_t MockPixySensor::init()
{
    return 0;  // Return success
}

std::vector<Block> MockPixySensor::getBlocks()
{
    return m_blocks;
}

Block MockPixySensor::getBlock(int index)
{
    if (index < 0 || index >= static_cast<int>(m_blocks.size()))
    {
        throw std::out_of_range("Block index out of range");
    }
    return m_blocks[index];
}

void MockPixySensor::setBlocks(const std::vector<Block>& blocks)
{
    m_blocks = blocks;
}

void MockPixySensor::addBlock(const Block& block)
{
    m_blocks.push_back(block);
}

void MockPixySensor::clearBlocks()
{
    m_blocks.clear();
}
