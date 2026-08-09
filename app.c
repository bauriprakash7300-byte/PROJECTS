/**
 ******************************************************************************
 * @file    app.c
 * @brief   Application layer: FreeRTOS task creation and the Control /
 *          Input / Monitor task bodies described in the README.
 *
 *          Control Task   (10 ms,  Above Normal) - PID speed control
 *          Input  Task    (50 ms,  Normal)        - Reads speed reference
 *          Monitor Task   (500 ms, Low)            - UART status output
 ******************************************************************************
 */

#include "app.h"
#include "app_config.h"
#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "speed_reference.h"

#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef DEBUG_UART_HANDLE;

/* Shared state between tasks. Kept simple (single writer / single
 * reader per field) so a mutex is not required; upgrade to a mutex or
 * queue if you add more producers/consumers. */
static volatile uint16_t g_setpoint_rpm = 0;
static volatile float    g_actual_rpm   = 0.0f;
static volatile uint16_t g_pwm_duty     = 0;

static PID_HandleTypeDef g_pid;

static osThreadId_t g_controlTaskHandle;
static osThreadId_t g_inputTaskHandle;
static osThreadId_t g_monitorTaskHandle;

static void ControlTask(void *argument);
static void InputTask(void *argument);
static void MonitorTask(void *argument);

void App_Init(void)
{
    Motor_Init();
    Encoder_Init();
    SpeedReference_Init();

    PID_Init(&g_pid,
              PID_KP, PID_KI, PID_KD,
              PID_OUTPUT_MIN, PID_OUTPUT_MAX,
              PID_INTEGRAL_MIN, PID_INTEGRAL_MAX,
              CONTROL_TASK_PERIOD_MS / 1000.0f);

    const osThreadAttr_t controlAttr = {
        .name = "ControlTask",
        .priority = (osPriority_t) CONTROL_TASK_PRIORITY,
        .stack_size = CONTROL_TASK_STACK_SIZE * 4U
    };
    const osThreadAttr_t inputAttr = {
        .name = "InputTask",
        .priority = (osPriority_t) INPUT_TASK_PRIORITY,
        .stack_size = INPUT_TASK_STACK_SIZE * 4U
    };
    const osThreadAttr_t monitorAttr = {
        .name = "MonitorTask",
        .priority = (osPriority_t) MONITOR_TASK_PRIORITY,
        .stack_size = MONITOR_TASK_STACK_SIZE * 4U
    };

    g_controlTaskHandle = osThreadNew(ControlTask, NULL, &controlAttr);
    g_inputTaskHandle   = osThreadNew(InputTask, NULL, &inputAttr);
    g_monitorTaskHandle = osThreadNew(MonitorTask, NULL, &monitorAttr);
}

/**
 * @brief  10 ms task: reads the encoder, runs PID, and updates PWM.
 *         Stops the motor when the setpoint is within the deadband.
 */
static void ControlTask(void *argument)
{
    (void)argument;
    TickType_t lastWake = xTaskGetTickCount();

    for (;;)
    {
        uint16_t setpoint = g_setpoint_rpm;
        float actual = Encoder_GetRPM();
        g_actual_rpm = actual;

        if (setpoint == 0U)
        {
            Motor_Stop();
            PID_Reset(&g_pid);
            g_pwm_duty = 0;
        }
        else
        {
            float output = PID_Compute(&g_pid, (float)setpoint, actual);
            uint16_t duty = (uint16_t)output;

            Motor_SetDirection(MOTOR_DIR_FORWARD);
            Motor_SetDutyCycle(duty);
            g_pwm_duty = duty;
        }

        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(CONTROL_TASK_PERIOD_MS));
    }
}

/**
 * @brief  50 ms task: reads the potentiometer and updates the
 *         shared speed setpoint used by ControlTask.
 */
static void InputTask(void *argument)
{
    (void)argument;
    TickType_t lastWake = xTaskGetTickCount();

    for (;;)
    {
        g_setpoint_rpm = SpeedReference_GetSetpointRPM();

        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(INPUT_TASK_PERIOD_MS));
    }
}

/**
 * @brief  500 ms task: prints setpoint / actual speed / duty / error
 *         over UART for debugging and tuning.
 */
static void MonitorTask(void *argument)
{
    (void)argument;
    TickType_t lastWake = xTaskGetTickCount();
    char line[96];

    for (;;)
    {
        uint16_t setpoint = g_setpoint_rpm;
        float actual = g_actual_rpm;
        uint16_t duty = g_pwm_duty;
        float error = (float)setpoint - actual;

        int len = snprintf(line, sizeof(line),
                             "Setpoint=%u RPM, Speed=%d RPM, Duty=%u, Error=%d\r\n",
                             (unsigned)setpoint,
                             (int)actual,
                             (unsigned)duty,
                             (int)error);

        if (len > 0)
        {
            HAL_UART_Transmit(&DEBUG_UART_HANDLE, (uint8_t *)line,
                               (uint16_t)len, 100);
        }

        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(MONITOR_TASK_PERIOD_MS));
    }
}
