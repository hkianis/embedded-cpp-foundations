# Embedded C++ Foundations

This repository contains low-level C++ implementations focused on embedded systems concepts.
The goal is to build deterministic, resource-aware code without relying on high-level abstractions.

---

## Objectives

* Write predictable and deterministic C++ code
* Avoid unnecessary dynamic memory allocation
* Understand memory layout and data ownership
* Handle edge cases and failure scenarios explicitly
* Build components that are safe for embedded environments

---

## Project Structure

```
embedded-cpp-foundations/
│
├── CMakeLists.txt
├── README.md
│
├── ring-buffer/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── ring_buffer.h
│   ├── ring_buffer.cpp
```

---

## Implementations

### Ring Buffer

A fixed-size FIFO buffer implemented without STL or dynamic memory.

**Key properties:**

* Constant memory usage
* Deterministic behavior
* No data overwrite when full
* Explicit handling of overflow and underflow

**Core operations:**

* `push(int value)` → inserts data, fails if full
* `pop(int& value)` → retrieves data, fails if empty
* `isEmpty()` → checks if buffer is empty
* `isFull()` → checks if buffer is full

---

## Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

---

## Testing

Basic tests are implemented in `main.cpp`:

* Fill buffer until full
* Attempt overflow (must fail)
* Empty buffer and verify FIFO order
* Attempt underflow (must fail)
* Wrap-around behavior validation

---

## Design Constraints

* No use of STL containers
* No dynamic memory allocation (`new`, `malloc`)
* Fixed-size buffer
* Deterministic execution

---

## Notes

This repository is focused on fundamentals.
Implementations prioritize correctness, clarity, and predictability over abstraction.

---
