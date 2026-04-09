#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <stdint.h>
#include <Arduino.h>
#include "types/Config.h"

class MotorController
{

private:
    uint8_t _speed;
    uint8_t _pwmPinR;
    uint8_t _dirPinR;
    uint8_t _pwmPinL;
    uint8_t _dirPinL;
    bool _isRotating = false;
    bool _isMoving = false;

public:
    int8_t init(MotorConfig config);
    bool isRotating();
    bool isMoving();

    void rotate(bool _direction, uint8_t _speed);
    void move(bool _direction, uint8_t _speed);

    void stop();
};

#endif