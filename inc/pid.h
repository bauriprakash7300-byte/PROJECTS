/**
 ******************************************************************************
 * @file    pid.h
 * @brief   Simple discrete PID controller with anti-windup clamping.
 ******************************************************************************
 */

#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float kp;
    float ki;
    float kd;

    float integral;
    float integral_min;
    float integral_max;

    float prev_error;

    float out_min;
    float out_max;

    float sample_time_s;
} PID_HandleTypeDef;

/**
 * @brief  Initialize a PID controller instance.
 * @param  pid           Pointer to the PID handle.
 * @param  kp, ki, kd    Controller gains.
 * @param  out_min/max   Output saturation limits (PWM duty range).
 * @param  i_min/max     Integral term clamp (anti-windup).
 * @param  sample_time_s Fixed control loop period in seconds.
 */
void PID_Init(PID_HandleTypeDef *pid,
              float kp, float ki, float kd,
              float out_min, float out_max,
              float i_min, float i_max,
              float sample_time_s);

/**
 * @brief  Reset the integral and derivative history (call on setpoint
 *         jumps or when re-enabling the controller after a stop).
 */
void PID_Reset(PID_HandleTypeDef *pid);

/**
 * @brief  Run one PID iteration.
 * @param  pid      Pointer to the PID handle.
 * @param  setpoint Desired value (e.g. target RPM).
 * @param  measured Measured value (e.g. actual RPM).
 * @retval Controller output, clamped to [out_min, out_max].
 */
float PID_Compute(PID_HandleTypeDef *pid, float setpoint, float measured);

#ifdef __cplusplus
}
#endif

#endif /* PID_H */
