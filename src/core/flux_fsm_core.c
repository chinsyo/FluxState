/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include "flux_fsm_core.h"

/**
 * @brief 创建有限状态机实例
 * @param init_state 初始状态标识
 * @param ctx 状态上下文指针
 * @return 成功返回状态机指针，失败返回NULL
 * @note 会分配内存并初始化状态机基础属性
 */
flux_fsm_t* flux_fsm_create(int init_state, void* ctx) {
    flux_fsm_t* fsm = (flux_fsm_t*)malloc(sizeof(flux_fsm_t));
    if (!fsm) {
        return NULL;
    }
    fsm->initial_state = init_state; 
    fsm->current_state = init_state;
    fsm->context = ctx;
    fsm->transitions = NULL;
    fsm->transition_count = 0;
    fsm->handlers = NULL;
    fsm->handler_count = 0;

    return fsm;
}

void flux_fsm_destroy(flux_fsm_t* fsm) {
    if (!fsm) {
        return;
    }

    if (fsm->transitions) {
        free(fsm->transitions);
    }
    if (fsm->handlers) {
        free(fsm->handlers);
    }
    free(fsm);
}

/**
 * @brief 查找匹配的状态转移
 * @param fsm 状态机实例指针
 * @param event 触发事件
 * @return 成功返回转移索引，未找到返回-1
 */
int flux_fsm_find_transition(flux_fsm_t* fsm, int event) {
    for (size_t i = 0; i < fsm->transition_count; i++) {
        if (fsm->transitions[i].from == fsm->current_state &&
            fsm->transitions[i].event == event) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief 处理状态事件
 * @param fsm 状态机实例指针
 * @param event 待处理事件
 * @return 状态处理结果 FLUX_FSM_OK 表示成功
 * @note 会依次执行守卫条件检查、状态转移动作和状态处理函数
 */
flux_fsm_rc_t flux_fsm_process_event(flux_fsm_t* fsm, flux_fsm_event_t event) {
    if (!fsm) {
        return FLUX_FSM_INVALID_EVENT;
    }

    int trans_idx = flux_fsm_find_transition(fsm, event);
    if (trans_idx < 0) {
        return FLUX_FSM_ERROR;
    }

    return flux_fsm_exec_transition(fsm, trans_idx);
}

flux_fsm_rc_t flux_fsm_exec_transition(flux_fsm_t* fsm, int trans_idx) {
    flux_fsm_transition_t* trans = &fsm->transitions[trans_idx];

    /* Check guard condition */
    if (trans->guard && !trans->guard(fsm->context)) {
        return FLUX_FSM_GUARD_FAIL;
    }

    /* Execute transition action */
    if (trans->action) {
        trans->action(fsm->context);
    }

    /* Execute state handler */
    if (fsm->handlers && fsm->handlers[fsm->current_state]) {
        fsm->handlers[fsm->current_state](fsm->context, trans->event);
    }

    /* Update state */
    fsm->current_state = trans->to;
    return FLUX_FSM_OK;
}

flux_fsm_rc_t flux_fsm_add_transition(flux_fsm_t* fsm, const flux_fsm_transition_t* trans) {
    if (!fsm || !trans) {
        return FLUX_FSM_INVALID_EVENT;
    }

    flux_fsm_transition_t* new_trans = realloc(fsm->transitions,
        (fsm->transition_count + 1) * sizeof(flux_fsm_transition_t));
    if (!new_trans) {
        return FLUX_FSM_ERROR;
    }

    fsm->transitions = new_trans;
    memcpy(&fsm->transitions[fsm->transition_count], trans,
           sizeof(flux_fsm_transition_t));
    fsm->transition_count++;

    return FLUX_FSM_OK;
}

flux_fsm_rc_t flux_fsm_add_handler(flux_fsm_t* fsm, int state, flux_fsm_state_handler_t handler) {
    if (!fsm || !handler) {
        return FLUX_FSM_INVALID_EVENT;
    }

    if ((size_t)state >= fsm->handler_count) {
        flux_fsm_state_handler_t* new_handlers = realloc(fsm->handlers,
            (state + 1) * sizeof(flux_fsm_state_handler_t));
        if (!new_handlers) {
            return FLUX_FSM_ERROR;
        }

        /* Initialize new handlers to NULL */
        memset(new_handlers + fsm->handler_count, 0,
               (state + 1 - fsm->handler_count) * sizeof(flux_fsm_state_handler_t));

        fsm->handlers = new_handlers;
        fsm->handler_count = state + 1;
    }

    fsm->handlers[state] = handler;
    return FLUX_FSM_OK;
}

int flux_fsm_get_state(const flux_fsm_t* fsm) {
    return fsm ? fsm->current_state : FLUX_FSM_INVALID_EVENT;
}
