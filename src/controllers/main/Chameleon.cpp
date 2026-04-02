#include <Arduino.h>

#include "Chameleon.h"
#include "constants/Pixy.h"

uint8_t Chameleon::init(ILogger *logger, IPixySensor *pixy, IUltraSonicSensor *ultraSonic)
{
    this->logger = logger;
    this->currentState = State::SEARCHING_FOR_BALL;
    this->pixy = pixy;

    logger->log(LogLevel::Info, 123, 1);
    // Initialize the PixyController with appropriate parameters
    int8_t status = pixyController.init(pixy, 1, 40, 150, 40, 150, 40, 20); // Example parameters, adjust as needed
    if (status != Codes::SUCCESS)
    {
        return status; // Return early if initialization fails
    }

    // logger->info("Initializing UltraSonicController...");
    // Initialize the UltraSonicController with appropriate parameters
    status = ultraSonicController.init(ultraSonic); // Example parameters, adjust as needed
    if (status != Codes::SUCCESS)
    {
        // logger->error("Failed to initialize UltraSonicController: %d", status);
        return status; // Return early if initialization fails
    }

    servoController.init(7, 20); // Example pin and interval, adjust as needed

    motorController.init(3, 4, 5, 6); // Example pins for left and right motors, adjust as needed

    return Codes::SUCCESS; // Return success if all initializations are successful
}

uint8_t Chameleon::run()
{

    // used by the whole state machine, we update it at the beginning of each loop so we always have the latest information on our target ball
    // used by motors to tell how close we are
    Types::PixyResult result = pixyController.getCurrentTargetBall();

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
        else if (currentState != State::CLAW_REPEATS && currentState != State::MOVE_TO_BARRIER && currentState != State::MOVE_BACK_TO_BASE)
        {
            // If we are not searching for a ball and there was an error getting the current target ball, we should probably go back to searching for a ball
            servoController.open();
            previousState = currentState;
            currentState = State::SEARCHING_FOR_BALL;
        }
    }

    // if ball shifts but stays on pixy fov, recenter to ball
    if (currentState != State::GRAB_CLAW && currentState != State::CLAW_REPEATS && currentState != State::MOVE_TO_BARRIER && currentState != State::MOVE_BACK_TO_BASE && currentState > State::CENTERING_TARGET && !pixyController.isBlockCentered(target))
    {
        // If we are past the centering state and our target is no longer centered, we should probably go back to centering the target
        motorController.stop();
        previousState = currentState;
        currentState = State::CENTERING_TARGET;
    }

    if (currentState == State::MOVE_TO_BARRIER && target.signature == 0)
    {
        // put the parameters for the colored barrier here, we want to use the same parameters as we used for the ball so we can reuse the same code, just with a different signature
        int8_t status = pixyController.init(pixy, 2, 40, 150, 40, 150, 40, 20); // Example parameters, adjust as needed

    }

    if (currentState == State::MOVE_BACK_TO_BASE && target.signature == 0)
    {
        // put the parameters for the colored barrier here, we want to use the same parameters as we used for the ball so we can reuse the same code, just with a different signature
        int8_t status = pixyController.init(pixy, 3, 40, 150, 40, 150, 40, 20); // Example parameters, adjust as needed

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
            result = pixyController.findTargetBall();
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
        if (pixyController.isBlockCentered(target))
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

        // UltraSonicResult distanceResult = ultraSonicController.readDistanceCm();
        // logger->debug("%s Target: x=%d y=%d width=%d height=%d signature=%d area=%d age=%d index=%d angle=%d | Distance=%d cm",
        //   Types::toString(currentState), target.x, target.y, target.width, target.height, target.signature, target.area, target.age, target.index, target.angle, distance);

        // Serial.print("Distance to object: ");
        // Serial.print(ultraSonicController.readDistanceCm().distanceCm);
        // Serial.println(" cm");

        // If we are within 3 cm of an object, transition to controlling the claw
        // reading distance takes multiple frames so we need timing to account for that
        if (ultraSonicController.isThereObjectWithin(3.5).isWithinThreshold)
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
            // Control motors to move towards the target based on target.x and target.y
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
        
        // servoController.open(); // Open the claw to release the ball, we can change this later to only open when we are at the base

        previousState = currentState;
        // currentState = State::SEARCHING_FOR_BALL; // Start searching for the next ball
        currentState = State::CLAW_REPEATS;    // Start claw repeat cycle for testing

        break;
    }

    case State::CLAW_REPEATS:
    {
        // ============================================
        // Claw (Repeat)
        // ============================================

        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();
        delay(1000);
        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();
        delay(1000);
        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();
        delay(1000);
        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();
        delay(1000);
        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();
        delay(1000);
        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();
        delay(1000);
        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();
        delay(1000);
        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();
        delay(1000);
        servoController.open();
        delay(1000); // Interval between open and close, adjust as needed
        servoController.close();

        previousState = currentState;
        currentState = State::MOVE_TO_BARRIER; 

        break;
    }

    case State::MOVE_TO_BARRIER:
    {
        // ============================================
        // Movement (To Barrier)
        // ============================================

        if (ultraSonicController.isThereObjectWithin(5).isWithinThreshold)
        {
            motorController.stop();
            motorController.rotate(false, 800); // Example parameters, rotate 180 degrees to turn around, adjust as needed
            previousState = currentState;
            currentState = State::MOVE_BACK_TO_BASE;
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

    case State::MOVE_BACK_TO_BASE:
    {
        // ============================================
        // Movement (To Base)
        // ============================================

        if (ultraSonicController.isThereObjectWithin(5).isWithinThreshold)
        {
            previousState = currentState;
            currentState = State::SEARCHING_FOR_BALL;
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
}