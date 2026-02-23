#ifndef MOCK_PIXYSENSOR_H
#define MOCK_PIXYSENSOR_H

#include "IPixySensor.h"
#include <vector>

class MockPixySensor : public IPixySensor
{
private:
    std::vector<Block> m_blocks;

public:
    // IPixySensor implementation
    int8_t init() override;
    std::vector<Block> getBlocks() override;
    Block getBlock(int index) override;

    // Test helper methods
    void setBlocks(const std::vector<Block>& blocks);
    void addBlock(const Block& block);
    void clearBlocks();
};

#endif
