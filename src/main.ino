#include "controllers/main/Chameleon.h"
#include "SPI.h"

#include "sensors/arduino/PixySensor.h"
#include "sensors/arduino/UltraSonicSensor.h"
#include "controllers/actuators/ServoController.h"
#include "helpers/logging/BluetoothLogger.h"
#include "helpers/logging/SerialLogger.h"
#include "types/Codes.h"
#include "types/Config.h"

constexpr unsigned long BAUD_RATE = 115200UL;

Chameleon chameleon;
SerialLogger logger(LogLevel::Debug, BAUD_RATE);
PixySensor pixySensor;
UltraSonicSensor bottomUltrasoundSensor;
UltraSonicSensor topUltrasoundSensor;
// BluetoothLogger logger(Serial, LogLevel::Debug, BAUD_RATE); // uses 0 and 1

void setup()
{



    Serial.begin(BAUD_RATE);

    SPI.begin();
    
    Serial.println("Starting setup...");

//     Serial.print("pixySensor addr: ");
// Serial.println((uintptr_t)&pixySensor, HEX);

    Config config = Config{
        .pixyConfig = { // Pins 10, 11, 12, 13 are used for SPI communication with the Pixy2 camera
            .pixy = &pixySensor,
            .ballParams = TargetParams{
                .signatures = {1, 2, 3}, // Example signature for the target ball, adjust as needed
                .min_Height = 10,
                .max_Height = 200,
                .min_Width = 10,
                .max_Width = 200,
            },
            .baseParams = TargetParams{
                .signatures = {4, 5, 6}, // Example signatures for the base, adjust as needed
                .min_Height = 20,
                .max_Height = 300,
                .min_Width = 20,
                .max_Width = 300,
            },
            .thresholdX = 20,
            .thresholdY = 20
        },
        // .motorConfig = MotorConfig{
        //     .leftPwmPin = 3,  // Example left PWM pin, adjust as needed
        //     .leftDirPin = 4,  // Example left direction pin, adjust as needed
        //     .rightPwmPin = 5, // Example right PWM pin, adjust as needed
        //     .rightDirPin = 6  // Example right direction pin, adjust as needed
        // },
        // .servoConfig = ServoConfig{
        //     .servoPin = 7,   // Example servo pin, adjust as needed
        //     .intervalMs = 20 // Example interval for smooth movement, adjust as needed
        // },
        // .bottomUltrasoundConfig = UltraSoundConfig{
        //     .ultraSonic = &bottomUltrasoundSensor,
        //     .trigPin = A0, // Analog pin used as a digital output for trigger
        //     .echoPin = A1  // Analog pin used as a digital input for echo
        // },
        // .topUltrasoundConfig = UltraSoundConfig{
        //     .ultraSonic = &topUltrasoundSensor,
        //     .trigPin = A2, // Analog pin used as a digital output for trigger
        //     .echoPin = A3  // Analog pin used as a digital input for echo
        // }
    };
    
    // PixyController pixyController;

    //     int8_t status = pixyController.init(config.pixyConfig);
    // if (status != Codes::SUCCESS)
    // {
    //     Serial.print("PixyController initialization failed with status code: ");
    // }


    // uint8_t status = config.pixyConfig.pixy->init();

    //     if (status != Codes::SUCCESS)
    //     {
    //         Serial.print("PixySensor initialization failed with status code: ");
    //         Serial.println(status);
    //         logger.log(LogLevel::Error, status, 0);
    //         halt(); // Halt on initialization failure
    //     }




    uint8_t status = chameleon.init(&logger, config); // Pass both sensors to the Chameleon controller
    if (status != Codes::SUCCESS)
    {
        Serial.print("Initialization failed with status code: ");
        Serial.println(status);

        logger.log(LogLevel::Error, status, 0);
        halt(); // Halt on initialization failure
    }

    Serial.println("Setup complete, starting main loop...");

    logger.log(LogLevel::Info, Codes::SETUP_SUCCESS, 0);
    return;
}

void loop()
{

    Serial.println("Running main loop...");

    // uint8_t status = chameleon.run();
    // if (status != Codes::SUCCESS)
    // {
    //     logger.log(LogLevel::Error, status, 0);
    //     halt(); // Halt on runtime error
    // }
}

// Halt function to stop execution in case of any failure
void halt()
{
    while (true)
    {
        logger.log(LogLevel::Error, Codes::HALT, 0);
        delay(5000); // Log the error message every 5 seconds
    }
}
