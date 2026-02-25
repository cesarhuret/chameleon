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

    switch (currentState)
    {

        case State::SEARCHING_FOR_BALL: {

            // ============================================
            // Ball Detection
            // ============================================

            // Get our current target
            Types::Block target = pixyController.getCurrentTargetBall();

            // If we don't have a target, find one
            if (target.signature == 0)
            {
                Serial.println("No target currently tracked.");
                target = pixyController.findTargetBall();
            }
        }

        case State::MOVING_TO_BALL: {
            
            // ============================================
            // Movement (To Target)
            // ============================================

            // Start moving towards the target if we have one
            // Check distance to target using Ultrasonic

            // Read off ultrasonic distance to target
            // If we are far, keep moving towards the target
            // If we are close, stop and transition to controlling the claw
        }

        case State::CONTROLLING_CLAW: {
            // ============================================
            // Claw (Open/Close)
            // ============================================

            // If ultrasound distance is less than threshold, close claw
            // Skip to movement (To Base) once claw is closed
        }

        case State::MOVING_TO_BASE: {
            // ============================================
            // Movement (To Base)
            // ============================================
            
            // PixyCam can detect lines, so we can use it to follow a line back to the base
            // We would first have to rotate to find the line though
        }
    }
}