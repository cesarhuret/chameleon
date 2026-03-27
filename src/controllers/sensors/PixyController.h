#ifndef PIXYCONTROLLER_H
#define PIXYCONTROLLER_H

#include "sensors/interfaces/IPixySensor.h"
#include "types/Block.h"
#include "types/Codes.h"

using namespace Types;
using namespace Codes;


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
    uint8_t init(IPixySensor* pixy, int8_t targetSignature, int16_t min_Height, int16_t max_Height, int16_t min_Width, int16_t max_Width, int16_t thresholdX, int16_t thresholdY);

    PixyResult findTargetBall();

    PixyResult getCurrentTargetBall() const;

    bool isBlockCentered(DetectedBlock block) const;

    uint8_t resetCurrentTargetBall();
};

#endif
