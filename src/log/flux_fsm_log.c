/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "flux_fsm_log.h"


/* ANSI color codes */
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

/* Level strings */
static const char* level_strings[] = {
    "EMERG",
    "ALERT",
    "CRIT",
    "ERROR",
    "WARN",
    "NOTICE",
    "INFO",
    "DEBUG"
};

/* Level colors */
static const char* level_colors[] = {
    COLOR_MAGENTA,  /* EMERG */
    COLOR_MAGENTA,  /* ALERT */
    COLOR_RED,      /* CRIT */
    COLOR_RED,      /* ERROR */
    COLOR_YELLOW,   /* WARN */
    COLOR_CYAN,     /* NOTICE */
    COLOR_GREEN,    /* INFO */
    COLOR_BLUE      /* DEBUG */
};

flux_fsm_log_t* flux_fsm_log_create(const char* filename, int level) {
    flux_fsm_log_t* log = (flux_fsm_log_t*)malloc(sizeof(flux_fsm_log_t));
    if (!log) {
        return NULL;
    }

    if (filename) {
        log->file = fopen(filename, "a");
        if (!log->file) {
            free(log);
            return NULL;
        }
    } else {
        log->file = stdout;
    }

    log->level = level;
    log->use_colors = 1;  /* Enable colors by default */

    return log;
}

void flux_fsm_log_destroy(flux_fsm_log_t* log) {
    if (!log) {
        return;
    }

    if (log->file && log->file != stdout) {
        fclose(log->file);
    }
    free(log);
}

void flux_fsm_log_set_level(flux_fsm_log_t* log, int level) {
    if (log) {
        log->level = level;
    }
}

int flux_fsm_log_get_level(flux_fsm_log_t* log) {
    return log ? log->level : -1;
}

static void write_colored(FILE* file, const char* color, const char* text) {
    fprintf(file, "%s%s%s", color, text, COLOR_RESET);
}

void flux_fsm_log_write(flux_fsm_log_t* log, int level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    flux_fsm_log_write_va(log, level, fmt, args);
    va_end(args);
}

void flux_fsm_log_write_va(flux_fsm_log_t* log, int level, const char* fmt, va_list args) {
    if (!log || !fmt || level < 0 || level > FLUX_FSM_LOG_DEBUG) {
        return;
    }

    if (level > log->level) {
        return;
    }

    /* Get current time */
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", lt);

    /* Write log header */
    if (log->use_colors && log->file == stdout) {
        fprintf(log->file, "%s [", time_str);
        write_colored(log->file, level_colors[level], level_strings[level]);
        fprintf(log->file, "] ");
    } else {
        fprintf(log->file, "%s [%s] ", time_str, level_strings[level]);
    }

    /* Write log message */
    vfprintf(log->file, fmt, args);
    fprintf(log->file, "\n");
    fflush(log->file);
}
