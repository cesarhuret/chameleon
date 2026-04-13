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

    return Codes::SUCCESS; // Return success if all initializations are successful
}

uint8_t Chameleon::run()
{
    pixyController.updateBlocks();

    // used by the whole state machine, we update it at the beginning of each loop so we always have the latest information on our target ball
    // used by motors to tell how close we are
    Types::PixyResult result = pixyController.getBall();
    Types::PixyResult baseResult = pixyController.findBase();

    // pixyController.updateHiddenBlocks();

    // We can choose to either return an error or continue with an empty block
    // For now, let's continue with an empty block to keep the state machine running
    Types::DetectedBlock target = result.block;

    if (target.signature != 0)
    {
        lastValidTarget = target;
    }

    if ((currentState == State::CENTERING_TARGET || currentState == State::MOVING_TO_BALL) && target.signature != 0 && target.y > 170)
    {
        // Types::PixyArrayResult hiddenBlocks = pixyController.getHiddenBlocks();

        // logger->log(LogLevel::Debug, 100, 3);
        // logger->log(LogLevel::Debug, hiddenBlocks.count, 3);

        // if (hiddenBlocks.count == 0) {
        // }
        servoController.open();
    }
    // ============================================
    // Edge Case #1:
    // We are going towards the ball and it is now off camera.
    // The ball is right in front of the camera in a dead angle.
    // So we continue moving till it gets under the ultrasonic.
    // Put a 3 second timeout such that if nothing is under it, then reset
    // Note: if our target no longer exists, but we were getting close to it, we should probably just continue towards where it was instead of stopping immediately
    // ============================================
    else if ((currentState == State::CENTERING_TARGET || currentState == State::MOVING_TO_BALL) && target.signature == 0 && lastValidTarget.y > 170)
    {
        currentState = State::MOVING_TO_BALL;
    }
    // otherwise we should find a ball
    else if ((currentState == State::CENTERING_TARGET || currentState == State::MOVING_TO_BALL) && target.signature == 0 && lastValidTarget.y < 170)
    {

        logger->log(LogLevel::Debug, 111111111111, 0);

        motorController.stop();

        // if (currentState == State::CENTERING_TARGET && previousState == State::MOVING_TO_BALL)
        // {
        //     previousState = currentState;
        //     currentState = State::GRAB_CLAW;
        // }
        // else
        // {
        //     // If we are not searching for a ball and there was an error getting the current target ball, we should probably go back to searching for a ball
        previousState = currentState;
        currentState = State::SEARCHING_FOR_BALL;
        // }
    }
    // ============================================
    // Edge Case #2:
    // We are moving towards the ball but it shifts and is no longer centered
    // So we stop and go back to centering it before moving towards it again.
    // ============================================
    if (currentState == State::MOVING_TO_BALL && target.signature != 0 && !pixyController.isCentered(target))
    {
        motorController.stop();
        delay(500); // Small delay to ensure motors have stopped before any further actions, adjust as needed
        previousState = currentState;
        currentState = State::CENTERING_TARGET;
    }

    // if ((currentState == State::SEARCHING_FOR_BALL || currentState == State::OBSTACLE) && baseResult.block.signature != 0)
    // {
    //     previousState = currentState;
    //     currentState = State::ROTATE_TO_CENTER;
    // }

    // ============================================
    // Edge Case #3:
    // An obstacle is placed in front of us at any point
    // We must go around it.
    // else if (currentState != State::MOVING_TO_BASE && currentState != State::OBSTACLE && currentState != State::RELEASE_CLAW && currentState != State::ROTATE_TO_CENTER && topUltrasoundController.isThereObjectWithin(8).isWithinThreshold)
    // {
    //     motorController.stop();
    //     previousState = currentState;
    //     currentState = State::OBSTACLE;
    // }

    // // if ball shifts but stays on pixy fov, recenter to ball
    // if (currentState != State::GRAB_CLAW && currentState != State::MOVE_TO_BARRIER && currentState != State::MOVE_BACK_TO_BASE && currentState > State::CENTERING_TARGET && !pixyController.isCentered(target))
    // {
    //     // If we are past the centering state and our target is no longer centered, we should probably go back to centering the target
    //     motorController.stop();
    //     previousState = currentState;
    //     currentState = State::CENTERING_TARGET;
    // }

    logger->log(LogLevel::Debug, currentState, 2);
    logger->log(LogLevel::Debug, packBlock(pixyController.getBase().block), 3);
    logger->log(LogLevel::Debug, packBlock(target), 3);
    logger->log(LogLevel::Debug, bottomUltrasoundController.readDistanceCm().distanceCm, 0);

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
            result = pixyController.findBall();
            target = result.block;

            // if (!motorController.isRotating())
            //     motorController.rotate(true, 50); // rotate left to find a ball, example parameters, adjust as needed
        }

        //! TODO: if targetBall is not stale
        if (target.signature != 0)
        {
            logger->log(LogLevel::Debug, packBlock(target), 3); 
            // logger->info("Target ACQUIRED -> CENTER TARGET");
            motorController.stop();
            previousState = currentState;
            currentState = State::CENTERING_TARGET;
        }

        break;
    }

    case State::CENTERING_TARGET:
    {
        // ============================================
        // Rotate Target to Center
        // ============================================

        // If the target is centered, transition to moving towards the ball
        if (pixyController.isCentered(target))
        {
            // logger->info("TARGET CENTERED -> MOVE TO BALL");
            motorController.stop();
            // delay(100); // Small delay to ensure motors have stopped before moving towards the ball
            previousState = currentState;
            currentState = State::MOVING_TO_BALL;
            break;
        }

        // If we are not rotating, start rotating to center the target in the camera's view
        if (!motorController.isRotating())
        {
            // logger->info("STARTING ROTATION...");

            int16_t centerX = PIXY_CAM_WIDTH / 2;
            int16_t centerY = PIXY_CAM_HEIGHT / 2;

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

            break;
        }

        break;
    }

    case State::MOVING_TO_BALL:
    {
        // ============================================
        // Movement (To Target)
        // ============================================

        // If we are within 3 cm of an object, transition to controlling the claw
        // reading distance takes multiple frames so we need timing to account for that
        if (bottomUltrasoundController.readDistanceCm().distanceCm >= 8) // the ground is 3cm, so how did we magically go up? - it means we went haywire and must close
        {
            previousState = currentState;
            currentState = State::GRAB_CLAW;
            motorController.stop();
            break;
        }

        // if(servoController.isClosed()) {
        //     currentState = State::SEARCHING_FOR_BALL;
        //     motorController.stop();
        //     break;
        // }

        // if we are not moving, start moving towards the target
        if (!motorController.isMoving())
        {
            // logger->info("STARTING MOVEMENT...");
            motorController.move(true, 50); // Example parameters, adjust as needed
        }

        break;
    }

    case State::GRAB_CLAW:
    {
        // ============================================
        // Claw (Grab)
        // ============================================

        servoController.close();

        delay(2000); // Wait for the claw to close, adjust as needed

        previousState = currentState;
        currentState = State::FIND_BASE; // Find the base by rotating

        break;
    }

    case State::FIND_BASE:
    {
        // ============================================
        // Find Base by rotating and using the PixyCam
        // ============================================

        PixyResult result = pixyController.findBase();

        if (result.block.signature == 0)
        {
            // We don't have a target base, and out of all visible blocks it's not in there either
            // So start rotating to find the base
            motorController.rotate(true, 50); // rotate left
            // once a valid base block is found, the findBase will set it as our target
            break;
        }

        logger->log(LogLevel::Debug, packBlock(result.block), 3);

        // so if we have a target base
        // we're rotating left until we find a base and is centered
        if (pixyController.isCentered(result.block))
        {
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

        if (topUltrasoundController.isThereObjectWithin(15).isWithinThreshold) // if at base position
        {
            previousState = currentState;
            currentState = State::RELEASE_CLAW;
            break;
        }

        if (!motorController.isMoving())
        {
            // Control motors to move towards the base
            motorController.move(true, 120); // Example parameters, adjust as needed
        }
        break;
    }

    case State::RELEASE_CLAW:
    {
        // ============================================
        // Claw (Release)
        // ============================================

        servoController.open();

        delay(500); // Wait for the claw to open, adjust as needed

        motorController.stop(); // we're headed to the base as we open the claw, so when claw is open stop

        delay(500);                      // Small delay to ensure claw has opened before any further actions, adjust as needed
        motorController.move(false, 50); // move backwards a little bit to be clear of the base, adjust as needed

        // reset current ball, base, increment sig
        pixyController.resetBall();
        pixyController.incrementBallSig();

        delay(1000); // Small delay to ensure we have moved back from the base before any further actions, adjust as needed

        previousState = currentState;
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
        PixyResult baseResult = pixyController.getBase();

        // if the base is in the view, or if there is no ball, rotate
        if (baseResult.block.signature != 0 || result.block.signature == 0)
        {
            // We don't have a target ball, and out of all visible blocks it's not in there either
            // So start rotating to find the ball
            // logger->log(LogLevel::Debug, Codes::ROTATING_TO_FIND_BALL, 0);

            motorController.rotate(true, 50); // rotate left
            break;
        }

        if (pixyController.isCentered(result.block))
        {
            pixyController.resetBase();
            motorController.stop();
            previousState = currentState;
            currentState = State::SEARCHING_FOR_BALL; // Start searching for the next ball
        }

        break;
    }

    case State::OBSTACLE:
    {
        // ============================================
        // Obstacle Avoidance
        // ============================================

        if (!topUltrasoundController.isThereObjectWithin(8).isWithinThreshold)
        {
            // If the obstacle is removed, go back to the previous state

            logger->log(LogLevel::Debug, topUltrasoundController.isThereObjectWithin(8).isWithinThreshold, 4);
            logger->log(LogLevel::Debug, previousState, 2);

            currentState = previousState;
        }

        break;
    }
    }

    return SUCCESS;
}