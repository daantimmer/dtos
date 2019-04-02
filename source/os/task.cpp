
#include "task.hpp"

#include <algorithm>
#include <cassert>

static void TaskExitError()
{
    while (1)
    {
        assert(false);
    }
}

Task::Task(void (*entry)(), uint32_t* stackTop, size_t stackSize)
    : stackPointer(stackTop + stackSize - 1)
    , stackTop(stackTop + 1)
    , stackSize(stackSize - 2)
{
    *(this->stackTop - 1) = 0xCCCCCCCC;
    std::fill(this->stackTop, stackPointer, 0xDEADBEEF);
    *(this->stackPointer) = 0xCDCDCDCD;

    stackPointer--;
    *stackPointer = 0x21'00'00'00UL; /* xPSR */

    stackPointer--;
    *stackPointer = ((uint32_t)entry) & 0xfffffffeUL; /* PC */

    stackPointer--;
    *stackPointer = (uint32_t)TaskExitError; /* LR */

    stackPointer -= 5;   /* R12, R3, R2 and R1. */
    *stackPointer = 0x0; /* R0 */

    stackPointer -= 8; /* R11, R10, R9, R8, R7, R6, R5 and R4. */
}