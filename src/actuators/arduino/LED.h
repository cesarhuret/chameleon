#ifndef LED_H
#define LED_H

#include "config.h"
#include <Arduino.h>
#include <Servo.h>

class LED
{
private:

public:

    void init()
    {
        pinMode(RED_PIN, OUTPUT);
        pinMode(GREEN_PIN, OUTPUT);
        pinMode(BLUE_PIN, OUTPUT);
    };

    void write(bool red, bool green, bool blue)
    {
        digitalWrite(RED_PIN, red ? HIGH : LOW);
        digitalWrite(GREEN_PIN, green ? HIGH : LOW);
        digitalWrite(BLUE_PIN, blue ? HIGH : LOW); 
    };

};

#endif