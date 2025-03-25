# 添加工具目录的 CMake 配置
add_executable(test_perf test_perf.c)

# 链接必要的库
target_include_directories(test_perf PRIVATE ${Unity_SOURCE_DIR}/src)
target_link_libraries(test_perf 
    PRIVATE
        flux_fsm_core 
        fsm_tools_viz
        fsm_tools_perf
        unity
)

# 设置输出目录
set_target_properties(test_perf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tools
)

# 添加测试（可选）
add_test(NAME test_perf COMMAND test_perf)
