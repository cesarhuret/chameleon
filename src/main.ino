#include "controllers/main/Chameleon.h"
#include "SPI.h"

#include "sensors/arduino/PixySensor.h"
#include "sensors/arduino/UltraSonicSensor.h"
#include "helpers/logging/SerialLogger.h"

Chameleon chameleon;
UltraSonicSensor *ultraSonicSensor; // Declare as global to access in loop() for testing

const int trigPin = 3;
const int echoPin = 2;

float duration, distance;

void setup()
{

    Serial.begin(115200);
    ILogger *logger = new SerialLogger(LogLevel::Debug, 115200);

    logger->info("Initializing Chameleon...");

    IPixySensor *pixySensor = new PixySensor();

    ultraSonicSensor = new UltraSonicSensor(3, 2);
    
    chameleon.init(logger, pixySensor, ultraSonicSensor); // Pass both sensors to the Chameleon controller
}

void loop()
{
    chameleon.run();
}