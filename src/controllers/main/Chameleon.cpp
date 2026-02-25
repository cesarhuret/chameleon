#include <Arduino.h>

#include "Chameleon.h"

void Chameleon::init(IPixySensor *pixy, IUltraSonicSensor *ultraSonic)
{
    // Initialize the PixyController with appropriate parameters
    int8_t status = pixyController.init(pixy, 1, 10, 10, 20, 20); // Example parameters, adjust as needed
    if (status == 0)
    {
        Serial.println("PixyController initialized successfully.");
    }
    else
    {
        Serial.print("Failed to initialize PixyController: ");
        Serial.println(status);
    }

    // Initialize the UltraSonicController with appropriate parameters
    status = ultraSonicController.init(ultraSonic); // Example parameters, adjust as needed
    if (status == 0)
    {
        Serial.println("UltraSonicController initialized successfully.");
    }
    else
    {
        Serial.print("Failed to initialize UltraSonicController: ");
        Serial.println(status);
    }
}

void Chameleon::run()
{

    // used by the whole state machine, we update it at the beginning of each loop so we always have the latest information on our target ball
    // used by motors to tell how close we are
    Types::Block target = pixyController.getCurrentTargetBall();

    switch (currentState)
    {

    case State::SEARCHING_FOR_BALL:
    {

        // ============================================
        // Ball Detection
        // ============================================

        // Get our current target - it gets a stale value, need to refresh
        target = pixyController.getCurrentTargetBall();

        // If we don't have a target, find one
        if (target.signature == 0)
        {
            Serial.println("No target currently tracked.");
            target = pixyController.findTargetBall();
        }

        //! TODO: if targetBall is not stale
        if (target.signature != 0)
        {
            currentState = State::CENTERING_TARGET;
        }
    }

    case State::CENTERING_TARGET:
    {
        // ============================================
        // Rotate Target to Center
        // ============================================

        if (pixyController.isBlockCentered(target))
        {
            motorController.stop();
            currentState = State::MOVING_TO_BALL;
            break;
        }

        // We are not centered

        // If we are not rotating, start rotating to center the target in the camera's view
        if (!motorController.isRotating())
        {
            Serial.println("Target is not centered, adjusting position...");

            // Control motors to adjust position based on target.x and target.y
            motorController.rotate(0, 0);

            break;
        }
    }

    case State::MOVING_TO_BALL:
    {

        // move towards the target
        Serial.println("Target is centered, moving towards it...");

        // If we are within 3 cm of an object, transition to controlling the claw
        // reading distance takes multiple frames so we need timing to account for that
        if (ultraSonicController.isThereObjectWithin(3))
        {
            currentState = State::GRAB_CLAW;
            motorController.stop();
            break;
        }

        // ============================================
        // Movement (To Target)
        // ============================================

        // if we are not moving, start moving towards the target
        if (!motorController.isMoving())
        {
            // Control motors to move towards the target based on target.x and target.y
            motorController.move(0, 0);
        }
    }

    case State::GRAB_CLAW:
    {
        // ============================================
        // Claw (Grab)
        // ============================================

        // object is within threshold distance, close the claw to grab the ball

        // takes multiple frames to close the claw
        servoController.close();

        if (servoController.isClosed())
        {
            currentState = State::ROTATE_TO_BASE;
            break;
        }
    }

    case State::RELEASE_CLAW:
    {
        // ============================================
        // Claw (Release)
        // ============================================

        // we are at the base, open the claw to release the ball

        // takes multiple frames to open the claw
        servoController.open();

        if (servoController.isOpen())
        {
            currentState = State::ROTATE_TO_CENTER; // Start searching for the next ball
            break;
        }
    }

    case State::ROTATE_TO_CENTER:
    {
        // ============================================
        // Rotate to Center
        // ============================================

        if (true) // pixyController.isCenterCentered()
        {
            motorController.stop();
            currentState = State::SEARCHING_FOR_BALL;
            break;
        }

        // If we are not rotating, start rotating to face the center
        if (!motorController.isRotating())
        {
            Serial.println("Rotating to center position...");

            // Control motors to rotate towards the center
            motorController.rotate(0, 0);
            break;
        }
    }

    case State::ROTATE_TO_BASE:
    {
        // ============================================
        // Rotate to Base
        // ============================================

        if (true) // pixyController.isBaseCentered()
        {
            motorController.stop();
            currentState = State::MOVING_TO_BASE;
            break;
        }

        // If we are not rotating, start rotating to face the base
        if (!motorController.isRotating())
        {
            Serial.println("Rotating to base position...");

            // Control motors to rotate towards the base
            // motorController.rotate(motorController.BASE_X, motorController.BASE_Y);
            break;
        }
    }

    case State::MOVING_TO_BASE:
    {
        // ============================================
        // Movement (To Base)
        // ============================================

        // PixyCam can detect lines, so we can use it to follow a line back to the base
        // We would first have to rotate to find the line though

        if (true) // if at base position
        {
            currentState = State::RELEASE_CLAW;
            motorController.stop();
            break;
        }

        if (!motorController.isMoving())
        {
            Serial.println("Moving towards base...");

            // Control motors to move towards the base
            motorController.move(0, 0); // Example parameters, adjust as needed
        }
    }
    }
}