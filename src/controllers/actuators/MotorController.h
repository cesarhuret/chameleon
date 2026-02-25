#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <stdint.h>

class MotorController
{

public:
    bool isRotating();
    bool isMoving();

    void rotate(int16_t targetX, int16_t targetY);
    void move(int16_t targetX, int16_t targetY);

    void stop();
};

#endif