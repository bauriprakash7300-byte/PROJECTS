/**
 ******************************************************************************
 * @file    motor.c
 * @brief   Motor driver abstraction implementation (generic H-bridge).
 ******************************************************************************
 */

#include "motor.h"
#include "app_config.h"

extern TIM_HandleTypeDef PWM_TIMER_HANDLE;

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&PWM_TIMER_HANDLE, PWM_CHANNEL);
    Motor_SetDirection(MOTOR_DIR_FORWARD);
    Motor_SetDutyCycle(0);
}

void Motor_SetDirection(Motor_DirectionTypeDef direction)
{
    switch (direction)
    {
        case MOTOR_DIR_FORWARD:
            HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
            break;

        case MOTOR_DIR_REVERSE:
            HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_SET);
            break;

        case MOTOR_DIR_BRAKE:
        default:
            HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
            break;
    }
}

void Motor_SetDutyCycle(uint16_t duty)
{
    if (duty > PWM_MAX_DUTY)
    {
        duty = PWM_MAX_DUTY;
    }

    __HAL_TIM_SET_COMPARE(&PWM_TIMER_HANDLE, PWM_CHANNEL, duty);
}

void Motor_Stop(void)
{
    Motor_SetDutyCycle(0);
    Motor_SetDirection(MOTOR_DIR_BRAKE);
}
