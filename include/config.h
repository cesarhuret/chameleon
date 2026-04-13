#ifndef CONFIG_H
#define CONFIG_H


// =========================
// PIXY CONFIG
// =========================
#define MIN_BALL_HEIGHT 10
#define MAX_BALL_HEIGHT 100
#define MIN_BALL_WIDTH 10
#define MAX_BALL_WIDTH 100

#define BALL_SIG_RED 1
#define BALL_SIG_GREEN 2
#define BALL_SIG_BLUE 3

#define MIN_BASE_HEIGHT 20  
#define MAX_BASE_HEIGHT 300
#define MIN_BASE_WIDTH 20
#define MAX_BASE_WIDTH 300

#define BASE_SIG_1 4
#define BASE_SIG_2 5
#define BASE_SIG_3 6

#define THRESHOLD_X 22
#define THRESHOLD_Y 20


// =========================
// SERVO CONFIG
// =========================
#define SERVO_PIN 7
#define SERVO_INTERVAL_MS 20
#define OPEN_ANGLE 130
#define CLOSED_ANGLE 170
#define SERVO_INTERVAL_MS 20 // Time in ms between angle updates for smooth movement



// =========================
// BOTTOM ULTRASOUND CONFIG
// =========================

#define BOTTOM_ULTRASONIC_TRIG_PIN A0
#define BOTTOM_ULTRASONIC_ECHO_PIN A1

// =========================
// TOP ULTRASOUND CONFIG
// =========================

#define TOP_ULTRASONIC_TRIG_PIN A3
#define TOP_ULTRASONIC_ECHO_PIN A2

// =========================
// MOTOR CONFIG
// =========================
#define LEFT_PWM_PIN 3
#define LEFT_DIR_PIN 4
#define RIGHT_PWM_PIN 6
#define RIGHT_DIR_PIN 5




#endif