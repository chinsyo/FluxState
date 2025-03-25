/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "flux_fsm_perf.h"
#include "flux_fsm_viz.h"

/* 测试状态定义 */
#define STATE_IDLE      0
#define STATE_RUNNING   1
#define STATE_PAUSED    2
#define STATE_STOPPED   3

/* 测试事件定义 */
#define EVENT_START     0
#define EVENT_PAUSE     1
#define EVENT_RESUME    2
#define EVENT_STOP      3

/* 测试用例：性能统计 */
void test_perf_stats(void) {
    flux_fsm_perf_t perf;

    /* 初始化性能计数器 */
    flux_fsm_perf_init(&perf);

    /* 模拟状态转换并更新性能统计 */
    for (int i = 0; i < 1000; i++) {
        double transition_time = (double)(rand() % 100) / 1000.0;
        flux_fsm_perf_update(&perf, transition_time);
    }

    /* 验证性能统计结果 */
    printf("Performance Statistics:\n");
    printf("Total Transitions: %lu\n", perf.transitions);
    printf("Average Time: %.3f ms\n", perf.avg_transition_time);
    printf("Maximum Time: %.3f ms\n", perf.max_transition_time);

    /* 导出性能数据为 JSON 格式 */
    const char* json_data = flux_fsm_perf_to_json(&perf);
    printf("Performance JSON Data:\n%s\n", json_data);

    /* 重置性能计数器 */
    flux_fsm_perf_reset(&perf);
}

/* 测试用例：状态图可视化 */
void test_visualization(void) {
    flux_fsm_t fsm;
    flux_fsm_viz_config_t viz_config;
    
    /* 初始化状态机 */
    memset(&fsm, 0, sizeof(flux_fsm_t));
    fsm.initial_state = STATE_IDLE;
    
    /* 添加状态转换 */
    flux_fsm_transition_t transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_RUNNING, NULL, NULL},
        {STATE_RUNNING, EVENT_PAUSE, STATE_PAUSED, NULL, NULL},
        {STATE_PAUSED, EVENT_RESUME, STATE_RUNNING, NULL, NULL},
        {STATE_RUNNING, EVENT_STOP, STATE_STOPPED, NULL, NULL}
    };
    
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        flux_fsm_add_transition(&fsm, &transitions[i]);
    }
    
    /* 配置可视化参数 */
    flux_fsm_viz_init(&viz_config);
    viz_config.format = FLUX_FSM_VIZ_DOT;
    viz_config.title = "Test FSM";
    viz_config.node_shape = "box";
    flux_fsm_viz_set_config(&viz_config);
    
    /* 生成状态图 */
    const char* dot_content = flux_fsm_viz_generate(&fsm, &viz_config);
    printf("Generated DOT Content:\n%s\n", dot_content);
    
    /* 导出为SVG文件 */
    flux_fsm_viz_export(&fsm, "test_fsm.svg");
}

/* 测试用例：状态机验证 */
void test_validation(void) {
    flux_fsm_t fsm = {0};
    flux_fsm_viz_validation_result_t result;
    
    /* 测试无效初始状态 */
    fsm.initial_state = -1;
    result = flux_fsm_viz_validate(&fsm);
    printf("无效初始状态测试: %s\n", result.error_code ? "通过" : "失败");

    /* 测试有效状态机 */
    fsm.initial_state = 0;
    result = flux_fsm_viz_validate(&fsm);
    printf("有效状态机测试: %s\n", result.error_code ? "失败" : "通过");
}

int main(void) {
    /* 初始化随机数生成器 */
    srand((unsigned int)time(NULL));

    /* 运行测试用例 */
    printf("\n=== Testing Performance Statistics ===\n");
    test_perf_stats();

    printf("\n=== Testing FSM Visualization ===\n");
    test_visualization();

    printf("\n=== Testing FSM Validation ===\n");
    test_validation();

    return 0;
}
