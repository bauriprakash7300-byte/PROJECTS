# RTOS-Based DC Motor Speed Controller

### STM32 | Embedded C | FreeRTOS | PWM | PID | Encoder | UART

A real-time closed-loop DC motor speed controller developed using an STM32 microcontroller, Embedded C, FreeRTOS, PWM, encoder feedback, and PID control.

The system controls the speed of a DC motor according to a user-defined speed reference. The STM32 generates PWM through a hardware timer, measures motor speed using an incremental encoder, and dynamically adjusts the PWM duty cycle using a PID controller. FreeRTOS is used to manage time-critical control, input acquisition, and monitoring tasks independently.

---

## Project Overview

The objective of this project is to design and implement a reliable real-time DC motor speed control system capable of maintaining the desired motor speed under varying operating conditions.

The STM32 performs the following operations:

- Reads the desired motor speed using an ADC-based potentiometer
- Measures actual motor speed using an encoder
- Calculates speed error
- Executes a PID control algorithm
- Generates PWM using a hardware timer
- Controls the motor through an external motor driver
- Uses FreeRTOS for real-time task scheduling
- Sends real-time system information through UART

---

## System Architecture

```text
                     +----------------------+
                     |   Speed Reference    |
                     |   Potentiometer      |
                     +----------+-----------+
                                |
                               ADC
                                |
                                v
                     +----------------------+
                     |        STM32         |
                     |                      |
                     |      FreeRTOS        |
                     |                      |
                     |  +----------------+  |
                     |  | Input Task     |  |
                     |  +-------+--------+  |
                     |          |           |
                     |          v           |
                     |  +----------------+  |
                     |  | Control Task   |  |
                     |  |                |  |
                     |  | PID Controller |  |
                     |  +-------+--------+  |
                     |          |           |
                     |         PWM          |
                     +----------+-----------+
                                |
                                v
                     +----------------------+
                     |    Motor Driver      |
                     +----------+-----------+
                                |
                                v
                         +-------------+
                         |  DC Motor   |
                         +------+------+
                                |
                             Encoder
                                |
                                v
                     +----------------------+
                     | Speed Measurement    |
                     +----------+-----------+
                                |
                                +-----------> STM32

                     +----------------------+
                     |    Monitor Task      |
                     |   UART Debugging     |
                     +----------------------+
