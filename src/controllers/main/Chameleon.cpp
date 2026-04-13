#include <Arduino.h>

#include "Chameleon.h"
#include "constants/Pixy.h"
#include "types/Config.h"

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

    status = motorController.init();
    if (status != Codes::SUCCESS)
    {
        return status;
    }

    logger->log(LogLevel::Info, Codes::MOTOR_INITIALIZATION_OK, 0);

    return Codes::SUCCESS; // Return success if all initializations are successful
}

uint8_t Chameleon::run()
{
    // just refresh whatever blocks the pixycam can see
    pixyController.updateBlocks();

    // just returns a list of blocks
    PixyArrayResult result = pixyController.getBlocks();

    DetectedBlock ball;

    // print out all blocks
    for (uint8_t i = 0; i < result.count; i++) {

        const DetectedBlock &block = result.blocks[i];

        Serial.print("Block ");
        Serial.print(block.index);
        Serial.print(": ");
        Serial.print(block.x);
        Serial.print(", ");
        Serial.println(block.y);

        if (block.signature == BALL_SIG_RED) {
            ball = block;
        }
    }

    // TODO: We need to get the robot to rotate to the ball we want, center, and move to it 

    if(ball.signature != 0) {
        Serial.println("Found a ball!");
    }

    // if block is to the left, rotate left


    // if block is to the right, rotate right


    // motorController.move(true, 100); // Move forward at speed 100
    // motorController.move(false, 100); // Move backward at speed 100
    // motorController.rotate(true, 100); // Rotate right at speed 100
    // motorController.rotate(false, 100); // Rotate left at speed 100


    return SUCCESS;
}