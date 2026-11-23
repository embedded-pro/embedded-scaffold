#include "targets/platform_implementations/st/PlatformImpl.hpp"

// All supported ST presets have an 8 MHz HSE.
unsigned int hse_value = 8'000'000;

namespace application
{
    hal::GpioPin& PlatformImpl::StatusLed()
    {
        return statusLed;
    }

    hal::SerialCommunication& PlatformImpl::Communication()
    {
        return uart;
    }

    services::Tracer& PlatformImpl::Tracer()
    {
        return tracer;
    }

    void PlatformImpl::Run()
    {
        eventInfrastructure.Run();
    }
}
