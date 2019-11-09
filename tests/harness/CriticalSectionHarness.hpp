#pragma once 

#include <gmock/gmock.h>
#include "kernel/criticalsection.hpp"

struct CriticalSectionSpy
{
    MOCK_METHOD0(Enter, void());
    MOCK_METHOD0(Exit, void());

	CriticalSectionSpy()
	{
        instance = this;
	}

	~CriticalSectionSpy()
	{
        instance = nullptr;
	}

    static CriticalSectionSpy* instance;
};

using CriticalSectionStrictSpy = ::testing::StrictMock<CriticalSectionSpy>;