#pragma once

#include "core/platform_abstraction/Platform.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include SCAFFOLD_ST_CLOCK_HEADER
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "infra/stream/OutputStream.hpp"
#include "services/tracer/StreamWriterOnSerialCommunication.hpp"
#include "services/tracer/Tracer.hpp"

namespace application
{
    // Boards without a virtual COM port on USART1 (PA9/PA10) need a USB-UART adapter.
    class PlatformImpl final
        : public platform::Platform
    {
    public:
        hal::GpioPin& StatusLed() override;
        hal::SerialCommunication& Communication() override;
        services::Tracer& Tracer() override;
        void Run() override;

    private:
        // Declared first: HAL and clock must be up before any peripheral is constructed.
        struct ClockInit
        {
            ClockInit()
            {
                HAL_Init();
                SCAFFOLD_ST_CLOCK_INIT();
            }
        };

        ClockInit clockInit;
        main_::StmEventInfrastructure eventInfrastructure;
        hal::GpioPinStm statusLed{ hal::Port::G, 13 };
        hal::GpioPinStm uartTx{ hal::Port::A, 9 };
        hal::GpioPinStm uartRx{ hal::Port::A, 10 };
        hal::UartStm uart{ 1, uartTx, uartRx };
        services::StreamWriterOnSerialCommunication::WithStorage<256> streamWriter{ uart };
        infra::TextOutputStream::WithErrorPolicy stream{ streamWriter };
        services::TracerToStream tracer{ stream };
    };
}
