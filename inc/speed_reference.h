/**
 ******************************************************************************
 * @file    speed_reference.h
 * @brief   Reads the potentiometer via ADC and converts it into a
 *          desired motor speed setpoint (RPM).
 ******************************************************************************
 */

#ifndef SPEED_REFERENCE_H
#define SPEED_REFERENCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief  Calibrate/start the ADC used for the speed reference.
 *         Call once during system init.
 */
void SpeedReference_Init(void);

/**
 * @brief  Read the ADC and convert the raw value into a target RPM
 *         between SPEED_REF_MIN_RPM and SPEED_REF_MAX_RPM.
 * @retval Desired motor speed in RPM.
 */
uint16_t SpeedReference_GetSetpointRPM(void);

#ifdef __cplusplus
}
#endif

#endif /* SPEED_REFERENCE_H */
