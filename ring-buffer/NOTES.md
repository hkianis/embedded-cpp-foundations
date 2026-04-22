# Embedded C++ Core Notes (Interview Review – Ring Buffer & Concurrency)

---

## 1. Types and Compile-Time Configuration

### `size_t`

* Unsigned type for sizes and indices
* Matches platform memory model

```cpp
#include <cstddef>
size_t index;
```

**Caution**

* No negative values
* Underflow wraps

---

### `constexpr`

* Compile-time constant
* Required for array size and masks

```cpp
static constexpr size_t SIZE = 8;
static constexpr size_t MASK = SIZE - 1;
```

---

### Power-of-2 Optimization

```cpp
index = (index + 1) & MASK;
```

**Why it works**

* `SIZE = 2^n`
* `MASK = SIZE - 1` → lower bits = 1
* AND keeps only valid range `[0, SIZE-1]`

---

## 2. SPSC (Single Producer Single Consumer)

### Definition

* One producer (ISR)
* One consumer (main loop)

---

### Ownership Model

| Variable | Owner    |
| -------- | -------- |
| `head`   | Producer |
| `tail`   | Consumer |

---

### Key Principle

> No shared writes → no race condition

---

### Safe Access Patterns

| Access Type   | Safe?                           |
| ------------- | ------------------------------- |
| Read + Read   | ✔                               |
| Read + Write  | ⚠ (needs visibility + ordering) |
| Write + Write | ❌                               |

---

## 3. Volatile (Visibility Only)

```cpp
volatile size_t head;
volatile size_t tail;
```

### Guarantees

* Always read/write from memory
* Prevents compiler caching

---

### Does NOT guarantee

* ❌ atomicity
* ❌ ordering
* ❌ thread safety

---

## 4. Memory Ordering Problem

### Producer code

```cpp
buffer[head] = value;
head = next;
```

---

### Possible reordering (weak CPU)

```cpp
head = next;
buffer[...] = value;
```

---

### Result

* Consumer sees new `head`
* Data not yet visible → stale read

---

## 5. Memory Barriers / Fences

### Release Fence (Producer)

```cpp
std::atomic_thread_fence(std::memory_order_release);
```

Guarantee:

> All writes before fence are visible before `head` update

---

### Acquire Fence (Consumer)

```cpp
std::atomic_thread_fence(std::memory_order_acquire);
```

Guarantee:

> After observing `head`, buffer data is visible

---

### Combined Effect

```text
Producer: write → release → publish head  
Consumer: observe head → acquire → read data
```

---

## 6. Correct SPSC Pattern

```cpp
bool push(int value)
{
    size_t next = (head + 1) & MASK;

    if (next == tail)
        return false;

    buffer[head] = value;
    std::atomic_thread_fence(std::memory_order_release);
    head = next;

    return true;
}

bool pop(int& value)
{
    if (head == tail)
        return false;

    std::atomic_thread_fence(std::memory_order_acquire);
    value = buffer[tail];
    tail = (tail + 1) & MASK;

    return true;
}
```

---

## 7. Why Push and Pop Are NOT Symmetric

### Producer (Push)

```text
WRITE data → PUBLISH pointer
```

Must enforce ordering → **critical**

---

### Consumer (Pop)

```text
OBSERVE pointer → READ data
```

Ordering matters only for visibility (acquire), not pointer update

---

### Key Insight

> Producer must ensure data-before-pointer
> Consumer must ensure pointer-before-data visibility

---

## 8. What Goes Wrong Without Fences

### On weakly ordered CPU (Cortex-A)

1. Producer updates `head` first
2. Buffer write delayed (cache/store buffer)
3. Consumer sees new `head`
4. Reads stale data

---

### Result

> No crash, but **incorrect data (stale read)**

---

## 9. Cortex-M vs Cortex-A

| Feature      | Cortex-M | Cortex-A     |
| ------------ | -------- | ------------ |
| Execution    | In-order | Out-of-order |
| Memory model | Strong   | Weak         |
| Cache        | Minimal  | Complex      |
| Need fences  | Rare     | Required     |

---

### Practical Rule

* Cortex-M → compiler barrier often enough
* Cortex-A → memory fences required

---

## 10. Atomic vs Embedded Reality

### Avoid

```cpp
std::atomic<T> with RMW operations
```

* heavy
* unpredictable timing

---

### Use

```cpp
std::atomic_thread_fence()
```

* lightweight
* enforces ordering
* maps to hardware barrier (e.g., DMB)

---

## 11. Interrupts vs Lock-Free Design

### Do NOT mix models

| Model            | Description                     |
| ---------------- | ------------------------------- |
| SPSC             | lock-free, no interrupt disable |
| Critical section | disable interrupts              |

---

### Rule

> Pick one model, do not combine both

---

## 12. Overflow Handling (System-Level)

When buffer is full:

```cpp
if (next == tail)
    return false;
```

---

### Possible strategies

* Drop new data
* Overwrite old data (requires synchronization)
* Count overflow events
* Apply backpressure

---

### Embedded principle

> Buffer does NOT decide policy — system does

---

## 13. Assignment Return Trap

```cpp
return (head = next);
```

Means:

```text
assign → return assigned value
```

---

### Danger

* returns `false` if value = 0
* logic becomes incorrect

---

### Correct

```cpp
head = next;
return true;
```

---

## 14. Final Principles

* Avoid shared mutable state
* Prefer ownership over locking
* Optimize for deterministic timing
* Understand hardware memory model
* Separate correctness from performance

---

## 15. One-Line Summary

```text
SPSC + release/acquire + ownership = safe, efficient embedded communication
```
