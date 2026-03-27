#include "PixyController.h"
#include "constants/Pixy.h"
#include <Arduino.h>

uint8_t PixyController::init(IPixySensor *pixy, int8_t targetSignature, int16_t min_Height, int16_t max_Height, int16_t min_Width, int16_t max_Width, int16_t thresholdX, int16_t thresholdY)
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
        return PIXY_NOT_AVAILABLE;
    }

    uint8_t status = this->pixy->init();
    if (status != SUCCESS)
    {
        // failed to initialize Pixy, return error code
        return status;
    }
    return SUCCESS;
}

PixyResult PixyController::findTargetBall()
{
    const PixyArrayResult result = pixy->getBlocks();
    if (result.status < SUCCESS)
    {
        return PixyResult{result.status, EMPTY_BLOCK}; // Return an empty block on error
    }

    for (uint8_t i = 0; i < result.count; i++)
    {
        const DetectedBlock &block = result.blocks[i];

        Serial.print("Block Details - Signature: ");
        Serial.print(block.signature);
        Serial.print(", X: ");
        Serial.print(block.x);
        Serial.print(", Y: ");
        Serial.print(block.y);
        Serial.print(", Width: ");
        Serial.print(block.width);
        Serial.print(", Height: ");
        Serial.println(block.height);


        if (isBall(block))
        {
            // Return the first block that matches the criteria
            // We need to change this to return the block that is closest to the robot.
            this->currentTargetBallIndex = block.index; // Store the index of the current target ball
            return PixyResult{SUCCESS, block};
        }
    }
    return PixyResult{PIXY_TARGET_NOT_FOUND, EMPTY_BLOCK}; // Return an empty block if no target ball is found
}

bool PixyController::isBall(DetectedBlock block)
{
    if (block.signature != targetSignature)
    {
        return false;
    }
    // if (block.height < min_Height || block.height > max_Height || block.width < min_Width || block.width > max_Width)
    // {
    //     return false;
    // }
    
    return true;
}

PixyResult PixyController::getCurrentTargetBall() const
{
    return pixy->getBlock(currentTargetBallIndex);
}

uint8_t PixyController::resetCurrentTargetBall()
{
    this->currentTargetBallIndex = -1; // Reset to an invalid index
    return SUCCESS;               // Return success
}

bool PixyController::isBlockCentered(DetectedBlock block) const
{
    int16_t centerX = PIXY_CAM_WIDTH / 2;
    int16_t centerY = PIXY_CAM_HEIGHT / 2;

    auto dx = (int16_t)block.x - centerX;
    // auto dy = (int16_t)block.y - centerY;

    return (dx <= thresholdX && dx >= -thresholdX);
        //    && (dy <= thresholdY && dy >= -thresholdY);
}