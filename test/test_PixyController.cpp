#include <unity.h>
#include "sensors/mocks/MockPixySensor.h"
#include "controllers/sensors/PixyController.h"
#include "constants/Pixy.h"

// Test fixture
PixyController controller;
MockPixySensor mockSensor;

void setUp(void)
{
    controller.init(&mockSensor, 1, 40, 150, 40, 150, 20, 20);
}

void tearDown(void)
{
    mockSensor.clearBlocks();
    mockSensor.setInitFails(false);
}

// ============================================================================
// Test: Controller Initialization
// ============================================================================

void test_PixyController_InitWithValidSensor(void)
{
    MockPixySensor sensor;
    int8_t result = controller.init(&sensor, 1, 40, 150, 40, 150, 20, 20);
    
    TEST_ASSERT_EQUAL_INT8(0, result);
}

void test_PixyController_InitWithNullSensor(void)
{
    PixyController ctrl;
    int8_t result = ctrl.init(nullptr, 1, 40, 150, 40, 150, 20, 20);
    
    TEST_ASSERT_EQUAL_INT8(-1, result);
}

void test_PixyController_InitFailsWhenSensorInitFails(void)
{
    PixyController ctrl;
    MockPixySensor failingSensor;
    failingSensor.setInitFails(true);
    
    int8_t result = ctrl.init(&failingSensor, 1, 40, 150, 40, 150, 20, 20);
    
    TEST_ASSERT_EQUAL_INT8(-1, result);
}

// ============================================================================
// Test: Ball Detection - Single Ball Cases
// ============================================================================

void test_PixyController_FindsRedBallInCenter(void)
{
    mockSensor.clearBlocks();
    mockSensor.addBlock({
        .x = 160,           // Center of 320-wide screen
        .y = 100,           // Center of 200-high screen
        .width = 45,
        .height = 48,
        .signature = 1,     // Red
        .area = 2160,
        .age = 5,
        .index = 0,
        .angle = 0
    });
    
    Types::DetectedBlock found = controller.findTargetBall();
    
    TEST_ASSERT_EQUAL_UINT16(160, found.x);
    TEST_ASSERT_EQUAL_UINT8(1, found.signature);
}

void test_PixyController_IgnoresWrongColorSignature(void)
{
    mockSensor.clearBlocks();
    mockSensor.addBlock({
        .x = 160,
        .y = 100,
        .width = 45,
        .height = 48,
        .signature = 2,     // Green, but looking for Red (sig 1)
        .area = 2160,
        .age = 5,
        .index = 0,
        .angle = 0
    });
    
    Types::DetectedBlock found = controller.findTargetBall();
    
    TEST_ASSERT_EQUAL_UINT8(0, found.signature);  // Empty block
}

void test_PixyController_IgnoresTooSmallBall(void)
{
    mockSensor.clearBlocks();
    mockSensor.addBlock({
        .x = 160,
        .y = 100,
        .width = 10,        // Too small (threshold is 40)
        .height = 10,
        .signature = 1,
        .area = 100,
        .age = 5,
        .index = 0,
        .angle = 0
    });
    
    Types::DetectedBlock found = controller.findTargetBall();
    
    TEST_ASSERT_EQUAL_UINT8(0, found.signature);  // Empty block
}

// void test_PixyController_IgnoresBallTooFarOffscreen(void)
// {
//     mockSensor.clearBlocks();
//     mockSensor.addBlock({
//         .x = 290,           // Too far right (center is 160, threshold is 100)
//         .y = 100,
//         .width = 45,
//         .height = 48,
//         .signature = 1,
//         .area = 2160,
//         .age = 5,
//         .index = 0,
//         .angle = 0
//     });
    
//     Types::DetectedBlock found = controller.findTargetBall();
    
//     TEST_ASSERT_EQUAL_UINT8(0, found.signature);  // Empty block
// }

void test_PixyController_FindsBallAtEdgeInBounds(void)
{
    mockSensor.clearBlocks();
    mockSensor.addBlock({
        .x = 60,            // Just within threshold (160 - 100 = 60)
        .y = 100,
        .width = 45,
        .height = 48,
        .signature = 1,
        .area = 2160,
        .age = 5,
        .index = 0,
        .angle = 0
    });
    
    Types::DetectedBlock found = controller.findTargetBall();
    
    TEST_ASSERT_EQUAL_UINT16(60, found.x);
    TEST_ASSERT_EQUAL_UINT8(1, found.signature);
}

// ============================================================================
// Test: Multiple Ball Cases
// ============================================================================

void test_PixyController_FindsFirstValidBallFromMultiple(void)
{
    mockSensor.clearBlocks();
    
    // Invalid ball (wrong color)
    mockSensor.addBlock({
        .x = 100, .y = 100, .width = 50, .height = 50,
        .signature = 2, .area = 2500, .age = 5, .index = 0, .angle = 0
    });
    
    // Valid red ball
    mockSensor.addBlock({
        .x = 160, .y = 100, .width = 45, .height = 48,
        .signature = 1, .area = 2160, .age = 5, .index = 1, .angle = 0
    });
    
    Types::DetectedBlock found = controller.findTargetBall();
    
    TEST_ASSERT_EQUAL_UINT8(1, found.signature);  // Found the red one
    TEST_ASSERT_EQUAL_UINT8(1, found.index);      // It's the second ball
}

// ============================================================================
// Test: Current Target Ball Tracking
// ============================================================================

void test_PixyController_GetCurrentTargetBall(void)
{
    mockSensor.clearBlocks();
    mockSensor.addBlock({
        .x = 160, .y = 100, .width = 45, .height = 48,
        .signature = 1, .area = 2160, .age = 5, .index = 0, .angle = 0
    });
    
    Types::DetectedBlock found = controller.findTargetBall();
    Types::DetectedBlock current = controller.getCurrentTargetBall();
    
    TEST_ASSERT_EQUAL_UINT16(found.x, current.x);
    TEST_ASSERT_EQUAL_UINT8(found.signature, current.signature);
}

void test_PixyController_ResetCurrentTargetBall(void)
{
    mockSensor.clearBlocks();
    mockSensor.addBlock({
        .x = 160, .y = 100, .width = 45, .height = 48,
        .signature = 1, .area = 2160, .age = 5, .index = 0, .angle = 0
    });
    
    controller.findTargetBall();
    int8_t result = controller.resetCurrentTargetBall();
    
    TEST_ASSERT_EQUAL_INT8(0, result);
    
    // After reset, current should return invalid
    Types::DetectedBlock current = controller.getCurrentTargetBall();
    TEST_ASSERT_EQUAL_UINT8(0, current.signature);
}

// ============================================================================
// Test: No Balls Detected
// ============================================================================

void test_PixyController_ReturnsEmptyBlockWhenNoBallsDetected(void)
{
    mockSensor.useSampleData_NoBalls();
    
    Types::DetectedBlock found = controller.findTargetBall();
    
    TEST_ASSERT_EQUAL_UINT8(0, found.signature);
}

void test_PixyController_HandlesEmptyBlockData(void)
{
    mockSensor.useSampleData_NoBalls();
    
    uint8_t count = 0;
    const Types::DetectedBlock *blocks = mockSensor.getBlocks(count);
    
    TEST_ASSERT_EQUAL_UINT8(0, count);
    TEST_ASSERT_NULL(blocks);
}


int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_PixyController_InitWithValidSensor);
  RUN_TEST(test_PixyController_InitWithNullSensor);
  RUN_TEST(test_PixyController_InitFailsWhenSensorInitFails);
  RUN_TEST(test_PixyController_FindsRedBallInCenter);
  RUN_TEST(test_PixyController_IgnoresWrongColorSignature);
  RUN_TEST(test_PixyController_IgnoresTooSmallBall);
//   RUN_TEST(test_PixyController_IgnoresBallTooFarOffscreen);
  return UNITY_END();
}

// WARNING!!! PLEASE REMOVE UNNECESSARY MAIN IMPLEMENTATIONS //

/**
  * For native dev-platform or for some embedded frameworks
  */
int main(void) {
  return runUnityTests();
}
