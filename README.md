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
```

---

## Hardware Requirements

| Component               |    Quantity | Purpose                       |
| ----------------------- | ----------: | ------------------------------ |
| STM32F103C8T6 Blue Pill |           1 | Main microcontroller          |
| DC Motor with Encoder   |           1 | Motor and speed feedback      |
| Motor Driver            |           1 | Motor power/control interface |
| 10 kΩ Potentiometer     |           1 | Speed reference               |
| ST-Link V2              |           1 | Programming and debugging     |
| USB-TTL Converter       |           1 | UART monitoring               |
| DC Power Supply         |           1 | Motor power                   |
| Breadboard / PCB        |           1 | Hardware implementation       |
| Jumper Wires            | As required | Hardware connections          |

### Recommended Motor Drivers

* TB6612FNG
* L298N
* BTS7960

The motor driver should be selected according to the motor voltage and current requirements.

> **Important:** Never connect a DC motor directly to an STM32 GPIO pin.

---

## Pin Configuration

The reference implementation uses the STM32F103C8T6.

| STM32 Pin | Peripheral  | Function        |
| --------- | ----------- | --------------- |
| PA0       | TIM2_CH1    | PWM Output      |
| PA1       | ADC1_IN1    | Speed Reference |
| PA6       | TIM3_CH1    | Encoder A       |
| PA7       | TIM3_CH2    | Encoder B       |
| PA9       | USART1_TX   | UART Transmit   |
| PA10      | USART1_RX   | UART Receive    |
| PB0       | GPIO Output | Motor IN1       |
| PB1       | GPIO Output | Motor IN2       |
| PC13      | GPIO Output | Status LED      |

---

## Motor Driver Connection

Generic H-bridge motor-driver connection:

```text
STM32                 Motor Driver
-----------------------------------
PA0  ----------------> PWM / ENA
PB0  ----------------> IN1
PB1  ----------------> IN2
GND  ----------------> GND

Motor Driver          DC Motor
-----------------------------------
OUT1 ----------------> Motor +
OUT2 ----------------> Motor -

External Supply -----> Motor Driver VM
```

The motor should be powered from a separate suitable power supply.

---

## Encoder Connection

```text
Encoder                 STM32
--------------------------------
Encoder A ------------> PA6
Encoder B ------------> PA7
Encoder GND ----------> GND
Encoder VCC ----------> Appropriate supply
```

Check the encoder datasheet for its operating voltage and output type before connecting it to the STM32.

---

## Potentiometer Connection

```text
                    +3.3V
                      |
                      |
                +-----------+
                |   POT     |
                +-----------+
                      |
                      +-----------> PA1 / ADC
                      |
                     GND
```

The potentiometer position is converted into a desired motor speed.

Example:

```text
0% potentiometer    → Minimum speed
25%                 → Low speed
50%                 → Medium speed
75%                 → High speed
100%                → Maximum configured speed
```

---

## Software Architecture

The firmware is divided into modular components.

```text
App/
│
├── Inc/
│   ├── app.h
│   ├── app_config.h
│   ├── pid.h
│   ├── motor.h
│   ├── encoder.h
│   └── speed_reference.h
│
└── Src/
    ├── app.c
    ├── pid.c
    ├── motor.c
    ├── encoder.c
    └── speed_reference.c
```

---

## FreeRTOS Task Architecture

The application uses three main FreeRTOS tasks.

### 1. Control Task

**Period:** 10 ms

Responsibilities:

* Read encoder feedback
* Calculate actual motor RPM
* Calculate speed error
* Execute PID controller
* Update PWM duty cycle
* Stop motor when speed reference is near zero

```text
Encoder
   |
   v
RPM Calculation
   |
   v
PID Controller
   |
   v
PWM Duty Cycle
   |
   v
Motor Driver
```

---

### 2. Input Task

**Period:** 50 ms

Responsibilities:

* Read ADC value
* Convert ADC value into desired RPM
* Update motor speed reference

```text
Potentiometer
      |
      v
     ADC
      |
      v
Speed Setpoint
```

---

### 3. Monitor Task

**Period:** 500 ms

Responsibilities:

* Send debugging information through UART
* Display desired speed
* Display measured speed
* Display PWM duty cycle
* Display speed error

Example:

```text
RTOS DC Motor Controller

Setpoint=1500 RPM
Speed=1432 RPM
Duty=61%
Error=68 RPM
```

---

## Why FreeRTOS?

A simple implementation could execute everything inside one infinite loop:

```c
while (1)
{
    Read_Speed();
    Control_Motor();
    Read_Input();
    Send_UART();
}
```

However, this approach becomes difficult to manage when multiple operations have different timing requirements.

FreeRTOS allows these operations to be separated into independent tasks:

```text
                 FreeRTOS
                    |
       +------------+------------+
       |            |            |
       v            v            v
 Input Task    Control Task   Monitor Task
   50 ms          10 ms          500 ms
       |            |            |
       v            v            v
      ADC          PID          UART
                    |
                   PWM
```

This provides:

* Task prioritization
* Periodic execution
* Better modularity
* Predictable task scheduling
* Improved real-time responsiveness

---

## Task Priorities

| Task         | Period | Priority     | Function            |
| ------------ | -----: | ------------ | -------------------- |
| Control Task |  10 ms | Above Normal | Motor speed control |
| Input Task   |  50 ms | Normal       | Speed reference     |
| Monitor Task | 500 ms | Low          | UART monitoring     |

The control task receives a higher priority because motor control is time-sensitive.

---

## PID Control

The controller calculates the difference between the desired and measured motor speed.

```text
Speed Error = Desired Speed - Actual Speed
```

The PID controller calculates the required control output using three terms:

```text
PID Output = Proportional
           + Integral
           + Derivative
```

Mathematically:

```text
u(t) = Kp × e(t)
     + Ki × ∫e(t)dt
     + Kd × de(t)/dt
```

Where:

* `Kp` = Proportional gain
* `Ki` = Integral gain
* `Kd` = Derivative gain
* `e(t)` = Speed error
* `u(t)` = Controller output

The PID output is converted into a PWM duty cycle.

---

## PWM Motor Control

The STM32 hardware timer generates the PWM signal.

The duty cycle determines the motor-drive command.

```text
Lower Duty Cycle
       |
       v
Lower Motor Speed


Higher Duty Cycle
       |
       v
Higher Motor Speed
```

Example:

```text
25% Duty Cycle → Low Speed
50% Duty Cycle → Medium Speed
75% Duty Cycle → High Speed
90% Duty Cycle → Near Maximum Speed
```

The PWM signal is sent to the motor driver.

---

## Closed-Loop Speed Control

The project uses encoder feedback to continuously correct motor speed.

```text
             Desired Speed
                   |
                   v
              +---------+
              |   PID   |
              +----+----+
                   |
                  PWM
                   |
                   v
              Motor Driver
                   |
                   v
                 Motor
                   |
                Encoder
                   |
                   v
              Actual RPM
                   |
                   +-----------> PID
```

### Working Principle

If:

```text
Desired Speed > Actual Speed
```

the controller increases the PWM command.

If:

```text
Desired Speed < Actual Speed
```

the controller decreases the PWM command.

This continuous feedback allows the system to maintain the desired motor speed more accurately.

---

## RPM Calculation

The encoder generates pulses as the motor rotates.

The motor speed can be calculated from the encoder count over a fixed sampling period.

```text
RPM = (Encoder Counts × 60)
      ----------------------
        PPR × Sample Time
```

Where:

* `Encoder Counts` = Encoder count during the sampling period
* `PPR` = Encoder pulses/counts per revolution according to the selected configuration
* `Sample Time` = Measurement interval in seconds

> Encoder manufacturers may specify PPR, CPR, or counts/revolution differently. The value must be matched to the actual encoder and STM32 timer decoding mode.

---

## STM32CubeMX Configuration

### Microcontroller

```text
STM32F103C8T6
```

### System Clock

```text
SYSCLK = 72 MHz
```

The exact clock configuration depends on the board and oscillator.

---

### USART1 Configuration

```text
Mode          : Asynchronous
Baud Rate     : 115200
Word Length   : 8 Bits
Parity        : None
Stop Bits     : 1
Flow Control  : None
```

Pins:

```text
PA9  → USART1_TX
PA10 → USART1_RX
```

---

### TIM2 PWM Configuration

Reference configuration:

```text
Mode           : PWM Generation CH1
Prescaler      : 0
Counter Period : 3599
```

For a 72 MHz timer clock:

```text
PWM Frequency = 72 MHz / 3600
              ≈ 20 kHz
```

The exact timer clock should be verified in STM32CubeMX because APB timer clock behavior depends on the clock configuration.

---

### TIM3 Encoder Configuration

```text
Mode:
Encoder Mode

Channel 1:
Encoder A

Channel 2:
Encoder B
```

Pins:

```text
PA6 → TIM3_CH1
PA7 → TIM3_CH2
```

---

### ADC Configuration

```text
ADC1
Channel: ADC1_IN1
Pin: PA1
Resolution: 12-bit
```

For a 12-bit ADC:

```text
ADC Range = 0 to 4095
```

The ADC value is converted into a speed reference.

---

### FreeRTOS Configuration

Enable FreeRTOS in STM32CubeMX/CubeIDE.

Create:

```text
ControlTask
InputTask
MonitorTask
```

Suggested configuration:

```text
ControlTask
Period: 10 ms
Priority: Above Normal

InputTask
Period: 50 ms
Priority: Normal

MonitorTask
Period: 500 ms
Priority: Low
```

---

## Project Structure

```text
RTOS-DC-Motor-Speed-Controller/
│
├── App/
│   ├── Inc/
│   │   ├── app.h
│   │   ├── app_config.h
│   │   ├── pid.h
│   │   ├── motor.h
│   │   ├── encoder.h
│   │   └── speed_reference.h
│   │
│   └── Src/
│       ├── app.c
│       ├── pid.c
│       ├── motor.c
│       ├── encoder.c
│       └── speed_reference.c
│
├── Core/
│   ├── Inc/
│   └── Src/
│
├── Drivers/
│
├── Docs/
│   ├── architecture.md
│   ├── wiring.md
│   ├── test_plan.md
│   └── interview_notes.md
│
├── README.md
├── LICENSE
└── .gitignore
```

---

## Development Environment

| Tool                 | Technology         |
| --------------------- | ------------------- |
| Microcontroller      | STM32F103C8T6      |
| Programming Language | Embedded C         |
| IDE                  | STM32CubeIDE       |
| RTOS                 | FreeRTOS           |
| HAL                  | STM32 HAL          |
| Motor Control        | PWM                |
| Feedback             | Quadrature Encoder |
| Control Algorithm    | PID                |
| Communication        | UART               |
| Debugger             | ST-Link V2         |

---

## Testing and Validation

The controller should be tested at multiple speed references.

Suggested test points:

```text
500 RPM
1000 RPM
1500 RPM
2000 RPM
2500 RPM
```

For each test, record:

* Desired RPM
* Actual RPM
* PWM Duty Cycle
* Speed Error
* Rise Time
* Overshoot
* Settling Time
* Steady-State Error

### Test Result Template

| Desired RPM | Actual RPM | PWM Duty | Error | Status  |
| ----------: | ---------: | -------: | ----: | ------- |
|         500 |        TBD |      TBD |   TBD | Pending |
|        1000 |        TBD |      TBD |   TBD | Pending |
|        1500 |        TBD |      TBD |   TBD | Pending |
|        2000 |        TBD |      TBD |   TBD | Pending |
|        2500 |        TBD |      TBD |   TBD | Pending |

> Replace `TBD` values with actual measurements from your hardware. Do not use simulated values as measured results.

---

## Performance Parameters

The following parameters can be used to evaluate the controller:

### Rise Time

Time required for the motor speed to reach the desired operating region after a speed command.

### Overshoot

Amount by which the motor speed exceeds the desired speed.

### Settling Time

Time required for the motor speed to remain within a specified error band around the setpoint.

### Steady-State Error

Difference between the desired and measured speed after the transient response has settled.

```text
Steady-State Error
=
Desired Speed - Measured Speed
```

---

## Debugging

UART is used to monitor the controller during development.

Example:

```text
RTOS DC Motor Controller

Setpoint=500 RPM, Speed=472 RPM, Duty=28%, Error=28
Setpoint=1000 RPM, Speed=963 RPM, Duty=42%, Error=37
Setpoint=1500 RPM, Speed=1452 RPM, Duty=58%, Error=48
```

Other debugging tools:

* STM32CubeIDE Debugger
* ST-Link V2
* Oscilloscope
* Logic Analyzer
* Multimeter
* UART Terminal

---

## Debugging Strategy

The project should be tested in stages.

### Stage 1 — STM32

```text
STM32
  ↓
LED Blink
```

Verify that the MCU is programmed correctly.

### Stage 2 — UART

```text
STM32
  ↓
UART
  ↓
PC
```

Verify serial communication.

### Stage 3 — PWM

```text
STM32
  ↓
PWM
  ↓
Oscilloscope
```

Verify:

* Frequency
* Duty cycle
* Waveform

### Stage 4 — Motor Driver

```text
STM32
  ↓
PWM
  ↓
Motor Driver
  ↓
Motor
```

Verify motor rotation.

### Stage 5 — Encoder

```text
Motor
  ↓
Encoder
  ↓
STM32
  ↓
RPM
```

Verify speed measurement.

### Stage 6 — Closed-Loop Control

```text
Setpoint
   ↓
PID
   ↓
PWM
   ↓
Motor
   ↓
Encoder
   ↓
RPM Feedback
   ↓
PID
```

Finally tune and validate the complete control loop.

---

## Safety Considerations

* Never connect the motor directly to an STM32 GPIO.
* Use a suitable motor driver.
* Use a separate motor power supply.
* Verify motor-driver voltage and current ratings.
* Verify encoder voltage levels.
* Ensure correct common-ground/reference connections.
* Use current limiting where appropriate.
* Keep motor power wiring separated from sensitive signal wiring where practical.
* Add emergency-stop and fault handling for a production system.
* Stop the motor if encoder feedback is lost in a real deployment.

---

## Future Improvements

* [ ] UART command-line interface
* [ ] CAN communication
* [ ] Current sensing
* [ ] Overcurrent protection
* [ ] Encoder fault detection
* [ ] Emergency-stop input
* [ ] OLED/LCD speed display
* [ ] DMA-based UART
* [ ] PID auto-tuning
* [ ] Data logging
* [ ] Hardware-in-the-loop testing
* [ ] Remote motor monitoring
* [ ] Web-based monitoring
* [ ] Motor temperature monitoring

---

## Learning Outcomes

### Embedded Systems

* STM32 Microcontroller Programming
* Embedded C
* STM32 HAL
* GPIO
* ADC
* Timers
* PWM
* UART
* Interrupts

### RTOS

* FreeRTOS
* Task Creation
* Task Scheduling
* Task Priorities
* Periodic Tasks
* Real-Time Control

### Control Systems

* Closed-Loop Control
* PID Controller
* Feedback Systems
* Error Calculation
* Motor Speed Regulation
* PID Parameter Tuning

### Debugging

* UART Debugging
* Oscilloscope-Based PWM Verification
* Encoder Signal Analysis
* Runtime Debugging
* Timing Analysis

---

## Interview Explanation

### 30-Second Explanation

> I developed a closed-loop DC motor speed controller using STM32, Embedded C, FreeRTOS, PWM and encoder feedback. The user provides the desired speed through a potentiometer, which is read using the ADC. The STM32 measures the actual motor speed using an encoder and calculates the speed error. A PID controller uses this error to adjust the PWM duty cycle supplied to the motor driver. I used FreeRTOS to divide the application into periodic tasks for speed control, input acquisition and monitoring. UART was used for debugging and monitoring parameters such as setpoint, actual RPM, PWM duty cycle and error.

---

## Common Interview Questions

### Why did you use STM32?

STM32 provides hardware timers, ADC, GPIO, UART, encoder interfaces and sufficient processing capability for real-time motor control.

### Why did you use PWM?

PWM provides an efficient method of controlling the average power delivered to the motor through the motor driver.

### Why can't you drive the motor directly from GPIO?

An STM32 GPIO cannot safely provide the current required by a DC motor. A motor driver is required to handle the motor's power requirements.

### Why did you use FreeRTOS?

The system contains multiple activities with different timing requirements. FreeRTOS allows these operations to be separated into independent tasks with appropriate priorities and execution periods.

### What is closed-loop control?

Closed-loop control uses feedback from the system output. In this project, the encoder measures actual motor speed and the controller adjusts PWM according to the difference between desired and actual speed.

### What is PID?

PID is a feedback control algorithm consisting of proportional, integral and derivative components. It uses the control error to calculate the actuator command.

### How did you calculate RPM?

RPM was calculated from the encoder count measured over a known sampling interval and the encoder counts-per-revolution configuration.

### How did you debug the project?

I used UART to monitor setpoint, measured RPM, PWM duty cycle and control error. I also used the STM32 debugger and oscilloscope/logic analyzer to verify firmware behavior and PWM signals.

### What improvements would you make?

I would add current sensing, encoder fault detection, emergency-stop handling, CAN communication, data logging and automated PID tuning.

---

## Key Technical Concepts

```text
                    STM32
                      |
          +-----------+-----------+
          |           |           |
         ADC        Timer       UART
          |           |           |
          |          PWM       Debugging
          |           |
          v           v
     Setpoint    Motor Driver
                     |
                     v
                   Motor
                     |
                  Encoder
                     |
                     v
                Speed Feedback
                     |
                     +----------+
                                |
                                v
                         PID Controller
                                |
                                v
                           PWM Update
```

---

## Expected System Behavior

When the user increases the speed reference:

```text
Speed Reference
      ↑
      |
      v
   ADC Value
      |
      v
 Setpoint RPM
      |
      v
  Speed Error
      |
      v
 PID Controller
      |
      v
 Increase PWM
      |
      v
 Motor Accelerates
      |
      v
 Encoder Feedback
      |
      v
 Actual RPM
```

The control loop continues until the actual motor speed approaches the desired speed.

---

## Future System Architecture

The project can be extended into a more advanced embedded motor-control platform:

```text
                +----------------+
                | PC / Dashboard |
                +-------+--------+
                        |
                     UART/CAN
                        |
                        v
                +---------------+
                |     STM32     |
                |               |
                |   FreeRTOS    |
                +-------+-------+
                        |
              +---------+---------+
              |                   |
              v                   v
        Motor Control        Diagnostics
              |
             PWM
              |
              v
        Motor Driver
              |
              v
            Motor
              |
           Encoder
              |
              +----------> STM32
```

---

## Project Highlights

* Real-time embedded motor control
* STM32-based firmware
* Embedded C implementation
* FreeRTOS-based multitasking
* Hardware timer-based PWM generation
* Encoder-based speed feedback
* PID closed-loop control
* ADC-based speed reference
* UART-based debugging
* Modular firmware architecture
* Real-time task scheduling
* Hardware-oriented debugging and validation

---

## Author

**Prakash Kumar**

M.Tech | Embedded Systems / VLSI

### Technologies

`STM32` `Embedded C` `FreeRTOS` `PWM` `PID` `ADC` `Timers` `UART` `Encoder`

---

## License

This project is licensed under the MIT License.

See the `LICENSE` file for details.

---

## Project Status

**Development / Hardware Validation**

The firmware architecture and control modules are implemented for the reference STM32 configuration. Hardware measurements, PID tuning results, performance graphs, oscilloscope captures, and motor response data should be added after testing on the actual hardware.
