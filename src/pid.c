/**
 ******************************************************************************
 * @file    pid.c
 * @brief   Discrete PID controller implementation.
 ******************************************************************************
 */

#include "pid.h"

void PID_Init(PID_HandleTypeDef *pid,
              float kp, float ki, float kd,
              float out_min, float out_max,
              float i_min, float i_max,
              float sample_time_s)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->out_min = out_min;
    pid->out_max = out_max;

    pid->integral_min = i_min;
    pid->integral_max = i_max;

    pid->sample_time_s = sample_time_s;

    PID_Reset(pid);
}

void PID_Reset(PID_HandleTypeDef *pid)
{
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}

float PID_Compute(PID_HandleTypeDef *pid, float setpoint, float measured)
{
    float error = setpoint - measured;

    /* Integral term with anti-windup clamping */
    pid->integral += error * pid->sample_time_s;
    if (pid->integral > pid->integral_max)
    {
        pid->integral = pid->integral_max;
    }
    else if (pid->integral < pid->integral_min)
    {
        pid->integral = pid->integral_min;
    }

    /* Derivative term (on error) */
    float derivative = (error - pid->prev_error) / pid->sample_time_s;
    pid->prev_error = error;

    float output = (pid->kp * error) +
                    (pid->ki * pid->integral) +
                    (pid->kd * derivative);

    /* Output saturation */
    if (output > pid->out_max)
    {
        output = pid->out_max;
    }
    else if (output < pid->out_min)
    {
        output = pid->out_min;
    }

    return output;
}
