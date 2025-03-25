/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#include "flux_fsm_log.h"
#include "flux_fsm_core.h"


#include <stdio.h>

/* 定义状态 */
#define STATE_OFF   0
#define STATE_ON    1
#define STATE_ERROR 2

/* 定义事件 */
#define EVENT_POWER_ON  0
#define EVENT_POWER_OFF 1
#define EVENT_ERROR     2
#define EVENT_RESET     3

/* 设备上下文 */
typedef struct {
    int power_level;
    int error_count;
} device_context_t;

/* 守卫条件：检查电源水平是否足够 */
static int check_power_level(void* context) {
    device_context_t* ctx = (device_context_t*)context;
    return ctx->power_level >= 90;
}

/* 动作：增加电源水平 */
static void increase_power(void* context) {
    device_context_t* ctx = (device_context_t*)context;
    ctx->power_level += 10;
    printf("Power level increased to %d\n", ctx->power_level);
}

/* 状态处理器：处理错误状态 */
static void error_handler(void* context, int event) {
    device_context_t* ctx = (device_context_t*)context;
    (void)event; // 显式忽略未使用参数
    ctx->error_count++;
    printf("Error occurred! Count: %d\n", ctx->error_count);
}

int main(void) {
    /* 创建设备上下文 */
    device_context_t ctx = {
        .power_level = 85,
        .error_count = 0
    };
    
    /* 创建状态机和日志 */
    flux_fsm_t* fsm = flux_fsm_create(STATE_OFF, &ctx);
    flux_fsm_log_t* log = flux_fsm_log_create(NULL, FLUX_FSM_LOG_DEBUG);
    
    /* 添加状态转移 */
    flux_fsm_transition_t transitions[] = {
        /* OFF -> ON 转移 */
        {
            .from = STATE_OFF,
            .event = EVENT_POWER_ON,
            .to = STATE_ON,
            .guard = check_power_level,
            .action = increase_power
        },
        /* ON -> OFF 转移 */
        {
            .from = STATE_ON,
            .event = EVENT_POWER_OFF,
            .to = STATE_OFF,
            .guard = NULL,
            .action = NULL
        },
        /* ON -> ERROR 转移 */
        {
            .from = STATE_ON,
            .event = EVENT_ERROR,
            .to = STATE_ERROR,
            .guard = NULL,
            .action = NULL
        },
        /* ERROR -> OFF 转移 */
        {
            .from = STATE_ERROR,
            .event = EVENT_RESET,
            .to = STATE_OFF,
            .guard = NULL,
            .action = NULL
        }
    };
    
    /* 注册转移和处理器 */
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        flux_fsm_add_transition(fsm, &transitions[i]);
    }
    flux_fsm_add_handler(fsm, STATE_ERROR, error_handler);
    
    /* 演示状态转移 */
    printf("Current state: %d\n", flux_fsm_get_state(fsm));
    
    /* 尝试开机 - 第一次应该失败（电源不足） */
    printf("\nTrying to power on...\n");
    flux_fsm_process_event(fsm, EVENT_POWER_ON);
    printf("Current state: %d\n", flux_fsm_get_state(fsm));
    
    /* 增加电源并重试 */
    ctx.power_level = 95;
    printf("\nTrying to power on again...\n");
    flux_fsm_process_event(fsm, EVENT_POWER_ON);
    printf("Current state: %d\n", flux_fsm_get_state(fsm));
    
    /* 触发错误 */
    printf("\nTriggering error...\n");
    flux_fsm_process_event(fsm, EVENT_ERROR);
    printf("Current state: %d\n", flux_fsm_get_state(fsm));
    
    /* 重置 */
    printf("\nResetting...\n");
    flux_fsm_process_event(fsm, EVENT_RESET);
    printf("Current state: %d\n", flux_fsm_get_state(fsm));
    
    /* 清理 */
    flux_fsm_destroy(fsm);
    flux_fsm_log_destroy(log);
    
    return 0;
}
