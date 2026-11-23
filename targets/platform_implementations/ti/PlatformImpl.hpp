#pragma once

#include "core/platform_abstraction/Platform.hpp"
#include "hal_tiva/instantiations/EventInfrastructure.hpp"
#include "hal_tiva/instantiations/LaunchPadBsp.hpp"
#include "hal_tiva/tiva/Gpio.hpp"
#include "hal_tiva/tiva/Uart.hpp"
#include "infra/stream/OutputStream.hpp"
#include "services/tracer/StreamWriterOnSerialCommunication.hpp"
#include "services/tracer/Tracer.hpp"

namespace application
{
    class PlatformImpl final
        : public platform::Platform
    {
    public:
        hal::GpioPin& StatusLed() override;
        hal::SerialCommunication& Communication() override;
        services::Tracer& Tracer() override;
        void Run() override;

    private:
        // UartBase's constructor is protected; this wrapper makes it public.
        class CliUart
            : public hal::tiva::Uart
        {
        public:
            CliUart(uint8_t index, hal::tiva::GpioPin& tx, hal::tiva::GpioPin& rx, const Config& config)
                : hal::tiva::Uart(index, tx, rx, config)
            {}
        };

        instantiations::LaunchPad launchPad;
        instantiations::EventInfrastructure eventInfrastructure;
        hal::tiva::GpioPin uartTx{ hal::tiva::Port::A, 1 };
        hal::tiva::GpioPin uartRx{ hal::tiva::Port::A, 0 };
        hal::tiva::UartBase::Config uartConfig;
        CliUart uart{ 0, uartTx, uartRx, uartConfig };
        services::StreamWriterOnSerialCommunication::WithStorage<256> streamWriter{ uart };
        infra::TextOutputStream::WithErrorPolicy stream{ streamWriter };
        services::TracerToStream tracer{ stream };
    };
}
