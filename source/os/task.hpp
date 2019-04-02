
#pragma once

#include <cstdint>
#include <cstdlib> 



struct Task
{
    Task(void (*entry)(), uint32_t* stackTop, size_t stackSize);


protected:
    uint32_t* stackPointer;
    uint32_t* stackTop;
    size_t stackSize;
};