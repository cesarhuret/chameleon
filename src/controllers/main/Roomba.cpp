#include <Arduino.h>

#include "Roomba.h"
#include "types/Config.h"

uint8_t Roomba::init(ILogger *logger, RuntimeConfig config)
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

    status = topUltrasoundController.init(config.topUltraSonic, TOP_ULTRASONIC_TRIG_PIN, TOP_ULTRASONIC_ECHO_PIN);
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

    led.init();

    led.write(false, true, false);

    delay(1000);

    return Codes::SUCCESS; // Return success if all initializations are successful
}

uint8_t Roomba::run()
{
    pixyController.updateBlocks();

    Types::PixyResult result = pixyController.getBall();
    Types::DetectedBlock target = result.block;

    pixyController.updateHiddenBlocks();

    uint16_t topUltrasonicDistance = topUltrasoundController.readDistanceCm().distanceCm;

    if (target.signature != 0)
    {
        lastValidTarget = target;
    }

    // obstcle
    if (topUltrasonicDistance <= 12 && currentState != State::RELEASE_CLAW)
    {
        motorController.stop();
        motorController.move(false, 60);
        delay(500);

        motorController.stop();
        if (currentState == State::CENTERING_TARGET || currentState == State::SEARCHING_FOR_BALL)
        {
            currentState = State::ROTATE_TO_CENTER;
        }
    }

    // Types::PixyArrayResult hiddenBlocksResult = pixyController.getHiddenBlocks();
    // // for (uint8_t i = 0; i < 4; i++)
    // // {
    // //     logger->log(LogLevel::Debug, packBlock(hiddenBlocksResult.blocks[i]), 3);
    // // }

    logger->log(LogLevel::Debug, currentState, 2);
    logger->log(LogLevel::Debug, packBlock(pixyController.findBase().block), 3);
    logger->log(LogLevel::Debug, packBlock(target), 3);
    // logger->log(LogLevel::Debug, bottomUltrasoundController.readDistanceCm().distanceCm, 0);
    logger->log(LogLevel::Debug, topUltrasonicDistance, 0);

    switch (currentState)
    {

    case State::SEARCHING_FOR_BALL:
    {

        // ============================================
        // Ball Detection
        // ============================================

        // If we don't have a target ball, find one
        if (target.signature == 0)
        {
            led.write(true, true, true);

            result = pixyController.findBall();
            target = result.block;
        }

        if (target.signature != 0)
        {
            led.write(target.signature == 1, target.signature == 2, target.signature == 3);
            // logger->log(LogLevel::Debug, packBlock(target), 3);
            motorController.stop();
            currentState = State::CENTERING_TARGET;
        }

        if (servoController.isOpen())
        {
            servoController.close();
        }

        break;
    }

    case State::CENTERING_TARGET:
    {
        // ============================================
        // Rotate Target to Center
        // ============================================

        // If the target is centered, transition to moving towards the ball
        if (pixyController.isCentered(target, THRESHOLD_X))
        {
            // logger->info("TARGET CENTERED -> MOVE TO BALL");
            motorController.stop();
            motorController.move(true, 150);
        }

        if (target.signature != 0 && target.y > 170)
        {
            Serial.print("IS HB: ");
            Serial.println(pixyController.isThereAHiddenBlock());
            if (!pixyController.isThereAHiddenBlock())
            {
                servoController.open();
            }
            else
            {
                servoController.close();
                delay(1000);
                motorController.stop();
                motorController.move(false, 150);
                delay(1000);
                motorController.stop();
                pixyController.resetIsThereAHiddenBlock();
                pixyController.resetBall();
                currentState = State::SEARCHING_FOR_BALL;
                break;
            }
        }

        if (target.signature == 0 && lastValidTarget.y > 170)
        {
            motorController.move(true, 150);
        }
        else if (target.signature == 0)
        {
            motorController.stop();
            currentState = State::ROTATE_TO_CENTER;
        }

        if (bottomUltrasoundController.readDistanceCm().distanceCm >= 8)
        {
            motorController.stop();
            currentState = State::GRAB_CLAW;
            break;
        }

        // If we are not rotating, start rotating to center the target in the camera's view
        if (target.signature != 0 && !motorController.isRotating())
        {
            // logger->info("STARTING ROTATION...");

            int16_t centerX = PIXY_CAM_WIDTH / 2;

            int16_t dx = (int16_t)target.x - centerX;

            // Control motors to adjust position based on target.x and target.y
            if (dx > THRESHOLD_X)
            {
                // logger->log(LogLevel::Debug, dx, 0);
                // target is to the right, rotate right
                motorController.rotate(true, ROTATE_SPEED); // Example parameters, adjust as needed
            }
            else if (dx < -THRESHOLD_X)
            {
                // logger->log(LogLevel::Debug, dx, 0);
                // target is to the left, rotate left
                motorController.rotate(false, ROTATE_SPEED); // Example parameters, adjust as needed
            }
        }

        break;
    }

    case State::GRAB_CLAW:
    {
        // ============================================
        // Claw (Grab)
        // ============================================

        servoController.close();

        delay(1000); // Wait for the claw to close, adjust as needed

        currentState = State::FIND_BASE; // Find the base by rotating

        break;
    }

    case State::FIND_BASE:
    {
        // ============================================
        // Find Base by rotating and using the PixyCam
        // ============================================

        DetectedBlock baseResult = pixyController.findBase().block;

        if (baseResult.signature == 0)
        {
            led.write(true, true, false); // yellow
            // We don't have a target base, and out of all visible blocks it's not in there either
            // So start rotating to find the base
            motorController.rotate(true, ROTATE_SPEED); // rotate left
            // once a valid base block is found, the findBase will set it as our target
            break;
        }
        else
        {
            // so if we have a target base
            // we're rotating left until we find a base and is centered
            led.write(false, true, false); // green
            motorController.stop();
            currentState = State::MOVING_TO_BASE;
            break;
        }

        break;
    }

    case State::MOVING_TO_BASE:
    {
        // ============================================
        // Movement (To Base)
        // ============================================

        // PixyCam can detect lines, so we can use it to follow a line back to the base
        // We would first have to rotate to find the line though

        DetectedBlock baseResult = pixyController.findBase().block;

        if (baseResult.signature == 0)
        {
            currentState = State::FIND_BASE;
            break;
        }

        if (pixyController.isCentered(baseResult, THRESHOLD_X))
        {
            // logger->info("TARGET CENTERED -> MOVE TO BALL");
            motorController.stop();
            motorController.move(true, 120);
        }

        if (topUltrasoundController.isThereObjectWithin(25).isWithinThreshold) // if at base position
        {
            led.write(true, true, true);
            motorController.move(true, 200);
            currentState = State::RELEASE_CLAW;
            break;
        }

        // if (!motorController.isMoving())
        // {
        //     // Control motors to move towards the base
        //     motorController.move(true, 200); // Example parameters, adjust as needed
        // }

        // If we are not rotating, start rotating to center the target in the camera's view
        if (baseResult.signature != 0 && !motorController.isRotating())
        {
            // logger->info("STARTING ROTATION...");

            int16_t centerX = PIXY_CAM_WIDTH / 2;

            int16_t dx = (int16_t)baseResult.x - centerX;

            // Control motors to adjust position based on target.x and target.y
            if (dx > THRESHOLD_X)
            {
                logger->log(LogLevel::Debug, dx, 0);
                // target is to the right, rotate right
                motorController.rotate(true, ROTATE_SPEED); // Example parameters, adjust as needed
            }
            else if (dx < -THRESHOLD_X)
            {
                logger->log(LogLevel::Debug, dx, 0);
                // target is to the left, rotate left
                motorController.rotate(false, ROTATE_SPEED); // Example parameters, adjust as needed
            }
        }

        break;
    }

    case State::RELEASE_CLAW:
    {
        // ============================================
        // Claw (Release)
        // ============================================

        if (servoController.isClosed())
        {
            servoController.open();
        }

        if (topUltrasoundController.isThereObjectWithin(4).isWithinThreshold)
        {
            delay(200);
            motorController.stop();          // we're headed to the base as we open the claw, so when claw is open stop
            currentState = State::MOVE_BACK; // Rotate to center to prepare for next search
        }

        break;
    }

    case State::MOVE_BACK:
    {
        // ============================================
        // Move Backwards (From Base)
        // ============================================

        motorController.move(false, 50); // move backwards a little bit to be clear of the base, adjust as needed

        // reset current ball, base, increment sig
        pixyController.resetBall();
        pixyController.resetIsThereAHiddenBlock();
        pixyController.incrementBallSig();

        delay(1000); // Small delay to ensure we have moved back from the base before any further actions, adjust as needed

        servoController.close(); // close claw to be ready for next grab

        currentState = State::ROTATE_TO_CENTER; // Rotate to center to prepare for next search

        break;
    }

    case State::ROTATE_TO_CENTER:
    {
        // ============================================
        // Rotate to Center
        // ============================================

        // We can just rotate in place until we find a ball again, which will be our next target
        PixyResult result = pixyController.findBall();
        PixyResult baseResult = pixyController.findBase();

        // if the base is in the view, or if there is no ball, rotate
        if (baseResult.block.signature != 0 || result.block.signature == 0 || topUltrasonicDistance <= 25)
        {
            // We don't have a target ball, and out of all visible blocks it's not in there either
            // So start rotating to find the ball
            // logger->log(LogLevel::Debug, Codes::ROTATING_TO_FIND_BALL, 0);

            led.write(true, false, true); // purple

            motorController.rotate(true, ROTATE_SPEED); // rotate left
            break;
        }

        if (pixyController.isCentered(result.block, THRESHOLD_X) && topUltrasonicDistance >= 60)
        {
            // WE HAVE FOUND THE NEW TARGET - THIS BASICALLY GOES STRAIGHT TO CENTER_TARGET
            pixyController.resetBase();
            motorController.stop();
            motorController.rotate(true, ROTATE_SPEED);
            currentState = State::SEARCHING_FOR_BALL;
        }

        break;
    }
    }

    return SUCCESS;
}