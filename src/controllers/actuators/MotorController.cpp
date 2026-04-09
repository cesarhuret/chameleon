#include "MotorController.h"

int8_t MotorController::init(MotorConfig config)
{
    _pwmPinR = config.rightPwmPin;
    _dirPinR = config.rightDirPin;
    _pwmPinL = config.leftPwmPin;
    _dirPinL = config.leftDirPin;

    pinMode(_pwmPinR, OUTPUT);
    pinMode(_dirPinR, OUTPUT);
    pinMode(_pwmPinL, OUTPUT);
    pinMode(_dirPinL, OUTPUT);

    stop();
    return Codes::SUCCESS;
}

bool MotorController::isRotating()
{
    return _isRotating;
}

bool MotorController::isMoving()
{
    return _isMoving;
}

void MotorController::rotate(bool _direction, uint8_t _speed)
{

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

    analogWrite(_pwmPinR, 0);
    analogWrite(_pwmPinL, 0);
    _isMoving = false;
    _isRotating = false;

    return;
}