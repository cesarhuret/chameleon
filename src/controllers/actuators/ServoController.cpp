#include "ServoController.h"

void ServoController::init(uint8_t pin, uint8_t interval)
{
    servo.init(pin);
    moveInterval = interval;
    servo.write(currentAngle);
}

bool ServoController::isOpen()
{
    return currentAngle == OPEN_ANGLE;
}

bool ServoController::isClosed()
{
    return currentAngle == CLOSED_ANGLE;
}

bool ServoController::isMoving()
{
    return moving;
}

void ServoController::open()
{
    if (currentAngle == OPEN_ANGLE)
    {
        moving = false;
        return;
    }

    servo.write(OPEN_ANGLE);
    currentAngle = OPEN_ANGLE;
    moving = false;

    // int targetAngle = 120; // Adjust angle as needed for open position

    // if (currentAngle != targetAngle)
    // {
    //     unsigned long now = millis();
    //     if (now - lastMoveTime >= moveInterval)
    //     {
    //         if (currentAngle < targetAngle)
    //             currentAngle++;
    //         else if (currentAngle > targetAngle)
    //             currentAngle--;
    //         moving = true;
    //         servo.write(currentAngle);
    //         lastMoveTime = now;
    //     }
    // }
    // else
    // {
    //     moving = false;
    // }
}

void ServoController::close()
{
    if (currentAngle == CLOSED_ANGLE)
    {
        moving = false;
        return;
    }

    servo.write(CLOSED_ANGLE);
    currentAngle = CLOSED_ANGLE;
    moving = false;

    // int targetAngle = 170; // Adjust angle as needed for closed position

    // if (currentAngle != targetAngle)
    // {
    //     unsigned long now = millis();
    //     if (now - lastMoveTime >= moveInterval)
    //     {
    //         if (currentAngle < targetAngle)
    //             currentAngle++;
    //         else if (currentAngle > targetAngle)
    //             currentAngle--;
    //         moving = true;
    //         servo.write(currentAngle);
    //         lastMoveTime = now;
    //     }
    // }
    // else
    // {
    //     moving = false;
    // }
}