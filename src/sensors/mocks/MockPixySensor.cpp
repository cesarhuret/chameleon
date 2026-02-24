#include "MockPixySensor.h"

MockPixySensor::MockPixySensor() : m_shouldFail(false)
{
}

int8_t MockPixySensor::init()
{
    if (m_shouldFail)
        return -1;  // Simulate init failure
    return 0;
}

const Types::Block *MockPixySensor::getBlocks(uint8_t &count)
{
    count = m_blocks.size();
    return m_blocks.empty() ? nullptr : m_blocks.data();
}

Types::Block MockPixySensor::getBlock(int index)
{
    if (index < 0 || index >= static_cast<int>(m_blocks.size()))
    {
        return Types::Block{};  // Return empty block
    }
    return m_blocks[index];
}

// Sample data scenarios
void MockPixySensor::useSampleData_SingleRedBall()
{
    clearBlocks();
    addBlock({
        .x = 160,
        .y = 120,
        .width = 45,
        .height = 48,
        .signature = 1,  // Red
        .area = 2160,
        .age = 5,
        .index = 0,
        .angle = 0
    });
}

void MockPixySensor::useSampleData_MultipleBalls()
{
    clearBlocks();
    // Red ball
    addBlock({
        .x = 100,
        .y = 80,
        .width = 40,
        .height = 42,
        .signature = 1,  // Red
        .area = 1680,
        .age = 8,
        .index = 0,
        .angle = 0
    });
    // Green ball
    addBlock({
        .x = 200,
        .y = 100,
        .width = 35,
        .height = 38,
        .signature = 2,  // Green
        .area = 1330,
        .age = 6,
        .index = 1,
        .angle = 0
    });
    // Blue ball
    addBlock({
        .x = 150,
        .y = 150,
        .width = 50,
        .height = 52,
        .signature = 3,  // Blue
        .area = 2600,
        .age = 4,
        .index = 2,
        .angle = 0
    });
}

void MockPixySensor::useSampleData_NoBalls()
{
    clearBlocks();
}

void MockPixySensor::useSampleData_BallAtEdge()
{
    clearBlocks();
    addBlock({
        .x = 310,  // Near right edge (Pixy2 is 320x200)
        .y = 195,  // Near bottom edge
        .width = 30,
        .height = 32,
        .signature = 1,  // Red
        .area = 960,
        .age = 3,
        .index = 0,
        .angle = 0
    });
}

void MockPixySensor::useSampleData_LargeBall()
{
    clearBlocks();
    addBlock({
        .x = 160,
        .y = 100,
        .width = 80,
        .height = 85,
        .signature = 2,  // Green
        .area = 6800,
        .age = 10,
        .index = 0,
        .angle = 0
    });
}

void MockPixySensor::useSampleData_SmallBall()
{
    clearBlocks();
    addBlock({
        .x = 160,
        .y = 100,
        .width = 12,
        .height = 14,
        .signature = 3,  // Blue
        .area = 168,
        .age = 2,
        .index = 0,
        .angle = 0
    });
}

void MockPixySensor::setBlocks(const std::vector<Types::Block>& blocks)
{
    m_blocks = blocks;
}

void MockPixySensor::addBlock(const Types::Block& block)
{
    m_blocks.push_back(block);
}

void MockPixySensor::clearBlocks()
{
    m_blocks.clear();
}

void MockPixySensor::setInitFails(bool shouldFail)
{
    m_shouldFail = shouldFail;
}
