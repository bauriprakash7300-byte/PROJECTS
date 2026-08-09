/**
 ******************************************************************************
 * @file    encoder.h
 * @brief   Quadrature encoder reading via STM32 timer encoder mode,
 *          plus RPM calculation.
 ******************************************************************************
 */

#ifndef ENCODER_H
#define ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief  Start the timer in encoder mode. Call once during system init.
 */
void Encoder_Init(void);

/**
 * @brief  Compute motor speed in RPM based on the encoder count
 *         accumulated since the last call.
 *
 *         Must be called on a fixed period equal to
 *         ENCODER_SAMPLE_TIME_MS for the RPM math to be valid.
 *
 * @retval Current motor speed in RPM (signed, direction included).
 */
float Encoder_GetRPM(void);

#ifdef __cplusplus
}
#endif

#endif /* ENCODER_H */
