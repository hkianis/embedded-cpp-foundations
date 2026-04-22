#include <atomic>
#include "interrupts.h"
#include "ring_buffer.h"

bool RingBuffer::push(int value)
{
    size_t next = (head + 1) & MASK;
    if (next == tail)
        return false;

    buffer[head] = value;
    std::atomic_thread_fence(std::memory_order_release); // prevent reordering
    head = next;
    return true;
}

bool RingBuffer::pop(int &value)
{
    if (head == tail)
        return false;

    std::atomic_thread_fence(std::memory_order_acquire);
    value = buffer[tail];
    tail = (tail + 1) & MASK;
    return true;
}