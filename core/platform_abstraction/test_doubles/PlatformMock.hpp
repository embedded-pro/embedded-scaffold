#ifndef CORE_PLATFORM_ABSTRACTION_TEST_DOUBLES_PLATFORM_MOCK_HPP
#define CORE_PLATFORM_ABSTRACTION_TEST_DOUBLES_PLATFORM_MOCK_HPP

#include "core/platform_abstraction/Platform.hpp"
#include "gmock/gmock.h"

namespace platform
{
    class PlatformMock
        : public Platform
    {
    public:
        MOCK_METHOD(hal::GpioPin&, StatusLed, (), (override));
        MOCK_METHOD(hal::SerialCommunication&, Communication, (), (override));
        MOCK_METHOD(services::Tracer&, Tracer, (), (override));
        MOCK_METHOD(void, Run, (), (override));
    };
}

#endif
