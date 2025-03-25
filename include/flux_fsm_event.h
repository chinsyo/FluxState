/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#ifndef _FLUX_FSM_EVENT_H_INCLUDED_
#define _FLUX_FSM_EVENT_H_INCLUDED_

#include "flux_fsm_config.h"

/* Event type */
typedef int flux_fsm_event_t;

/* Handler function pointer */
typedef void (*flux_fsm_handler_pt)(void* context, flux_fsm_event_t event);

/* Guard function pointer */
typedef int (*flux_fsm_guard_pt)(void* context);

/* Action function pointer */
typedef void (*flux_fsm_action_pt)(void* context);

/* Return codes */
typedef enum {
    FLUX_FSM_OK = 0,
    FLUX_FSM_ERROR = -1,
    FLUX_FSM_GUARD_FAIL = -2,
    FLUX_FSM_INVALID_EVENT = -3,
    FLUX_FSM_INVALID_STATE = -4
} flux_fsm_rc_t;

#endif /* _FLUX_FSM_EVENT_H_INCLUDED_ */
