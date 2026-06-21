#ifndef TARGETS_PLATFORM_IMPLEMENTATIONS_ST_PLATFORM_IMPL_HPP
#define TARGETS_PLATFORM_IMPLEMENTATIONS_ST_PLATFORM_IMPL_HPP

#include "core/platform_abstraction/Platform.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF429ZI.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "infra/stream/OutputStream.hpp"
#include "services/tracer/StreamWriterOnSerialCommunication.hpp"
#include "services/tracer/Tracer.hpp"

namespace application
{
    // STM32F429I-DISC1 implementation of platform::Platform.
    //
    // - Status LED  : LD3 (green) on PG13
    // - CLI UART    : USART1, TX = PA9, RX = PA10 (115200 8N1). The on-board ST-LINK
    //                 has no virtual COM port on this board, so connect a USB-UART
    //                 adapter to PA9/PA10/GND.
    class PlatformImpl
        : public platform::Platform
    {
    public:
        hal::GpioPin& StatusLed() override;
        hal::SerialCommunication& Communication() override;
        services::Tracer& Tracer() override;
        void Run() override;

    private:
        // Initialises the STM32 HAL and system clock before any peripheral member
        // is constructed. Declared first so its constructor runs first.
        struct ClockInit
        {
            ClockInit()
            {
                HAL_Init();
                ConfigureDefaultClockNucleoF429ZI();
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

#endif
