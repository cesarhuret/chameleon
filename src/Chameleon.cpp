#include "Chameleon.h"

void Chameleon::init()
{
    // Initialize the PixyController with appropriate parameters
    int8_t status = pixyController.init(1, 10, 10, 20, 20); // Example parameters, adjust as needed

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
    
}