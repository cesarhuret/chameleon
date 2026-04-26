#ifndef PIXYCONTROLLER_H
#define PIXYCONTROLLER_H

#include "sensors/interfaces/IPixySensor.h"
#include "types/Block.h"
#include "types/Codes.h"
#include "types/Config.h"
#include "config.h"

using namespace Types;
using namespace Codes;



class PixyController
{

private:
    IPixySensor* pixy;
    uint8_t currentBallSig;
    int16_t currentTargetBallIndex;
    int16_t currentTargetBaseIndex;
    bool hiddenBallInFront = false;

    bool _isTarget(DetectedBlock block, uint8_t targetSig);
    PixyResult _findTarget(uint8_t targetSig);

public:

    PixyController() 
        : pixy(nullptr),
        currentBallSig(0),
        currentTargetBallIndex(-1),
        currentTargetBaseIndex(-1)
    {}

    uint8_t init(IPixySensor* pixy);

    PixyResult findBall();

    PixyResult findBase();

    PixyResult getBall() const;

    PixyResult getBase() const;
    
    PixyArrayResult getBlocks() const;

    uint8_t updateBlocks();

    uint8_t updateHiddenBlocks();

    PixyArrayResult getHiddenBlocks();

    bool isThereAHiddenBlock() const;

    bool isCentered(DetectedBlock block, uint8_t threshold) const;

    uint8_t resetBall();

    uint8_t resetBase();

    uint8_t resetIsThereAHiddenBlock();

    uint8_t incrementBallSig();

};

#endif
