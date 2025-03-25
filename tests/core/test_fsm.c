/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#include <unity.h>
#include "../../include/flux_fsm_core.h"
#include "../../include/flux_fsm_event.h"
#include "../../include/flux_fsm_log.h"

flux_fsm_t* fsm;
flux_fsm_log_t* flux_fsm_log;

/* Test context */
typedef struct {
    int value;
} test_context_t;

test_context_t ctx;

/* Test states */
#define STATE_INIT  0
#define STATE_WORK  1
#define STATE_DONE  2

/* Test events */
#define EVENT_START 0
#define EVENT_STOP  1

/* Guard condition */
static int test_guard(void* context) {
    test_context_t* ctx = (test_context_t*)context;
    return ctx->value > 0;
}

/* Action function */
static void test_action(void* context) {
    test_context_t* ctx = (test_context_t*)context;
    ctx->value++;
}

/* State handler */
static void test_handler(void* context, int event) {
    test_context_t* ctx = (test_context_t*)context;
    flux_fsm_log_debug(flux_fsm_log, "Handler called with event %d, context value %d", 
                  event, ctx->value);
}

void setUp(void) {
    ctx.value = 1;
    fsm = flux_fsm_create(STATE_INIT, &ctx);
    flux_fsm_log = flux_fsm_log_create(NULL, FLUX_FSM_LOG_DEBUG);
}

void tearDown(void) {
    flux_fsm_destroy(fsm);
    flux_fsm_log_destroy(flux_fsm_log);
}

void test_flux_fsm_create(void) {
    TEST_ASSERT_NOT_NULL(fsm);
    TEST_ASSERT_EQUAL_INT(STATE_INIT, flux_fsm_get_state(fsm));
}

void test_flux_fsm_transition(void) {
    flux_fsm_transition_t trans = {
        .from = STATE_INIT,
        .event = EVENT_START,
        .to = STATE_WORK,
        .guard = test_guard,
        .action = test_action
    };

    TEST_ASSERT_EQUAL_INT(FLUX_FSM_OK, flux_fsm_add_transition(fsm, &trans));
    int trans_idx = flux_fsm_find_transition(fsm, EVENT_START);
    TEST_ASSERT_TRUE(trans_idx >= 0);
    TEST_ASSERT_EQUAL_INT(FLUX_FSM_OK, flux_fsm_exec_transition(fsm, trans_idx));
    TEST_ASSERT_EQUAL_INT(STATE_WORK, flux_fsm_get_state(fsm));
    TEST_ASSERT_EQUAL_INT(2, ctx.value); /* Action increments value */
}

void test_flux_fsm_guard_fail(void) {
    ctx.value = 0; /* Make guard condition fail */
    
    flux_fsm_transition_t trans = {
        .from = STATE_INIT,
        .event = EVENT_START,
        .to = STATE_WORK,
        .guard = test_guard,
        .action = test_action
    };

    TEST_ASSERT_EQUAL_INT(FLUX_FSM_OK, flux_fsm_add_transition(fsm, &trans));
    int trans_idx = flux_fsm_find_transition(fsm, EVENT_START);
    TEST_ASSERT_TRUE(trans_idx >= 0);
    TEST_ASSERT_EQUAL_INT(FLUX_FSM_GUARD_FAIL, flux_fsm_exec_transition(fsm, trans_idx));
    TEST_ASSERT_EQUAL_INT(STATE_INIT, flux_fsm_get_state(fsm));
}

void test_flux_fsm_invalid_event(void) {
    TEST_ASSERT_EQUAL_INT(-1, flux_fsm_find_transition(fsm, EVENT_STOP));
}

void test_flux_fsm_handler(void) {
    TEST_ASSERT_EQUAL_INT(FLUX_FSM_OK, 
        flux_fsm_add_handler(fsm, STATE_INIT, test_handler));
    
    flux_fsm_transition_t trans = {
        .from = STATE_INIT,
        .event = EVENT_START,
        .to = STATE_WORK,
        .guard = NULL,
        .action = NULL
    };

    TEST_ASSERT_EQUAL_INT(FLUX_FSM_OK, flux_fsm_add_transition(fsm, &trans));
    int trans_idx = flux_fsm_find_transition(fsm, EVENT_START);
    TEST_ASSERT_TRUE(trans_idx >= 0);
    TEST_ASSERT_EQUAL_INT(FLUX_FSM_OK, flux_fsm_exec_transition(fsm, trans_idx));
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_flux_fsm_create);
    RUN_TEST(test_flux_fsm_transition);
    RUN_TEST(test_flux_fsm_guard_fail);
    RUN_TEST(test_flux_fsm_invalid_event);
    RUN_TEST(test_flux_fsm_handler);
    
    return UNITY_END();
}
