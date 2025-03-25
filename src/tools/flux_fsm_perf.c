#include "flux_fsm_perf.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void flux_fsm_perf_init(flux_fsm_perf_t* perf) {
    memset(perf, 0, sizeof(flux_fsm_perf_t));
}

void flux_fsm_perf_reset(flux_fsm_perf_t* perf) {
    perf->transitions = 0;
    perf->events = 0;
    perf->guard_failures = 0;
    perf->invalid_events = 0;
    perf->invalid_states = 0;
    perf->total_time = 0.0;
    perf->avg_transition_time = 0.0;
    perf->max_transition_time = 0.0;
}

void flux_fsm_perf_update(flux_fsm_perf_t* perf, double transition_time) {
    perf->transitions++;
    perf->total_time += transition_time;
    
    if (transition_time > perf->max_transition_time) {
        perf->max_transition_time = transition_time;
    }
    perf->avg_transition_time = perf->total_time / perf->transitions;
}

const char* flux_fsm_perf_to_json(const flux_fsm_perf_t* perf) {
    static char json_buffer[2048];
    snprintf(json_buffer, sizeof(json_buffer),
        "{\n"
        "  \"transitions\": %lu,\n"
        "  \"events\": %lu,\n"
        "  \"guard_failures\": %lu,\n"
        "  \"invalid_events\": %lu,\n"
        "  \"invalid_states\": %lu,\n"
        "  \"total_time\": %.3f,\n"
        "  \"avg_transition_time\": %.3f,\n"
        "  \"max_transition_time\": %.3f\n"
        "}",
        perf->transitions,
        perf->events,
        perf->guard_failures,
        perf->invalid_events,
        perf->invalid_states,
        perf->total_time,
        perf->avg_transition_time,
        perf->max_transition_time);
    return json_buffer;
}

void flux_fsm_perf_output(const flux_fsm_perf_t* perf) {
    if (perf->transitions > 0) {
        printf("%s\n", flux_fsm_perf_to_json(perf));
    }
}
