#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <stdint.h>
#include "actuators/arduino/Servo.h"

class ServoController
{

private:
    static constexpr uint8_t OPEN_ANGLE = 110;
    static constexpr uint8_t CLOSED_ANGLE = 170;

    ServoWrap servo;
    uint8_t moveInterval = 20; // Time in ms between angle updates for smooth movement
    unsigned long lastMoveTime = 0;
    bool moving = false;
    uint16_t currentAngle = OPEN_ANGLE; // Track current angle for smooth movement

public:
    void init(uint8_t pin, uint8_t interval);
    bool isOpen();
    bool isClosed();
    bool isMoving();

    void open();
    void close();
};


#endif