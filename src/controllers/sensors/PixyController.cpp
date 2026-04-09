#include "PixyController.h"
#include "constants/Pixy.h"
#include <Arduino.h>

uint8_t PixyController::init(PixyConfig config)
{
    this->pixy = config.pixy;
    this->currentBallSig = config.ballParams.signatures[0]; // Start with the first signature for the ball
    this->ballParams = config.ballParams;
    this->baseParams = config.baseParams;
    this->thresholdX = config.thresholdX;
    this->thresholdY = config.thresholdY;
    this->currentTargetBallIndex = -1; // Initialize to an invalid index
    this->currentTargetBaseIndex = -1; // Initialize to an invalid index

    if (this->pixy == nullptr)
    {
        return PIXY_NOT_AVAILABLE;
    }

    delay(50);
    
    uint8_t status = this->pixy->init();
    if (status != SUCCESS)
    {
        // failed to initialize Pixy, return error code
        return status;
    }
    return SUCCESS;
}

PixyResult PixyController::_findTarget(TargetParams targetParams, uint8_t targetSig)
{
    const PixyArrayResult result = pixy->getBlocks();
    if (result.status < SUCCESS)
    {
        return PixyResult{result.status, EMPTY_BLOCK}; // Return an empty block on error
    }

    for (uint8_t i = 0; i < result.count; i++)
    {
        const DetectedBlock &block = result.blocks[i];

        if (_isTarget(block, ballParams, targetSig))
        {
            // Return the first block that matches the criteria
            // We need to change this to return the block that is closest to the robot.
            this->currentTargetBallIndex = block.index; // Store the index of the current target ball
            return PixyResult{SUCCESS, block};
        }
    }
    return PixyResult{PIXY_TARGET_NOT_FOUND, EMPTY_BLOCK}; // Return an empty block if no target ball is found
}

bool PixyController::_isTarget(DetectedBlock block, TargetParams targetParams, uint8_t targetSig)
{
    if (block.signature == 0)
    {
        return false;
    }

    if (block.signature == targetSig)
    {
        // If the signature matches, we can further check the size criteria if needed
        // For now, we will just check the signature
        return true;
    }

    // if (block.height < min_Height || block.height > max_Height || block.width < min_Width || block.width > max_Width)
    // {
    //     return false;
    // }

    return true;
}

PixyResult PixyController::findBall()
{
    return _findTarget(ballParams, currentBallSig);
}

PixyResult PixyController::findBase()
{
    if (this->currentTargetBaseIndex != -1)
    {
        // If we have a valid index for the current target base, return that block
        return getBase();
    }

    return _findTarget(baseParams, baseParams.signatures[0]); // Look for the first signature in the base parameters
}

PixyResult PixyController::getBall() const
{
    return pixy->getBlock(currentTargetBallIndex);
}

PixyResult PixyController::getBase() const
{
    return pixy->getBlock(currentTargetBaseIndex);
}

uint8_t PixyController::resetBall()
{
    this->currentTargetBallIndex = -1; // Reset to an invalid index
    return SUCCESS;                    // Return success
}

uint8_t PixyController::resetBase()
{
    this->currentTargetBaseIndex = -1; // Reset to an invalid index
    return SUCCESS;                    // Return success
}

uint8_t PixyController::incrementBallSig()
{
    // Increment the current ball signature to look for the next ball in the next search
    if (currentBallSig == ballParams.signatures[0])
    {
        currentBallSig = ballParams.signatures[1];
    }
    else if (currentBallSig == ballParams.signatures[1])
    {
        currentBallSig = ballParams.signatures[2];
    }
    else
    {
        currentBallSig = ballParams.signatures[0]; // Wrap around to the first signature
    }
    return SUCCESS; // Return success
}

bool PixyController::isCentered(DetectedBlock block) const
{
    int16_t centerX = PIXY_CAM_WIDTH / 2;
    int16_t centerY = PIXY_CAM_HEIGHT / 2;

    auto dx = (int16_t)block.x - centerX;
    // auto dy = (int16_t)block.y - centerY;

    return (dx <= thresholdX && dx >= -thresholdX);
    //    && (dy <= thresholdY && dy >= -thresholdY);
}