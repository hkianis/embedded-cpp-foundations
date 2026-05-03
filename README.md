# Embedded ECU Logger

## Overview

This project implements a small embedded-style logging system for an ECU-like environment.  
The logger is designed for interrupt-driven systems where log events may be produced inside an ISR and consumed later by the main loop.

The project avoids dynamic memory allocation and STL containers in the core embedded modules.  
It focuses on deterministic behavior, fixed-size data structures, and testability.

## Goals

- Implement a fixed-size ring buffer.
- Design an ISR-safe logging interface.
- Avoid dynamic memory allocation.
- Avoid STL containers in embedded core modules.
- Keep ISR execution time short and bounded.
- Defer formatting and printing to the main loop.
- Add unit tests for core behavior.

## Non-Goals

- This project does not implement a full RTOS.
- This project does not use dynamic logging strings inside ISR context.
- This project does not perform UART/CAN output inside ISR.
- Hardware support is planned later but not part of the first version.

## Architecture

ISR context:
- creates a compact fixed-size log entry
- pushes it into a preallocated ring buffer
- returns immediately

Main loop context:
- reads log entries from the buffer
- formats or processes logs outside ISR context

## Current Modules

### Ring Buffer

A fixed-size FIFO buffer implemented without STL containers or dynamic allocation.

### ISR Logger

A logger that stores compact log entries from ISR context and allows the main loop to consume them later.

## Planned Modules

- CAN frame parser
- ECU state machine
- STM32 hardware port
- UART log output
- timer-based timestamps

## Build and Test

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest
