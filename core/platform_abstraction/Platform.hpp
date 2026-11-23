#pragma once

#include "hal/interfaces/Gpio.hpp"
#include "hal/interfaces/SerialCommunication.hpp"
#include "services/tracer/Tracer.hpp"

namespace platform
{
    class Platform
    {
    public:
        Platform() = default;
        Platform(const Platform& other) = delete;
        Platform& operator=(const Platform& other) = delete;

        virtual hal::GpioPin& StatusLed() = 0;
        virtual hal::SerialCommunication& Communication() = 0;
        virtual services::Tracer& Tracer() = 0;

        // Never returns on hardware.
        virtual void Run() = 0;

    protected:
        ~Platform() = default;
    };
}
