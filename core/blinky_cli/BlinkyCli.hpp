#pragma once

#include "core/platform_abstraction/Platform.hpp"
#include "services/peripheral/DebugLed.hpp"
#include "services/util/Terminal.hpp"

namespace application
{
    class BlinkyCli
    {
    public:
        explicit BlinkyCli(platform::Platform& platform);

    private:
        class CliCommands final
            : public services::TerminalCommands
        {
        public:
            CliCommands(services::TerminalWithCommands& terminal, services::Tracer& tracer);

            infra::MemoryRange<const Command> Commands() override;

        private:
            void Ping(const infra::BoundedConstString& params);
            void Identify(const infra::BoundedConstString& params);

            services::Tracer& tracer;
        };

        services::DebugLed debugLed;
        services::TerminalWithCommandsImpl::WithMaxQueueAndMaxHistory<> terminal;
        CliCommands commands;
    };
}
