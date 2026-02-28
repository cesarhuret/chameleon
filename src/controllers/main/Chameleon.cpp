#include <Arduino.h>

#include "Chameleon.h"
#include "constants/Pixy.h"

void Chameleon::init(ILogger *logger, IPixySensor *pixy, IUltraSonicSensor *ultraSonic)
{
    // Initialize the PixyController with appropriate parameters
    int8_t status = pixyController.init(pixy, 1, 40, 150, 40, 150, 20, 20); // Example parameters, adjust as needed
    if (status == 0)
    {
        logger->info("PixyController initialized successfully.");
    }
    else
    {
        logger->error("Failed to initialize PixyController: %d", status);
        return; // Return early if initialization fails
    }

    // Initialize the UltraSonicController with appropriate parameters
    status = ultraSonicController.init(ultraSonic); // Example parameters, adjust as needed
    if (status == 0)
    {
        logger->info("UltraSonicController initialized successfully.");
    }
    else
    {
        logger->error("Failed to initialize UltraSonicController: %d", status);
        return; // Return early if initialization fails
    }

    this->logger = logger;
    this->currentState = State::SEARCHING_FOR_BALL;
}

void Chameleon::run()
{

    // used by the whole state machine, we update it at the beginning of each loop so we always have the latest information on our target ball
    // used by motors to tell how close we are
    Types::DetectedBlock target = pixyController.getCurrentTargetBall();

    logger->debug("%s Target: x=%d y=%d width=%d height=%d signature=%d area=%d age=%d index=%d angle=%d",
                  Types::toString(currentState), target.x, target.y, target.width, target.height, target.signature, target.area, target.age, target.index, target.angle);

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
            target = pixyController.findTargetBall();
        }

        //! TODO: if targetBall is not stale
        if (target.signature != 0)
        {
            logger->info("Target ACQUIRED -> CENTER TARGET");
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
            logger->info("TARGET CENTERED -> MOVE TO BALL");
            motorController.stop();
            currentState = State::MOVING_TO_BALL;
            break;
        }

        // If we are not rotating, start rotating to center the target in the camera's view
        if (!motorController.isRotating())
        {
            logger->info("STARTING ROTATION...");

            // Control motors to adjust position based on target.x and target.y
            motorController.rotate(0, 0);

            break;
        }

        break;
    }

    case State::MOVING_TO_BALL:
    {
        // ============================================
        // Movement (To Target)
        // ============================================

        uint16_t distance = ultraSonicController.readDistanceCm();
        logger->debug("%s Target: x=%d y=%d width=%d height=%d signature=%d area=%d age=%d index=%d angle=%d | Distance=%d cm",
                  Types::toString(currentState), target.x, target.y, target.width, target.height, target.signature, target.area, target.age, target.index, target.angle, distance);

        // If we are within 3 cm of an object, transition to controlling the claw
        // reading distance takes multiple frames so we need timing to account for that
        if (ultraSonicController.isThereObjectWithin(4))
        {
            logger->info("OBJECT WITHIN THRESHOLD -> GRAB CLAW");
            currentState = State::GRAB_CLAW;
            motorController.stop();
            break;
        }

        // if we are not moving, start moving towards the target
        if (!motorController.isMoving())
        {
            logger->info("STARTING MOVEMENT...");
            // Control motors to move towards the target based on target.x and target.y
            motorController.move(0, 0);
        }

        break;
    }

    case State::GRAB_CLAW:
    {
        // ============================================
        // Claw (Grab)
        // ============================================

        if (!servoController.isMoving())
        {
            logger->info("CLOSING CLAW...");

            // takes multiple frames to close the claw
            servoController.close();
        }

        if (servoController.isClosed())
        {
            logger->info("BALL GRABBED -> ROTATE TO BASE");
            currentState = State::ROTATE_TO_BASE;
            break;
        }

        break;
    }

    case State::RELEASE_CLAW:
    {
        // ============================================
        // Claw (Release)
        // ============================================

        // we are at the base, open the claw to release the ball

        // takes multiple frames to open the claw
        if (!servoController.isMoving())
        {
            logger->info("OPENING CLAW...");
            servoController.open();
        }

        if (servoController.isOpen())
        {
            currentState = State::ROTATE_TO_CENTER; // Start searching for the next ball
            break;
        }
        break;
    }

        // case State::ROTATE_TO_CENTER:
        // {
        //     // ============================================
        //     // Rotate to Center
        //     // ============================================

        //     if (true) // pixyController.isCenterCentered()
        //     {
        //         motorController.stop();
        //         currentState = State::SEARCHING_FOR_BALL;
        //         break;
        //     }

        //     // If we are not rotating, start rotating to face the center
        //     if (!motorController.isRotating())
        //     {
        //         Serial.println("Rotating to center position...");

        //         // Control motors to rotate towards the center
        //         motorController.rotate(0, 0);
        //         break;
        //     }
        //     break;
        // }

        // case State::ROTATE_TO_BASE:
        // {
        //     // ============================================
        //     // Rotate to Base
        //     // ============================================

        //     if (true) // pixyController.isBaseCentered()
        //     {
        //         motorController.stop();
        //         currentState = State::MOVING_TO_BASE;
        //         break;
        //     }

        //     // If we are not rotating, start rotating to face the base
        //     if (!motorController.isRotating())
        //     {
        //         Serial.println("Rotating to base position...");

        //         // Control motors to rotate towards the base
        //         // motorController.rotate(motorController.BASE_X, motorController.BASE_Y);
        //         break;
        //     }
        //     break;
        // }

        // case State::MOVING_TO_BASE:
        // {
        //     // ============================================
        //     // Movement (To Base)
        //     // ============================================

        //     // PixyCam can detect lines, so we can use it to follow a line back to the base
        //     // We would first have to rotate to find the line though

        //     if (true) // if at base position
        //     {
        //         currentState = State::RELEASE_CLAW;
        //         motorController.stop();
        //         break;
        //     }

        //     if (!motorController.isMoving())
        //     {
        //         Serial.println("Moving towards base...");

        //         // Control motors to move towards the base
        //         motorController.move(0, 0); // Example parameters, adjust as needed
        //     }
        //     break;
        // }
    }
}