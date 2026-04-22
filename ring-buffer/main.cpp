#include <iostream>
#include "ring_buffer.h"

int buffer_size = 7;

void test1_fillBuffer(void)
{
    std::cout << "Executing Test 1 Fill Buffer ..." << std::endl;

    RingBuffer rb;

    bool ret;
    bool result = true;

    for (int i = 0; i < buffer_size; i++)
    {
        ret = rb.push(i);

        if (!ret)
        {
            result = false;
            std::cout << "Push method returned false at idx " << i << "." << std::endl;
            break;
        }
    }

    if (result)
    {
        ret = rb.push(8);

        if (ret)
        {
            result = false;
            std::cout << "Push method returned true when the buffer was full." << std::endl;
        }
    }

    if (result)
        std::cout << "Test 1 done!" << std::endl;
    else
        std::cout << "Test 1 failed!" << std::endl;
}

void test2_emptyBuffer(void)
{
    std::cout << "Executing Test 2 Empty Buffer ..." << std::endl;

    RingBuffer rb;
    bool result = true;
    int value;
    bool ret;

    for (int i = 0; i < buffer_size; i++)
    {
        ret = rb.push(i);
        if (!ret)
        {
            result = false;
            std::cout << "Push method returned false at idx " << i << "." << std::endl;
            break;
        }
    }

    if (result)
    {
        for (int i = 0; i < buffer_size; i++)
        {
            ret = rb.pop(value);
            if (!ret)
            {
                result = false;
                std::cout << "Pop method returned false at idx " << i << "." << std::endl;
                break;
            }
            else if (value != i)
            {
                result = false;
                std::cout << "Pop method returned wrong value at idx " << i << "." << std::endl;
                break;
            }
        }
        if (result)
            std::cout << "Test 2 done." << std::endl;
        else
            std::cout << "Test 2 failed!" << std::endl;
    }
}

void test3_wrapAround(void)
{
    std::cout << "Executing Test 3 Wrap Around ..." << std::endl;
    RingBuffer rb;
    int value;
    bool ret;
    bool result = true;

    for (int i = 0; i < 3; i++)
    {
        ret = rb.push(i);
        if (!ret)
        {
            result = false;
            std::cout << "Push method returned false at idx " << i << "." << std::endl;
            break;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        ret = rb.pop(value);
        if (!ret)
        {
            result = false;
            std::cout << "Pop method returned false at idx " << i << "." << std::endl;
            break;
        }
        else if (value != i)
        {
            result = false;
            std::cout << "Pop method returned wrong value at idx " << i << "." << std::endl;
            break;
        }
    }

    for (int i = 3; i < buffer_size; i++)
    {
        ret = rb.push(i);
        if (!ret)
        {
            result = false;
            std::cout << "Push method returned false at idx " << i << "." << std::endl;
            break;
        }
    }

    for (int i = 2; i < buffer_size; i++)
    {
        ret = rb.pop(value);
        if (!ret)
        {
            result = false;
            std::cout << "Pop method returned false at idx " << i << "." << std::endl;
            break;
        }
        else if (value != i)
        {
            result = false;
            std::cout << "Pop method returned wrong value at idx " << i << "." << std::endl;
            break;
        }
    }

    if (result)
        std::cout << "Test 3 done." << std::endl;
    else
        std::cout << "Test 3 failed!" << std::endl;
}

int main()
{

    test1_fillBuffer();

    test2_emptyBuffer();

    test3_wrapAround();

    return 0;
}