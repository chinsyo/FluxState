# Core FSM tests
add_executable(test_fsm
    test_fsm.c
)


target_include_directories(test_fsm PRIVATE ${Unity_SOURCE_DIR}/src)
target_link_libraries(test_fsm
    PRIVATE
        flux_fsm_log
        flux_fsm_core
        unity
)

add_test(NAME test_fsm COMMAND test_fsm)