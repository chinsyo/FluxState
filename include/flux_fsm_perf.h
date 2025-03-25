/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#ifndef FLUX_FSM_PERF_H_INCLUDED_
#define FLUX_FSM_PERF_H_INCLUDED_

#include "flux_fsm_core.h"

/* Performance metrics */
typedef struct {
    unsigned long transitions;
    unsigned long events;
    unsigned long guard_failures;
    unsigned long invalid_events;
    unsigned long invalid_states;
    double total_time;
    double avg_transition_time;
    double max_transition_time;
} flux_fsm_perf_t;

/* Performance monitoring API */
void flux_fsm_perf_init(flux_fsm_perf_t* perf);
void flux_fsm_perf_reset(flux_fsm_perf_t* perf);
void flux_fsm_perf_update(flux_fsm_perf_t* perf, double transition_time);
const char* flux_fsm_perf_to_json(const flux_fsm_perf_t* perf);
void flux_fsm_perf_output(const flux_fsm_perf_t* perf);

#endif /* FLUX_FSM_PERF_H_INCLUDED_ */
