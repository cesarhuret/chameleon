#include "MotorController.h"

void MotorController::init(int8_t leftPwmPin, int8_t leftDirPin, int8_t rightPwmPin, int8_t rightDirPin)
{
    _pwmPinR = rightPwmPin;
    _dirPinR = rightDirPin;
    _pwmPinL = leftPwmPin;
    _dirPinL = leftDirPin;

    pinMode(_pwmPinR, OUTPUT);
    pinMode(_dirPinR, OUTPUT);
    pinMode(_pwmPinL, OUTPUT);
    pinMode(_dirPinL, OUTPUT);

    stop();
}

bool MotorController::isRotating()
{
    //! TODO: IMPLEMENT
    return _isRotating;
}

bool MotorController::isMoving()
{
    //! TODO: IMPLEMENT
    return _isMoving;
}

void MotorController::rotate(bool _direction, uint8_t _speed)
{
    //! TODO: IMPLEMENT

    _isRotating = true;
    _isMoving = false;

    if(_direction) { // left
        digitalWrite(_dirPinR, LOW);
        digitalWrite(_dirPinL, HIGH);
    } else { // right
        digitalWrite(_dirPinR, HIGH);
        digitalWrite(_dirPinL, LOW);
    }
    analogWrite(_pwmPinR, _speed);
    analogWrite(_pwmPinL, _speed);

    return;
}

void MotorController::move(bool _direction, uint8_t _speed)
{
    //! TODO: IMPLEMENT

    _isMoving = true;
    _isRotating = false;

    if(_direction) { // forward
        digitalWrite(_dirPinR, HIGH);
        digitalWrite(_dirPinL, HIGH);
    } else { // backward
        digitalWrite(_dirPinR, LOW);
        digitalWrite(_dirPinL, LOW);
    }

    analogWrite(_pwmPinR, _speed);
    analogWrite(_pwmPinL, _speed);

    return;
}

void MotorController::stop()
{
    //! TODO: IMPLEMENT

    analogWrite(_pwmPinR, 0);
    analogWrite(_pwmPinL, 0);
    _isMoving = false;
    _isRotating = false;

    return;
}