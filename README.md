# Embedded ECU Logger

## Overview

This project is an embedded C++ practice project focused on building interview-relevant embedded software components.

The current goal is to implement a small ECU-style logging system where events can be produced from interrupt context and consumed later by the main loop. The design follows typical embedded constraints: fixed-size memory, deterministic behavior, no dynamic allocation, and minimal work inside interrupt service routines.

The project starts with a reusable fixed-size ring buffer and builds an ISR-safe logger on top of it. Later milestones will extend the project with CAN frame parsing, ECU state-machine logic, and an STM32 hardware port.

## Goals

- Implement embedded-style C++ components suitable for interview preparation.
- Use fixed-size data structures.
- Avoid dynamic memory allocation in core embedded modules.
- Avoid STL containers in core embedded modules.
- Keep interrupt service routines short and predictable.
- Defer formatting, printing, and communication work to the main loop.
- Add unit tests for core behavior.
- Document design decisions clearly enough to explain them in an interview.

## Non-Goals

- This is not a full AUTOSAR implementation.
- This is not a complete RTOS-based system.
- This project does not perform logging output directly inside an ISR.
- This project does not use dynamic strings or heap allocation in the embedded core.
- Hardware support is planned later and is not required for the first software milestones.

## Current Status

| Module | Status | Description |
|---|---|---|
| Fixed-Size Ring Buffer | Completed | FIFO buffer with fixed capacity, no STL containers, and no dynamic allocation. |
| ISR-Safe Logger | In Progress | Logger that stores compact log events from ISR context and allows main-loop consumption. |
| CAN Frame Parser | Planned | Decode CAN payload data using embedded-style fixed-size structures. |
| ECU State Machine | Planned | Simple ECU behavior model for event-driven embedded logic. |
| STM32 Hardware Port | Planned | Run selected modules on an STM32 Nucleo board. |

## Architecture

The logger is designed around a producer-consumer pattern.

### ISR Context

The ISR-side function should:

- create a compact fixed-size log entry;
- push the entry into a preallocated ring buffer;
- avoid formatting strings;
- avoid printing;
- avoid blocking operations;
- avoid heap allocation;
- return quickly.

### Main Loop Context

The main loop should:

- read log entries from the ring buffer;
- format logs if needed;
- print or transmit logs outside interrupt context;
- handle dropped log counters or overflow diagnostics.

This keeps interrupt latency predictable and moves expensive work out of the ISR.

## Planned Repository Structure

```text
embedded-ecu-logger/
|
|-- README.md
|-- CMakeLists.txt
|
|-- include/
|   |-- ring_buffer.h
|   |-- isr_logger.h
|
|-- src/
|   |-- ring_buffer.cpp
|   |-- isr_logger.cpp
|
|-- tests/
|   |-- test_ring_buffer.cpp
|   |-- test_isr_logger.cpp
|
|-- docs/
|   |-- architecture.md
|   |-- interview_notes.md
|
|-- .gitignore
```

## Core Design Rules

The embedded core modules should follow these rules:

- no dynamic memory allocation;
- no STL containers;
- fixed-size data structures only;
- predictable execution time;
- explicit overflow behavior;
- clear ownership of data;
- small APIs with simple types;
- testable logic independent of hardware.

## Example Logger API

```cpp
bool log_from_isr(EventId event_id, LogLevel level, uint8_t parameter);
bool read_log(LogEntry& entry);
uint32_t get_dropped_log_count();
```

## Example Log Entry

```cpp
enum class EventId : uint8_t {
    ButtonPressed,
    CanMessageReceived,
    SensorTimeout,
    ErrorDetected
};

enum class LogLevel : uint8_t {
    Info,
    Warning,
    Error
};

struct LogEntry {
    uint32_t timestamp;
    EventId event_id;
    LogLevel level;
    uint8_t parameter;
};
```

## Build and Test

A typical CMake workflow is expected:

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest
```

The exact build commands may change depending on the chosen test framework and compiler setup.

## Milestones

### Milestone 1: Core Embedded Data Structures

- Implement fixed-size ring buffer.
- Add ring buffer tests.
- Document ring buffer behavior.

### Milestone 2: ISR-Safe Logging System

- Design ISR-safe logger API.
- Implement logger using the fixed-size ring buffer.
- Add overflow handling.
- Add logger unit tests.
- Document ISR logger architecture.

### Milestone 3: Automotive Message Handling

- Define CAN frame structure.
- Implement CAN signal extraction.
- Add CAN parser tests.
- Document CAN parser assumptions.

### Milestone 4: ECU Behavior

- Implement simple ECU state machine.
- Add state transition tests.
- Connect logger to state transitions.

### Milestone 5: STM32 Hardware Port

- Port logger to STM32 Nucleo board.
- Use a button interrupt as an event source.
- Use a hardware timer for timestamps.
- Print logs from the main loop over UART.

## Definition of Done

An issue is considered done only when:

- the code compiles successfully;
- compiler warnings are fixed or justified;
- the implementation satisfies the acceptance criteria;
- unit tests are added or updated;
- edge cases are tested;
- no dynamic memory allocation is used in core embedded modules;
- no STL containers are used in core embedded modules;
- behavior is documented where needed;
- the solution can be explained clearly in an interview.

## Interview Topics Covered

This project is designed to support embedded software interview preparation. It covers:

- fixed-size memory management;
- ring buffers;
- interrupt-safe design;
- producer-consumer communication;
- embedded logging;
- overflow handling;
- unit testing embedded logic;
- CAN message parsing;
- ECU state-machine design;
- STM32 hardware bring-up.

## Hardware Plan

Hardware is not required for the first milestones. The first phase focuses on software design and testing on a normal development machine.

Later, the project can be ported to an STM32 Nucleo board. The planned hardware setup includes:

- STM32 Nucleo board;
- USB cable for flashing and debugging;
- breadboard;
- jumper wires;
- LEDs and resistors;
- push buttons;
- optional logic analyzer;
- optional CAN transceiver module.

## Notes

The purpose of this project is not just to make code work. The purpose is to build embedded software that demonstrates good engineering decisions under embedded constraints.

Each module should be simple, testable, deterministic, and explainable.
