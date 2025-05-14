# Rival Rush Game on SJ2-C Development Environment

The `SJ2` board is an ARM-based platform used at San Jose State University (SJSU) for teaching Embedded Systems. This repository provides a C-based development environment designed for learning low-level microcontroller workings with a Real-Time Operating System (RTOS). It supports cross-platform development (Mac, Linux, Windows) without requiring VMs or WSL for Windows.

Key principles:
- Implemented in C for clarity and simplicity.
- Minimalistic design with few abstractions.
- Adherence to good coding practices (YAGNI, DRY).
- Version-controlled toolchains for consistency.

We use [SCons](https://scons.org/) as our build platform. The developers of SJ2-C applied experience of diverse set of build systems acquired over many years, and resorted to this one. The candidates were:

- SCons (used at Tesla, and many other companies)
- Bazel (used at Google, Zoox and many other companies)
- Make

SCons discusses the advantages and disadvantages of other existing build systems: [Build System Comparison](https://github.com/SCons/scons/wiki/sconsvsotherbuildtools)

## Docker

This project utilizes Docker for its development environment. For educational purposes, feel free to see the [Docker Guide](README-DOCKER.md) to learn how to maintain and manually interact with this project's Docker image. However, it's important to note that this guide is not part of the firmware development workflow and is only applicable to maintainers of this project.

## LPC40xx FreeRTOS Sample Project: "Rush-Rivals" Game

The `projects/lpc40xx_freertos` directory contains an embedded application built for the LPC40xx microcontroller using FreeRTOS. This example project, potentially a game tentatively named "Rush Rivals," demonstrates the use of an LED matrix display, accelerometer input, and ESP32-based wireless communication.

### Project Structure

The project is organized into several layers, each with a specific responsibility:

#### `l3_drivers` - Low-Level Peripheral Drivers
This layer contains the fundamental drivers for interacting with the LPC40xx microcontroller's hardware peripherals.

*   **Headers (`*.h`)**:
    *   `adc.h`: Analog-to-Digital Converter
    *   `board_io.h`: Board-specific I/O configurations (like LEDs, buttons)
    *   `can_bus.h`: Controller Area Network bus communication
    *   `gpio.h`: General Purpose Input/Output
    *   `hw_timer.h`: Hardware Timers
    *   `i2c.h`: Inter-Integrated Circuit communication
    *   `lpc_peripherals.h`: Definitions for LPC peripheral base addresses and power-on bits.
    *   `pwm1.h`: Pulse Width Modulation (module 1)
    *   `ssp2.h`: Synchronous Serial Port 2 (for SPI communication)
    *   `uart.h`: Universal Asynchronous Receiver/Transmitter
*   **Sources (`sources/*.c`)**:
    *   Contains the corresponding C implementation files for the headers listed above. These files house the logic for initializing, configuring, reading from, and writing to the hardware peripherals.

#### `l4_io` - Mid-Level I/O and System Services
This layer builds upon the `l3_drivers` to provide more abstracted I/O functionalities and system-level services.

*   **Core I/O & System:**
    *   `delay.h`/`sources/delay.c`: Provides delay functionalities.
    *   `peripherals_init.h`/`sources/peripherals_init.c`: Handles initialization of various peripherals used by the application.
    *   `ssp2_mutex.h`/`sources/ssp2_mutex.c`: Provides mutex protection for SSP2 peripheral, likely for thread-safe SPI communication.
    *   `sys_time.h`/`sources/sys_time.c`: System time utilities, possibly for FreeRTOS run-time stats.
    *   `uart_printf.h`/`sources/uart_printf.c`: UART-based printf functionality for debugging and output.
    *   `uart3_init.h`: (Header only) Initialization for UART3, likely used for ESP32 communication.
*   **Command Line Interface (`cli/`)**:
    *   `app_cli.h`/`app_cli.c`: Application-specific CLI implementation.
    *   `sj2_cli.h`/`sj2_cli.c`: SJ2 board specific CLI components.
    *   `cli_handlers.h`: Header for CLI command handlers.
    *   `handlers/handler_i2c.c`: CLI handlers for I2C operations.
    *   `handlers/handlers_general.c`: General CLI command handlers.
*   **ESP32 WiFi Module (`esp32/`)**:
    *   `esp32.h`/`esp32.c`: Driver for interacting with an ESP32 WiFi module via AT commands.
    *   `esp32_task.h`/`esp32_task.c`: FreeRTOS task for managing ESP32 communication (e.g., TCP/IP connection).
    *   `README.md`: Details on using the ESP32 module, including UART3 interface, AT commands, and SD card configuration for WiFi and TCP server parameters (`wifi.txt`, `tcp_server.txt`).
*   **Examples (`examples/`)**:
    *   `file_io_example.c`: Example usage of file I/O, likely with the SD card.
*   **FAT File System (`fatfs/`)**:
    *   Integrates the FatFs library for file system operations, primarily for SD card storage.
    *   `fatfs_glue/`: Contains glue logic to interface FatFs with the underlying hardware (SD card driver).
        *   `disk_glue_logic.c`: Implements disk I/O functions required by FatFs.
        *   `sd_card.h`/`sd_card.c`: Driver for SD card communication (likely via SPI/SSP).
    *   `ff13c/`: Contains the FatFs library source code (version ff13c).
*   **Newlib System Calls (`newlib/`)**:
    *   `system_calls.c`: Implements system calls required by the Newlib C standard library, redirecting them to appropriate hardware functions (e.g., UART for `printf`).
*   **Sensors (`sensors/`)**:
    *   `acceleration.h`/`sources/acceleration.c`: Interface for an accelerometer.
    *   `apds.h`: (Header only) Likely for an APDS light/proximity sensor.
    *   `sensors.h`/`sources/sensors.c`: General sensor management or a placeholder.

#### `l5_application` - Application Logic
This layer contains the main application code, including game logic, tasks, and RTOS specific configurations.

*   **Core Application Files**:
    *   `main.c`: The entry point of the application. Initializes peripherals, creates FreeRTOS tasks for various functionalities (blinking LEDs, accelerometer, display updates, UART control), and starts the FreeRTOS scheduler.
    *   `rtos_hooks.c`: Implements FreeRTOS application hook functions such as stack overflow handling, malloc failure hooks, idle task behavior, and tick hooks.
*   **Accelerometer (`accelerometer/`)**:
    *   `accelerometer_task.h`/`accelerometer_task.c`: FreeRTOS task to read and process data from the accelerometer.
*   **Game Logic (`game/`)**:
    *   `draw.h`/`draw.c`: Functions for drawing game elements on the display.
    *   `game_task.h`/`game_task.c`: The main FreeRTOS task managing the game state, logic, and interactions. This includes `display_update_task` and `uart_control_task`.
    *   `object.h`/`object.c`: Defines game objects, their properties, and behaviors.
*   **LED Matrix Driver (`led_matrix_driver/`)**:
    *   `graphics.h`/`graphics.c`: Higher-level graphics routines for the LED matrix.
    *   `led_driver.h`/`led_driver.c`: Low-level driver for controlling the LED matrix display.
*   **Periodic Callbacks (`periodics/`)**:
    *   `periodic_callbacks.h`/`periodic_callbacks.c`: Defines callback functions that can be scheduled to run periodically.
    *   `periodic_scheduler.h`/`periodic_scheduler.c`: A scheduler for managing these periodic tasks.

### Key Functionalities of the Sample Project

*   **Hardware Abstraction**: Clear separation of concerns with low-level drivers (`l3_drivers`), mid-level I/O (`l4_io`), and application logic (`l5_application`).
*   **Real-Time Operating System**: Utilizes FreeRTOS for task management.
*   **LED Matrix Display**: Support for an LED matrix with low-level driving and graphics routines.
*   **Accelerometer Input**: Integration of an accelerometer for motion-based input.
*   **Game Logic**: Modules for game objects, drawing, and game state management.
*   **Serial Communication**: UART drivers and `printf` redirection.
*   **Command Line Interface**: For debugging or configuration.
*   **SD Card Support**: FatFS integration for file storage (e.g., ESP32 configuration).
*   **ESP32 WiFi Connectivity**: Support for network communication via an ESP32 module.
*   **System Services**: Periodic task scheduling, delay functions, and RTOS hooks.

### Getting Started with the Sample Project (Inferred)

1.  **Hardware Setup**:
    *   LPC40xx board (e.g., SJ2).
    *   LED Matrix connected to GPIO.
    *   Accelerometer connected.
    *   (Optional) ESP32 module via UART3.
    *   (Optional) Micro SD card with `wifi.txt` and `tcp_server.txt` for ESP32.
2.  **Compilation and Flashing**:
    *   Use the project's build system (SCons) to compile.
    *   Flash the binary to the LPC40xx.
3.  **Running the Application**:
    *   `main.c` initializes UART0 (115200 baud), LED matrix, and creates FreeRTOS tasks.
    *   The FreeRTOS scheduler starts the tasks.

### Main Tasks Created in `main.c`

*   `blink_task` (two instances): Toggles on-board LEDs (`led0`, `led1`) every 500ms.
*   `accelerometer_task`: Manages accelerometer input.
*   `display_update_task`: Updates the LED matrix display.
*   `uart_control_task`: Handles UART input/commands.

This provides a high-level overview of the `lpc40xx_freertos` sample project. Further details can be found by exploring the source code within each module.
