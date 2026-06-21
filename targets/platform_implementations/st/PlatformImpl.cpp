#include "targets/platform_implementations/st/PlatformImpl.hpp"

// Referenced by the hal_st clock configuration; the F429I-DISC1 has an 8 MHz HSE.
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
