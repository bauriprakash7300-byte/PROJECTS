/**
 ******************************************************************************
 * @file    motor.h
 * @brief   Motor driver abstraction: direction control + PWM duty output.
 ******************************************************************************
 */

#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
    MOTOR_DIR_FORWARD = 0,
    MOTOR_DIR_REVERSE,
    MOTOR_DIR_BRAKE
} Motor_DirectionTypeDef;

/**
 * @brief  Start the PWM timer channel used for motor drive.
 *         Call once during system init.
 */
void Motor_Init(void);

/**
 * @brief  Set motor rotation direction via the H-bridge IN1/IN2 pins.
 */
void Motor_SetDirection(Motor_DirectionTypeDef direction);

/**
 * @brief  Set PWM duty cycle (0 .. PWM_MAX_DUTY) driving the motor.
 */
void Motor_SetDutyCycle(uint16_t duty);

/**
 * @brief  Immediately stop the motor (duty = 0, brake).
 */
void Motor_Stop(void);

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_H */
