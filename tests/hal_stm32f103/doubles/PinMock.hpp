#ifndef DOUBLES_PINMOCK_HPP
#define DOUBLES_PINMOCK_HPP

#include "gmock/gmock.h"

#include <hal/pin.hpp>

struct IPinInputMock
{
    MOCK_CONST_METHOD0(Get, bool());
	MOCK_METHOD0(SetInput, void());
};

struct IPinOutputMock : IPinInputMock
{
    MOCK_CONST_METHOD1(Set, void(bool));
    MOCK_METHOD0(SetOutput, void());
    MOCK_METHOD1(SetOutput, void(bool));
};

#endif