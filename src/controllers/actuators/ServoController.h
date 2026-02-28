#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <stdint.h>

class ServoController
{

public:

    bool isOpen();
    bool isClosed();
    bool isMoving();

    void open();
    void close();
};


#endif