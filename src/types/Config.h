#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include "sensors/interfaces/IPixySensor.h"
#include "sensors/interfaces/IUltraSonicSensor.h"

struct TargetParams {
    uint8_t signatures[3]; // Array of valid signatures for the target
    int16_t min_Height;
    int16_t max_Height;
    int16_t min_Width;
    int16_t max_Width;
};

struct PixyConfig {
    IPixySensor *pixy;
    TargetParams ballParams;
    TargetParams baseParams;
    int16_t thresholdX;
    int16_t thresholdY;
};

struct MotorConfig {
    uint8_t leftPwmPin;
    uint8_t leftDirPin;
    uint8_t rightPwmPin;
    uint8_t rightDirPin;
};

struct ServoConfig {
    uint8_t servoPin;
    uint16_t intervalMs;
};

struct UltraSoundConfig {
    IUltraSonicSensor *ultraSonic;
    uint8_t trigPin;
    uint8_t echoPin;
};

struct Config {
    PixyConfig pixyConfig;
    MotorConfig motorConfig;
    ServoConfig servoConfig;
    UltraSoundConfig bottomUltrasoundConfig;
    UltraSoundConfig topUltrasoundConfig;
};

#endif