add_executable(basic_fsm
    basic_fsm.c
)

target_include_directories(basic_fsm PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(basic_fsm
    flux_fsm_core
    flux_fsm_log
)

install(TARGETS basic_fsm
    RUNTIME DESTINATION bin
)