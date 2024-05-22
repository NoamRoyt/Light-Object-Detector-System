
api.c:
scan180degree: Initiates a 180-degree scan, controlling an engine's movement, generating pulses, and calculating measurements using a timer.

Telemeter: Conducts measurements with a telemeter, involving engine control, pulse generation, measurement calculation, and data preparation.

scan180degreeLDR: Manages a 180-degree scan with LDR (light-dependent resistor) measurements, including engine control, ADC configuration, data acquisition, and angle adjustment.

calibration: Performs LDR calibration by conducting a scan and calculating the delta measurement for calibration purposes.

ldrAndObjekt: Combines LDR and object detection, acquiring LDR measurements and calculating deltas while accounting for engine movement.

clear_data_to_send: Clears an array used for data storage and transmission.


halGPIO.h:
Provides function prototypes for hardware control, including GPIO pins, LCD display, interrupts, timers, and delays. It defines global variables for system states, mode settings, engine movement, range measurements, data storage, and angle information. The file includes macros for LCD pin control, cursor operations, and custom delay functions, streamlining hardware interactions.


halGPIO.c:
Provides low-level hardware abstraction functions and configurations for managing GPIO (General Purpose Input/Output) operations in a microcontroller-based system. It includes functions for system configuration, delay generation, timer configuration, pulse generation, interrupt control, and UART initialization. It also implements ISRs for various timer events, ADC (Analog-to-Digital Converter) interrupts, and UART communication. The file abstracts hardware-specific details, allowing higher-level software layers to interact with GPIO pins and perform various tasks, such as signal timing, pulse generation, and data communication.


app.h:
Defines two global enumerations: FSMstate for representing states in a finite state machine, and SYSmode for representing different operating modes of a system. It's enclosed in preprocessor directives to ensure header file inclusion protection.

bsp.h:
Defines hardware configurations and GPIO assignments for an MSP430 microcontroller. It includes necessary headers, sets constants for debounce and LED rate, and configures pins for PWM, trigger, echo, and UART. Functions for GPIO, timer, ADC, and UART setup are declared.

bsp.c:
Provides functions for configuring GPIO pins for PWM, trigger, and echo, setting up timers for PWM generation, and configuring the ADC (Analog-to-Digital Converter) for specific input pins. It also includes functions to clear the ADC settings. It's likely a part of a Board Support Package (BSP) providing hardware abstraction and configuration for the underlying microcontroller.



main.c:
Initializes the system, sets initial states for the state machine, and enters an infinite loop where it continuously switches between different states based on the state variable. It calls functions corresponding to different states, such as scanning, telemetry, calibration, and handling sensors. It also handles communication using UART and enters low power mode between state transitions.
