#ifndef CODE_H
#define CODE_H

#include <stdint.h>

namespace Codes {
    
    constexpr uint8_t SUCCESS = 0x00;
    constexpr uint8_t SETUP_SUCCESS = 0x01;
    constexpr uint8_t INIT_FAILURE = 0x02;
    constexpr uint8_t RUNTIME_ERROR = 0x03;
    
    // PixyController error codes
    constexpr uint8_t PIXY_INITIALISING = 0x20;
    constexpr uint8_t PIXY_NOT_AVAILABLE = 0x21;
    // PixySensor error codes
    constexpr uint8_t PIXY_INVALID_INDEX = 0x22;
    constexpr uint8_t PIXY_BLOCK_NOT_FOUND = 0x23;
    constexpr uint8_t PIXY_TARGET_NOT_FOUND = 0x24;

    // #define PIXY_RESULT_OK                       0
    // #define PIXY_RESULT_ERROR                    -1
    // #define PIXY_RESULT_BUSY                     -2
    // #define PIXY_RESULT_CHECKSUM_ERROR           -3
    // #define PIXY_RESULT_TIMEOUT                  -4
    // #define PIXY_RESULT_BUTTON_OVERRIDE          -5
    // #define PIXY_RESULT_PROG_CHANGING            -6

    constexpr uint8_t ULTRASONIC_NOT_AVAILABLE = 0x30;
    constexpr uint8_t ULTRASONIC_OUT_OF_RANGE = 0x31;

    constexpr uint8_t HALT = 0x09;
}

#endif