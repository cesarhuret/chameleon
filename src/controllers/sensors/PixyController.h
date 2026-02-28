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
    int16_t min_Height;
    int16_t max_Height;
    int16_t min_Width;
    int16_t max_Width;
    int16_t thresholdX;
    int16_t thresholdY;

    bool isBall(DetectedBlock block);

public:
    int8_t init(IPixySensor* pixy, int8_t targetSignature, int16_t min_Height, int16_t max_Height, int16_t min_Width, int16_t max_Width, int16_t thresholdX, int16_t thresholdY);

    DetectedBlock findTargetBall();

    DetectedBlock getCurrentTargetBall() const;

    bool isBlockCentered(DetectedBlock block) const;

    int8_t resetCurrentTargetBall();
};

#endif
