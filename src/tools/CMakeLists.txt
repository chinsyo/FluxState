add_library(fsm_tools_viz STATIC flux_fsm_viz.c)

target_include_directories(fsm_tools_viz PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core
)

target_link_libraries(fsm_tools_viz
    flux_fsm_core
    flux_fsm_log
)

add_library(fsm_tools_perf STATIC flux_fsm_perf.c)

target_include_directories(fsm_tools_perf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core
)

target_link_libraries(fsm_tools_perf
    flux_fsm_core
    flux_fsm_log
)