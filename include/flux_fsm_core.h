/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#ifndef FLUX_FSM_CORE_H_INCLUDED_
#define FLUX_FSM_CORE_H_INCLUDED_

#include "flux_fsm_config.h"
#include "flux_fsm_event.h"
#include <stddef.h>

typedef void (*flux_fsm_state_handler_t)(void* ctx, flux_fsm_event_t event);

typedef void (*flux_fsm_handler_pt)(void* ctx, flux_fsm_event_t event);

typedef struct {
    int from;
    int event;
    int to;
    int (*guard)(void*);
    void (*action)(void*);
} flux_fsm_transition_t;

/**
 * @struct flux_fsm
 * @brief 有限状态机核心结构体
 * 
 * @var initial_state 初始状态
 * @var current_state 当前状态
 * @var context 状态上下文指针
 * @var transitions 状态转移表指针
 * @var transition_count 转移规则数量
 * @var handlers 状态处理器数组
 * @var handler_count 处理器数量
 * @var state_count 状态数量
 */
typedef struct flux_fsm {
    int initial_state;
    int current_state;
    void* context;
    flux_fsm_transition_t* transitions;
    size_t transition_count;
    flux_fsm_state_handler_t* handlers;
    size_t handler_count;
    size_t state_count; // 新增的状态数量属性
} flux_fsm_t;

/* 状态机内存池接口 */
#if defined(FLUX_FSM_HAVE_POOL)
typedef struct flux_fsm_pool_s flux_fsm_pool_t;

flux_fsm_pool_t* flux_fsm_pool_create(size_t size);
void flux_fsm_pool_destroy(flux_fsm_pool_t* pool);
void* flux_fsm_palloc(flux_fsm_pool_t* pool, size_t size);
#endif

/* 状态机核心接口 */
flux_fsm_t* flux_fsm_create(int initial_state, void* context);
void flux_fsm_destroy(flux_fsm_t* fsm);
flux_fsm_rc_t flux_fsm_add_transition(flux_fsm_t* fsm, const flux_fsm_transition_t* trans);
flux_fsm_rc_t flux_fsm_add_handler(flux_fsm_t* fsm, int state, flux_fsm_handler_pt handler);
flux_fsm_rc_t flux_fsm_process_event(flux_fsm_t* fsm, flux_fsm_event_t event);
flux_fsm_rc_t flux_fsm_exec_transition(flux_fsm_t* fsm, int trans_idx);
int flux_fsm_find_transition(flux_fsm_t* fsm, int event);
int flux_fsm_get_state(const flux_fsm_t* fsm);

/* 特殊状态定义 */
#define FLUX_FSM_ANY_STATE    -1

#endif /* FLUX_FSM_CORE_H_INCLUDED_ */
