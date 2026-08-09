/**
 ******************************************************************************
 * @file    app_config.h
 * @brief   Central configuration for pins, timers, PID gains, and
 *          FreeRTOS task periods / priorities.
 *
 *          Edit the values in this file to match your hardware
 *          (motor driver, encoder PPR, timer clock, etc.) instead of
 *          hunting through every source file.
 ******************************************************************************
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* ---------------------------------------------------------------------- */
/* PWM (TIM2 CH1 - PA0)                                                    */
/* ---------------------------------------------------------------------- */
#define PWM_TIMER_HANDLE            htim2
#define PWM_CHANNEL                 TIM_CHANNEL_1
#define PWM_MAX_DUTY                (3599U)   /* Matches TIM2 ARR value    */
#define PWM_MIN_DUTY                (0U)

/* ---------------------------------------------------------------------- */
/* Encoder (TIM3 CH1/CH2 - PA6/PA7)                                       */
/* ---------------------------------------------------------------------- */
#define ENCODER_TIMER_HANDLE        htim3
#define ENCODER_PPR                 (330U)    /* Counts per revolution.
                                                   Change to match your
                                                   encoder datasheet.      */
#define ENCODER_SAMPLE_TIME_MS      (10U)     /* Must match control task
                                                   period below.           */

/* ---------------------------------------------------------------------- */
/* ADC (ADC1 IN1 - PA1) Speed reference potentiometer                     */
/* ---------------------------------------------------------------------- */
#define ADC_HANDLE                  hadc1
#define ADC_RESOLUTION_MAX           (4095U)  /* 12-bit ADC                */
#define SPEED_REF_MIN_RPM            (0U)
#define SPEED_REF_MAX_RPM            (3000U)  /* Adjust to your motor's
                                                   rated speed.            */
#define SPEED_REF_DEADBAND_RPM       (30U)    /* Below this, motor stops. */

/* ---------------------------------------------------------------------- */
/* UART (USART1 - PA9/PA10)                                                */
/* ---------------------------------------------------------------------- */
#define DEBUG_UART_HANDLE           huart1

/* ---------------------------------------------------------------------- */
/* Motor driver direction pins                                            */
/* ---------------------------------------------------------------------- */
#define MOTOR_IN1_GPIO_Port         GPIOB
#define MOTOR_IN1_Pin               GPIO_PIN_0
#define MOTOR_IN2_GPIO_Port         GPIOB
#define MOTOR_IN2_Pin               GPIO_PIN_1

/* ---------------------------------------------------------------------- */
/* PID gains - starting point only, must be tuned on real hardware        */
/* ---------------------------------------------------------------------- */
#define PID_KP                      (0.60f)
#define PID_KI                      (0.35f)
#define PID_KD                      (0.02f)
#define PID_OUTPUT_MIN              (0.0f)
#define PID_OUTPUT_MAX              ((float)PWM_MAX_DUTY)
#define PID_INTEGRAL_MIN            (-500.0f)
#define PID_INTEGRAL_MAX            (500.0f)

/* ---------------------------------------------------------------------- */
/* FreeRTOS task periods (ms) and priorities                              */
/* ---------------------------------------------------------------------- */
#define CONTROL_TASK_PERIOD_MS      (10U)
#define INPUT_TASK_PERIOD_MS        (50U)
#define MONITOR_TASK_PERIOD_MS      (500U)

#define CONTROL_TASK_PRIORITY       (osPriorityAboveNormal)
#define INPUT_TASK_PRIORITY         (osPriorityNormal)
#define MONITOR_TASK_PRIORITY       (osPriorityLow)

#define CONTROL_TASK_STACK_SIZE     (256U)  /* words */
#define INPUT_TASK_STACK_SIZE       (192U)
#define MONITOR_TASK_STACK_SIZE     (256U)

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
