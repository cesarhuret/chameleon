#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <stdint.h>

class ServoController
{

public:

    bool isOpen();
    bool isClosed();

    void open();
    void close();
};


#endif