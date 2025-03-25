# Core FSM library
add_library(flux_fsm_core SHARED
    flux_fsm_core.c
)

target_include_directories(flux_fsm_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Install rules
install(TARGETS flux_fsm_core
    EXPORT FluxStateTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES
    flux_fsm_core.h
    DESTINATION include
)