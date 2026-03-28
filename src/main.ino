#include "controllers/main/Chameleon.h"
#include "SPI.h"

#include "sensors/arduino/PixySensor.h"
#include "sensors/arduino/UltraSonicSensor.h"
#include "controllers/actuators/ServoController.h"
#include "helpers/logging/BluetoothLogger.h"
#include "helpers/logging/SerialLogger.h"
#include "types/Codes.h"

constexpr unsigned long BAUD_RATE = 115200UL;

Chameleon chameleon;
SerialLogger logger(LogLevel::Debug, BAUD_RATE);
// BluetoothLogger logger(Serial, LogLevel::Debug, BAUD_RATE); // uses 0 and 1
PixySensor pixySensor;
UltraSonicSensor ultraSonicSensor(9, 8); // trig, echo
ServoController servoController;
MotorController motorController;

void setup()
{

    Serial.begin(BAUD_RATE);

    // logger.init();
    // logger.log(LogLevel::Info, 20000, 1);

    // servoController.init(12, 20); // Example pin and interval, adjust as needed
    // motorController.init(3, 4, 5, 6); // Example pins for left and right motors, adjust as needed

    uint8_t status = chameleon.init(&logger, &pixySensor, &ultraSonicSensor); // Pass both sensors to the Chameleon controller
    if (status != Codes::SUCCESS)
    {
        logger.log(LogLevel::Error, status, 0);
        halt(); // Halt on initialization failure
    }

    logger.log(LogLevel::Info, Codes::SETUP_SUCCESS, 0);
}

void loop()
{


    // // digitalWrite(4, HIGH);
    // // digitalWrite(6, HIGH);
    // // analogWrite(3, 190);
    // // analogWrite(5, 190);

    // motorController.move(true, 100); // Example parameters, adjust as needed

    // delay(500);
    
    // motorController.stop();
    
    // motorController.move(false, 100); // Example parameters, adjust as needed

    // delay(500);

    // // // analogWrite(3, 0);
    // // // analogWrite(5, 0);

    // motorController.stop();

    // // delay(2000);

    // servoController.open();
    // delay(2000);
    // servoController.close();
    // delay(2000);

    uint8_t status = chameleon.run();
    // logger.log(LogLevel::Info, status, 0);

    if (status != Codes::SUCCESS)
    {
        logger.log(LogLevel::Error, status, 0);
        halt(); // Halt on runtime error
    }
}

// Halt function to stop execution in case of any failure
void halt()
{
    while (true)
    {
        logger.log(LogLevel::Error, Codes::HALT, 0);
        delay(5000); // Log the error message every 5 seconds
    }
}
