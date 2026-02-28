#ifndef MOCK_PIXYSENSOR_H
#define MOCK_PIXYSENSOR_H

#include <vector>
#include "sensors/interfaces/IPixySensor.h"

class MockPixySensor : public IPixySensor
{
private:
    std::vector<Types::DetectedBlock> m_blocks;
    bool m_shouldFail;

public:
    MockPixySensor();

    // IPixySensor implementation
    int8_t init() override;
    const Types::DetectedBlock *getBlocks(uint8_t &count) override;
    Types::DetectedBlock getBlock(uint8_t index) override;

    // Test helper methods - set predefined scenarios
    void useSampleData_SingleRedBall();
    void useSampleData_MultipleBalls();
    void useSampleData_NoBalls();
    void useSampleData_BallAtEdge();
    void useSampleData_LargeBall();
    void useSampleData_SmallBall();

    // Manual data control
    void setBlocks(const std::vector<Types::DetectedBlock> &blocks);
    void addBlock(const Types::DetectedBlock &block);
    void clearBlocks();
    void setInitFails(bool shouldFail);
};

#endif
