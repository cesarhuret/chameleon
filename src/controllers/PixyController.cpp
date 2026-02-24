#include "PixyController.h"
#include "constants/Pixy.h"

int8_t PixyController::init(IPixySensor *pixy, int8_t targetSignature, int16_t thresholdHeight, int16_t thresholdWidth, int16_t thresholdX, int16_t thresholdY)
{
    this->pixy = pixy;
    this->targetSignature = targetSignature;
    this->thresholdHeight = thresholdHeight;
    this->thresholdWidth = thresholdWidth;
    this->thresholdX = thresholdX;
    this->thresholdY = thresholdY;

    if (this->pixy == nullptr)
    {
        return -1;
    }

    int8_t status = this->pixy->init();
    if (status != 0)
    {
        // failed to initialize Pixy, return error code
        return status;
    }
    return 0;
}

Block PixyController::findTargetBall()
{
    uint8_t count = 0;
    const Block *blocks = pixy->getBlocks(count);

    for (uint8_t i = 0; i < count; i++)
    {
        const Block &block = blocks[i];
        if (isBall(block))
        {
            // Return the first block that matches the criteria
            // We need to change this to return the block that is closest to the robot.
            this->currentTargetBallIndex = block.index; // Store the index of the current target ball
            return block;
        }
    }
    return Block(); // Return an empty block if no target ball is found
}

bool PixyController::isBall(Block block)
{
    if (block.signature != targetSignature)
    {
        return false;
    }
    if (block.height < thresholdHeight || block.width < thresholdWidth)
    {
        return false;
    }
    if ((block.x - PIXY_CAM_WIDTH / 2) > thresholdX || (block.y - PIXY_CAM_HEIGHT / 2) > thresholdY)
    {
        return false;
    }
    return true;
}

Block PixyController::getCurrentTargetBall()
{
    return pixy->getBlock(currentTargetBallIndex);
}

int8_t PixyController::resetCurrentTargetBall()
{
    this->currentTargetBallIndex = -1; // Reset to an invalid index
    return 0; // Return success
}