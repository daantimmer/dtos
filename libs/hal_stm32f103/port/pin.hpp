#ifndef PORT_PIN_HPP
#define PORT_PIN_HPP

#include <hal/pin.hpp>

namespace hal
{
    namespace port
	{
        struct PinInput : IPinInput
        {
            virtual bool Get() const override;

            virtual void SetInput() override;
        };

        struct PinOutput : IPinOutput
        {
            virtual void Set(bool) override;

            virtual void SetOutput() override;
            virtual void SetOutput(bool) override;
        };
    }
}

#endif