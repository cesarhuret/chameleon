#include <Arduino.h>

#include "Chameleon.h"
#include "constants/Pixy.h"
#include "types/Config.h"

uint8_t Chameleon::init(ILogger *logger, const Config& config)
{
    this->logger = logger;
    this->currentState = State::SEARCHING_FOR_BALL;

    Serial.println("Initializing PixyController...");

    // Initialize the PixyController with appropriate parameters
    pixyController = new PixyController();
    int8_t status = pixyController->init(config.pixyConfig);
    if (status != Codes::SUCCESS)
    {
        return status;
    }
    
    Serial.println("PixyController initialized successfully.");

    // status = bottomUltrasoundController.init(config.bottomUltrasoundConfig);
    // if (status != Codes::SUCCESS)
    // {
    //     return status;
    // }

    // Serial.println("Bottom UltraSonicController initialized successfully.");

    // status = topUltrasoundController.init(config.topUltrasoundConfig);
    // if (status != Codes::SUCCESS)
    // {
    //     return status;
    // }

    // Serial.println("Top UltraSonicController initialized successfully.");

    // status = servoController.init(config.servoConfig);
    // if (status != Codes::SUCCESS)
    // {
    //     return status;
    // }

    // Serial.println("ServoController initialized successfully.");

    // status = motorController.init(config.motorConfig);
    // if (status != Codes::SUCCESS)
    // return status;
    // {
    // }

    // Serial.println("MotorController initialized successfully.");


    return Codes::SUCCESS; // Return success if all initializations are successful
}
/*
uint8_t Chameleon::run()
{

    // used by the whole state machine, we update it at the beginning of each loop so we always have the latest information on our target ball
    // used by motors to tell how close we are
    Types::PixyResult result = pixyController.getBall();

    Serial.print("Current target ball signature: ");
    Serial.println(result.block.signature);

    // We can choose to either return an error or continue with an empty block
    // For now, let's continue with an empty block to keep the state machine running
    Types::DetectedBlock target = result.block;

    // if our target no longer exists, go back to searching for a ball
    if (currentState != State::SEARCHING_FOR_BALL && (currentState != State::MOVING_TO_BALL && currentState != State::GRAB_CLAW) && target.signature == 0)
    {
        motorController.stop();

        if (currentState == State::CENTERING_TARGET && previousState == State::MOVING_TO_BALL)
        {
            previousState = currentState;
            currentState = State::GRAB_CLAW;
        }
        else
        {
            // If we are not searching for a ball and there was an error getting the current target ball, we should probably go back to searching for a ball
            servoController.open();
            previousState = currentState;
            currentState = State::SEARCHING_FOR_BALL;
        }
    }

    // if ball shifts but stays on pixy fov, recenter to ball
    if (currentState != State::GRAB_CLAW && currentState != State::MOVE_TO_BARRIER && currentState != State::MOVE_BACK_TO_BASE && currentState > State::CENTERING_TARGET && !pixyController.isCentered (target))
    {
        // If we are past the centering state and our target is no longer centered, we should probably go back to centering the target
        motorController.stop();
        previousState = currentState;
        currentState = State::CENTERING_TARGET;
    }

    // logger->log(LogLevel::Debug, currentState, 2);
    // logger->log(LogLevel::Debug, packBlock(target), 3);

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
            // Serial.println("Searching for ball...");
            result = pixyController.findBall();
            target = result.block;
        }

        //! TODO: if targetBall is not stale
        if (target.signature != 0)
        {
            // logger->info("Target ACQUIRED -> CENTER TARGET");
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
            Serial.println("Target centered, moving towards ball...");
            motorController.stop();
            delay(500); // Small delay to ensure motors have stopped before moving towards the ball
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

            Serial.print("Target position - X: ");
            Serial.print(target.x);
            Serial.print(" dX: ");
            Serial.println(dx);

            // Control motors to adjust position based on target.x and target.y
            if (dx > 40)
            {
                // target is to the right, rotate right
                Serial.println("Rotating left to center target...");
                motorController.rotate(false, 70); // Example parameters, adjust as needed
            }
            else if (dx < -40)
            {
                // target is to the left, rotate right
                Serial.println("Rotating right to center target...");
                motorController.rotate(true, 70); // Example parameters, adjust as needed
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

        // Serial.print("Distance to object: ");
        // Serial.print(ultraSonicController.readDistanceCm().distanceCm);
        // Serial.println(" cm");

        // If we are within 3 cm of an object, transition to controlling the claw
        // reading distance takes multiple frames so we need timing to account for that
        if (bottomUltrasoundController.isThereObjectWithin(3.5).isWithinThreshold)
        {
            // logger->info("OBJECT WITHIN THRESHOLD -> GRAB CLAW");
            // Serial.println("Object within threshold, preparing to grab...");

            previousState = currentState;
            currentState = State::GRAB_CLAW;
            motorController.stop();
            break;
        }

        // if we are not moving, start moving towards the target
        if (!motorController.isMoving())
        {
            // logger->info("STARTING MOVEMENT...");
            motorController.move(true, 150); // Example parameters, adjust as needed
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

        if(result.block.signature == 0)
        {
            // We don't have a target base, and out of all visible blocks it's not in there either
            // So start rotating to find the base
            Serial.println("Searching for base...");
            
            motorController.rotate(true, 70); // rotate left
            // once a valid base block is found, the findBase will set it as our target
            break;
        }

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

        if (topUltrasoundController.isThereObjectWithin(8).isWithinThreshold) // if at base position
        {
            previousState = currentState;
            currentState = State::RELEASE_CLAW;
            break;
        }

        if (!motorController.isMoving())
        {
            Serial.println("Moving towards base...");

            // Control motors to move towards the base
            motorController.move(true, 150); // Example parameters, adjust as needed
        }
        break;
    }

    case State::RELEASE_CLAW:
    {
        // ============================================
        // Claw (Release)
        // ============================================

        servoController.open();

        delay(1000); // Wait for the claw to open, adjust as needed

        motorController.stop(); // we're headed to the base as we open the claw, so when claw is open stop

        delay(500); // Small delay to ensure claw has opened before any further actions, adjust as needed

        // reset current ball, base, increment sig
        pixyController.resetBall();
        pixyController.resetBase();
        pixyController.incrementBallSig();

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

        if(result.block.signature == 0)
        {
            // We don't have a target ball, and out of all visible blocks it's not in there either
            // So start rotating to find the ball
            Serial.println("Rotating to find next ball...");
            
            motorController.rotate(true, 70); // rotate left
            break;
        }

        if (pixyController.isCentered(result.block))
        {
            motorController.stop();
            previousState = currentState;
            currentState = State::SEARCHING_FOR_BALL; // Start searching for the next ball
            break;
        }

        break;
    }

    case State::MOVE_BACK_TO_BASE:
    {
        // ============================================
        // Movement (To Base)
        // ============================================

        if (topUltrasoundController.isThereObjectWithin(5).isWithinThreshold)
        {
            previousState = currentState;
            // currentState = State::SEARCHING_FOR_BALL;
            // release claw
            motorController.stop();
            break;
        }

        // if we are not moving, start moving towards the target
        if (!motorController.isMoving())
        {
            // logger->info("STARTING MOVEMENT...");
            motorController.move(true, 150); // Example parameters, adjust as needed
            // Control motors to move towards the target based on target.x and target.y
        }

        break;
    }
    }

    return SUCCESS;
}*/