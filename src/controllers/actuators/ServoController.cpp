#include "ServoController.h"

int8_t ServoController::init()
{
    servo.init(SERVO_PIN);

    close();

    return Codes::SUCCESS;
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
}