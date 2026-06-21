# STM32 platform implementation (STM32F429I-DISC1). Selected by the ST embedded
# presets in CMakePresets.json.
add_library(embedded_scaffold.platform_st ${EMIL_EXCLUDE_FROM_ALL})

target_include_directories(embedded_scaffold.platform_st PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(embedded_scaffold.platform_st PUBLIC
    embedded_scaffold.platform_abstraction
    hal_st.instantiations
    services.tracer
)

target_sources(embedded_scaffold.platform_st PRIVATE
    PlatformImpl.hpp
    PlatformImpl.cpp
)
