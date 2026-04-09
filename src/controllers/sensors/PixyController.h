#ifndef PIXYCONTROLLER_H
#define PIXYCONTROLLER_H

#include "sensors/interfaces/IPixySensor.h"
#include "types/Block.h"
#include "types/Codes.h"
#include "types/Config.h"

using namespace Types;
using namespace Codes;



class PixyController
{

private:
    IPixySensor* pixy;
    uint8_t currentBallSig;
    int16_t currentTargetBallIndex;
    int16_t currentTargetBaseIndex;
    TargetParams ballParams;
    TargetParams baseParams;
    int16_t thresholdX;
    int16_t thresholdY;

    bool _isTarget(DetectedBlock block, TargetParams targetParams, uint8_t targetSig);
    PixyResult _findTarget(TargetParams targetParams, uint8_t targetSig);

public:

    PixyController() 
        : pixy(nullptr),
        currentBallSig(0),
        currentTargetBallIndex(-1),
        currentTargetBaseIndex(-1),
        thresholdX(0),
        thresholdY(0)
    {}

    uint8_t init(PixyConfig config);

    PixyResult findBall();

    PixyResult findBase();

    PixyResult getBall() const;

    PixyResult getBase() const;

    bool isCentered(DetectedBlock block) const;

    uint8_t resetBall();

    uint8_t resetBase();

    uint8_t incrementBallSig();
};

#endif
