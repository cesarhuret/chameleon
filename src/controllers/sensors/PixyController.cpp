#include "PixyController.h"
#include "constants/Pixy.h"
#include <Arduino.h>

uint8_t PixyController::init(IPixySensor *pixy)
{
    this->pixy = pixy;
    this->currentBallSig = BALL_SIG_RED;
    this->currentTargetBallIndex = -1;
    this->currentTargetBaseIndex = -1;

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

PixyResult PixyController::_findTarget(uint8_t targetSig)
{
    const PixyArrayResult result = pixy->getBlocks();
    if (result.status < SUCCESS)
    {
        return PixyResult{result.status, EMPTY_BLOCK}; // Return an empty block on error
    }

    DetectedBlock bestBlock = EMPTY_BLOCK;

    for (uint8_t i = 0; i < result.count; i++)
    {
        const DetectedBlock &block = result.blocks[i];

        if (_isTarget(block, targetSig))
        {
            // Return the first block that matches the criteria
            // We need to change this to return the block that is closest to the robot.
            if (block.y > bestBlock.y)
            {
                bestBlock = block;
            }
        }
    }

    if (bestBlock.signature == 0)
    {
        return PixyResult{PIXY_TARGET_NOT_FOUND, EMPTY_BLOCK}; // Return an empty block if no valid target is found
    }
    else
    {
        return PixyResult{SUCCESS, bestBlock};
    }
}

bool PixyController::_isTarget(DetectedBlock block, uint8_t targetSig)
{
    if (block.signature == 0)
    {
        return false;
    }

    if (block.signature != targetSig)
    {
        return false;
    }

    // if (block.height < min_Height || block.height > max_Height || block.width < min_Width || block.width > max_Width)
    // {
    //     return false;
    // }

    return true;
}

PixyResult PixyController::findBall()
{
    PixyResult result = _findTarget(currentBallSig);

    if (result.status == SUCCESS)
    {
        this->currentTargetBallIndex = result.block.index; // Store the index of the current target ball
    }

    return result;
}

PixyResult PixyController::findBase()
{
    if (this->currentTargetBaseIndex != -1)
    {
        // If we have a valid index for the current target base, return that block
        return getBase();
    }

    PixyResult result = _findTarget(BASE_SIG_1); // Look for the first signature in the base parameters

    if (result.status == SUCCESS)
    {
        this->currentTargetBaseIndex = result.block.index; // Store the index of the current target base
    }

    return result;
}

uint8_t PixyController::updateBlocks()
{
    return pixy->updateBlocks();
}

uint8_t PixyController::updateHiddenBlocks()
{
    Types::PixyArrayResult result = pixy->getBlocks();
    if (result.status < SUCCESS)
    {
        return result.status; // Return the error status
    }

    // check which blocks from last frame are no longer visible
    for (uint8_t i = 0; i < m_prevCount; i++)
    {
        bool stillVisible = false;
        for (uint8_t j = 0; j < result.count; j++)
        {
            if (result.blocks[j].index == m_prevBlocks[i].index)
            {
                stillVisible = true;
                break;
            }
        }

        if (m_prevBlocks[i].y > 170 && m_prevBlocks[i].signature != currentBallSig)
        {
            m_lostBlocks[m_lostCount % 4] = m_prevBlocks[i];
            m_lostBlocks[m_lostCount % 4].age = 0; // repurpose index to store order of lost blocks
            m_lostCount++;
            if (m_lostCount > 4)
                m_lostCount = 4;
        }
    }

    int16_t centerX = PIXY_CAM_WIDTH / 2;
    uint16_t dxThreshold = THRESHOLD_X + 50; // Add some buffer to the threshold for hidden block detection

    for (uint8_t i = 0; i < m_lostCount; i++)
    {
        m_lostBlocks[i].age++;

        auto dx = (int16_t)m_lostBlocks[i].x - centerX;
        
        if (dx <= dxThreshold && dx >= -dxThreshold)
        {
            this->hiddenBallInFront = true;
            return true;
        }

        // Expire old entries — ball probably rolled away after 30 frames
        if (m_lostBlocks[i].age > 30)
        {
            for (uint8_t j = i; j < m_lostCount - 1; j++)
                m_lostBlocks[j] = m_lostBlocks[j + 1];
            m_lostCount--;
            i--;
        }
    }

    // copy current blocks into prev for next call
    m_prevCount = result.count > 4 ? 4 : result.count;
    for (uint8_t i = 0; i < result.count; i++)
    {
        m_prevBlocks[i] = result.blocks[i];
    }

    return SUCCESS;
}

PixyArrayResult PixyController::getHiddenBlocks()
{
    return Types::PixyArrayResult{SUCCESS, m_lostBlocks, m_lostCount};
}

PixyResult PixyController::getBall() const
{
    return pixy->getBlock(currentTargetBallIndex);
}

PixyResult PixyController::getBase() const
{
    return pixy->getBlock(currentTargetBaseIndex);
}

bool PixyController::isThereAHiddenBlock() const {
    return hiddenBallInFront;
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

uint8_t PixyController::resetIsThereAHiddenBlock()
{
    this->m_lostCount = 0;
    this->hiddenBallInFront = false;
    return SUCCESS; // Return success
}   

uint8_t PixyController::incrementBallSig()
{
    // Increment the current ball signature to look for the next ball in the next search
    if (currentBallSig == BALL_SIG_RED)
    {
        currentBallSig = BALL_SIG_GREEN;
    }
    else if (currentBallSig == BALL_SIG_GREEN)
    {
        currentBallSig = BALL_SIG_BLUE;
    }
    else
    {
        currentBallSig = BALL_SIG_RED;
    }
    return SUCCESS; // Return success
}

bool PixyController::isCentered(DetectedBlock block) const
{
    int16_t centerX = PIXY_CAM_WIDTH / 2;
    int16_t centerY = PIXY_CAM_HEIGHT / 2;

    auto dx = (int16_t)block.x - centerX;
    // auto dy = (int16_t)block.y - centerY;

    return (dx <= THRESHOLD_X && dx >= -THRESHOLD_X);
    //    && (dy <= THRESHOLD_Y && dy >= -THRESHOLD_Y);
}