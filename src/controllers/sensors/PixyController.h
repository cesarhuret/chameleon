#ifndef PIXYCONTROLLER_H
#define PIXYCONTROLLER_H

#include "sensors/interfaces/IPixySensor.h"
#include "types/Block.h"

using namespace Types;

class PixyController
{

private:
    IPixySensor* pixy;
    int16_t currentTargetBallIndex;
    int8_t targetSignature;
    int16_t thresholdHeight;
    int16_t thresholdWidth;
    int16_t thresholdX;
    int16_t thresholdY;

    bool isBall(Block block);

public:
    int8_t init(IPixySensor* pixy, int8_t targetSignature, int16_t thresholdHeight, int16_t thresholdWidth, int16_t thresholdX, int16_t thresholdY);

    Block findTargetBall();

    Block getCurrentTargetBall() const;

    int8_t resetCurrentTargetBall();
};

#endif
