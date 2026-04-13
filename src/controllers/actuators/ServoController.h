#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <stdint.h>
#include "actuators/arduino/Servo.h"
#include "types/Codes.h"
#include "config.h"

class ServoController
{

private:
    ServoWrap servo;
    unsigned long lastMoveTime = 0;
    bool moving = false;
    uint16_t currentAngle = OPEN_ANGLE; // Track current angle for smooth movement

public:
    int8_t init();
    bool isOpen();
    bool isClosed();
    bool isMoving();

    void open();
    void close();
};


#endif