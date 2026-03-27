#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include <Servo.h>


class ServoWrap
{
private:
    Servo servo;
    uint8_t pin;

public:

    void init(uint8_t pin)
    {
        this->pin = pin;
        servo.attach(pin);
    };

    void write(int angle)
    {
        servo.write(angle);
    };

    void detach()
    {
        servo.detach();
    };
};

#endif