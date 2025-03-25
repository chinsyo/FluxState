add_library(flux_fsm_log STATIC flux_fsm_log.c)

target_include_directories(flux_fsm_log PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(flux_fsm_log
    PRIVATE
        flux_fsm_core
)