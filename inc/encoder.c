/**
 ******************************************************************************
 * @file    encoder.c
 * @brief   Quadrature encoder + RPM calculation implementation.
 ******************************************************************************
 */

#include "encoder.h"
#include "app_config.h"

extern TIM_HandleTypeDef ENCODER_TIMER_HANDLE;

static int16_t last_count = 0;

void Encoder_Init(void)
{
    HAL_TIM_Encoder_Start(&ENCODER_TIMER_HANDLE, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&ENCODER_TIMER_HANDLE, 0);
    last_count = 0;
}

float Encoder_GetRPM(void)
{
    int16_t current_count = (int16_t)__HAL_TIM_GET_COUNTER(&ENCODER_TIMER_HANDLE);
    int16_t delta = (int16_t)(current_count - last_count);
    last_count = current_count;

    /* RPM = (counts * 60) / (PPR * sample_time_seconds) */
    float sample_time_s = ENCODER_SAMPLE_TIME_MS / 1000.0f;
    float rpm = ((float)delta * 60.0f) / ((float)ENCODER_PPR * sample_time_s);

    return rpm;
}
