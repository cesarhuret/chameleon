#include "PixyController.h"
#include "constants/Pixy.h"

int8_t PixyController::init(IPixySensor *pixy, int8_t targetSignature, int16_t min_Height, int16_t max_Height, int16_t min_Width, int16_t max_Width, int16_t thresholdX, int16_t thresholdY)
{
    this->pixy = pixy;
    this->targetSignature = targetSignature;
    this->min_Height = min_Height;
    this->max_Height = max_Height;
    this->min_Width = min_Width;
    this->max_Width = max_Width;
    this->thresholdX = thresholdX;
    this->thresholdY = thresholdY;
    this->currentTargetBallIndex = -1; // Initialize to an invalid index

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

DetectedBlock PixyController::findTargetBall()
{
    uint8_t count = 0;
    const DetectedBlock *blocks = pixy->getBlocks(count);

    for (uint8_t i = 0; i < count; i++)
    {
        const DetectedBlock &block = blocks[i];
        if (isBall(block))
        {
            // Return the first block that matches the criteria
            // We need to change this to return the block that is closest to the robot.
            this->currentTargetBallIndex = block.index; // Store the index of the current target ball
            return block;
        }
    }
    return DetectedBlock(); // Return an empty block if no target ball is found
}

bool PixyController::isBall(DetectedBlock block)
{
    if (block.signature != targetSignature)
    {
        return false;
    }
    if (block.height < min_Height || block.height > max_Height || block.width < min_Width || block.width > max_Width)
    {
        return false;
    }
    
    return true;
}

DetectedBlock PixyController::getCurrentTargetBall() const
{
    return pixy->getBlock(currentTargetBallIndex);
}

int8_t PixyController::resetCurrentTargetBall()
{
    this->currentTargetBallIndex = -1; // Reset to an invalid index
    return 0;                          // Return success
}

bool PixyController::isBlockCentered(DetectedBlock block) const
{
    int16_t centerX = PIXY_CAM_WIDTH / 2;
    int16_t centerY = PIXY_CAM_HEIGHT / 2;

    auto dx = (int16_t)block.x - centerX;
    auto dy = (int16_t)block.y - centerY;

    return (dx <= thresholdX && dx >= -thresholdX) &&
           (dy <= thresholdY && dy >= -thresholdY);
}