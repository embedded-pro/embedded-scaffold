#include PLATFORM_IMPL_HEADER
#include "core/blinky_cli/BlinkyCli.hpp"

int main()
{
    static application::PlatformImpl platform;
    static application::BlinkyCli blinkyCli{ platform };

    platform.Run();

#if defined(__GNUC__) || defined(__clang__)
    __builtin_unreachable();
#endif
}
