#include <Arduino.h>

#include "Chameleon.h"
#include "constants/Pixy.h"
#include "types/Config.h"

Types::DetectedBlock lastTarget = Types::EMPTY_BLOCK;

uint8_t Chameleon::init(ILogger *logger, RuntimeConfig config)
{
    this->logger = logger;
    this->currentState = State::SEARCHING_FOR_BALL;

    // Initialize the PixyController with appropriate parameters
    int8_t status = pixyController.init(config.pixy);
    if (status != Codes::SUCCESS)
    {
        return status;
    }

    logger->log(LogLevel::Info, Codes::PIXY_INITIALIZATION_OK, 0);

    status = bottomUltrasoundController.init(config.bottomUltraSonic, BOTTOM_ULTRASONIC_TRIG_PIN, BOTTOM_ULTRASONIC_ECHO_PIN);
    if (status != Codes::SUCCESS)
    {
        return status;
    }

    logger->log(LogLevel::Info, Codes::ULTRASONIC_INITIALIZATION_OK, 0);

    status = servoController.init();
    if (status != Codes::SUCCESS)
    {
        return status;
    }

    logger->log(LogLevel::Info, Codes::SERVO_INITIALIZATION_OK, 0);

    status = motorController.init();
    if (status != Codes::SUCCESS)
    {
        return status;
    }

    logger->log(LogLevel::Info, Codes::MOTOR_INITIALIZATION_OK, 0);

    delay(100); // Small delay to ensure all components are initialized before moving the servo

    servoController.open();

    return Codes::SUCCESS; // Return success if all initializations are successful
}

uint8_t Chameleon::run()
{
    // just refresh whatever blocks the pixycam can see
    pixyController.updateBlocks();

    PixyResult result = pixyController.findBall();
    Types::DetectedBlock target = result.block;

    if(target.signature != 0) {
        lastTarget = target;
    }

    // If the target is centered, transition to moving towards the ball
    if (pixyController.isCentered(target))
    {
        motorController.stop();

        motorController.move(true, 70); // Move forward at speed 100
    }

    if (lastTarget.y > 170){
        motorController.move(true, 100); // Move forward at speed 100
    }
    else if (target.signature == 0)
    {
        motorController.stop();
    }

    if (bottomUltrasoundController.readDistanceCm().distanceCm >= 8) {
        motorController.stop();
        servoController.close();
    }

    // If we are not rotating, start rotating to center the target in the camera's view
    if (target.signature != 0 && !motorController.isRotating())
    {
        int16_t centerX = PIXY_CAM_WIDTH / 2;

        int16_t dx = (int16_t)target.x - centerX;

        // Control motors to adjust position based on target.x and target.y
        if (dx > THRESHOLD_X)
        {
            logger->log(LogLevel::Debug, dx, 0);
            // target is to the right, rotate right
            motorController.rotate(true, 50); // Example parameters, adjust as needed
        }
        else if (dx < -THRESHOLD_X)
        {
            logger->log(LogLevel::Debug, dx, 0);
            // target is to the left, rotate left
            motorController.rotate(false, 50); // Example parameters, adjust as needed
        }
    }

    return SUCCESS;
}