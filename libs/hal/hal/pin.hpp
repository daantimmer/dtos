#ifndef HAL_IPIN_HPP
#define HAL_IPIN_HPP

namespace hal
{
    struct IPinInput
    {
        virtual bool Get() const = 0;

        virtual void SetInput() = 0;
    };

    struct IPinOutput : IPinInput
    {
        virtual void Set(bool) = 0;

        virtual void SetOutput() = 0;
        virtual void SetOutput(bool) = 0;
    };
}

#endif