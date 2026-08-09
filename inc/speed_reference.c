/**
 ******************************************************************************
 * @file    speed_reference.c
 * @brief   Potentiometer -> ADC -> RPM setpoint conversion.
 ******************************************************************************
 */

#include "speed_reference.h"
#include "app_config.h"

extern ADC_HandleTypeDef ADC_HANDLE;

void SpeedReference_Init(void)
{
    HAL_ADCEx_Calibration_Start(&ADC_HANDLE);
}

uint16_t SpeedReference_GetSetpointRPM(void)
{
    uint32_t raw = 0;

    HAL_ADC_Start(&ADC_HANDLE);
    if (HAL_ADC_PollForConversion(&ADC_HANDLE, 10) == HAL_OK)
    {
        raw = HAL_ADC_GetValue(&ADC_HANDLE);
    }
    HAL_ADC_Stop(&ADC_HANDLE);

    uint32_t rpm = (raw * SPEED_REF_MAX_RPM) / ADC_RESOLUTION_MAX;

    if (rpm < SPEED_REF_DEADBAND_RPM)
    {
        rpm = 0;
    }

    return (uint16_t)rpm;
}
