/**
 ******************************************************************************
 * @file    app.h
 * @brief   Application entry point: creates the FreeRTOS tasks that
 *          implement input acquisition, closed-loop control, and
 *          UART monitoring.
 ******************************************************************************
 */

#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize peripherals (motor, encoder, ADC) and create the
 *         Control, Input, and Monitor FreeRTOS tasks.
 *
 *         Call this once from main(), after MX_*_Init() calls and
 *         before osKernelStart().
 */
void App_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_H */
