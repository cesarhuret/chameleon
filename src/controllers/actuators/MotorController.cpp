#include "MotorController.h"
#include "config.h"

int8_t MotorController::init()
{
    pinMode(RIGHT_PWM_PIN, OUTPUT);
    pinMode(RIGHT_DIR_PIN, OUTPUT);
    pinMode(LEFT_PWM_PIN, OUTPUT);
    pinMode(LEFT_DIR_PIN, OUTPUT);

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

    if (_direction)
    { // right
        digitalWrite(RIGHT_DIR_PIN, LOW);
        digitalWrite(LEFT_DIR_PIN, HIGH);
        analogWrite(RIGHT_PWM_PIN, _speed);
        analogWrite(LEFT_PWM_PIN, _speed);
    }
    else
    { // left
        digitalWrite(RIGHT_DIR_PIN, HIGH);
        digitalWrite(LEFT_DIR_PIN, LOW);
        analogWrite(RIGHT_PWM_PIN, _speed);
        analogWrite(LEFT_PWM_PIN, _speed);
    }

    return;
}

void MotorController::move(bool _direction, uint8_t _speed)
{
    _isMoving = true;
    _isRotating = false;

    if (_direction)
    { // forward
        digitalWrite(RIGHT_DIR_PIN, HIGH);
        digitalWrite(LEFT_DIR_PIN, HIGH);
        analogWrite(RIGHT_PWM_PIN, _speed);
        analogWrite(LEFT_PWM_PIN, _speed);
    }
    else
    { // backward
        digitalWrite(RIGHT_DIR_PIN, LOW);
        digitalWrite(LEFT_DIR_PIN, LOW);
        analogWrite(RIGHT_PWM_PIN, _speed);
        analogWrite(LEFT_PWM_PIN, _speed);
    }

    return;
}

void MotorController::stop()
{
    analogWrite(RIGHT_PWM_PIN, 0);
    analogWrite(LEFT_PWM_PIN, 0);
    _isMoving = false;
    _isRotating = false;

    return;
}