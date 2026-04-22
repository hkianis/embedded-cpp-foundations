#include <cstddef>
#define COMPILER_BARRIER() asm volatile("" ::: "memory")
class RingBuffer {
public:
    bool push(int value);
    bool pop(int& value);

    bool isEmpty() const;
    bool isFull() const;

private:
    static constexpr size_t SIZE = 8;
    static constexpr size_t MASK = SIZE - 1;

    int buffer[SIZE];

    volatile size_t head = 0;
    volatile size_t tail = 0;
};