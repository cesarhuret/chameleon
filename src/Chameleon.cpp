#include "Chameleon.h"

void Chameleon::init(IPixySensor *pixy)
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
}

void Chameleon::run()
{
    // ============================================
    // Ball Detection
    // ============================================

    // Get our current target
    Block target = pixyController.getCurrentTargetBall();

    // If we don't have a target, find one
    if (target.m_signature == 0)
    {
        Serial.println("No target currently tracked.");
        target = pixyController.findTargetBall();
    }

    // ============================================
    // Movement (To Target)
    // ============================================

    // Start moving towards the target if we have one
    // Check distance to target using Ultrasonic

    // If we are close, skip to claw control


    // ============================================
    // Claw (Open/Close)
    // ============================================

    // If ultrasound distance is less than threshold, close claw
    // Skip to movement (To Base) if we have the ball

    // ============================================
    // Movement (To Base)
    // ============================================

}