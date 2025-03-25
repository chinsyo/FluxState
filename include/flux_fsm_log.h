/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

 #ifndef _FLUX_FSM_LOG_H_INCLUDED_
 #define _FLUX_FSM_LOG_H_INCLUDED_
 
 #include <stdarg.h>
 #include <stdio.h>
 
 /* Log levels */
 #define FLUX_FSM_LOG_EMERG   0
 #define FLUX_FSM_LOG_ALERT   1
 #define FLUX_FSM_LOG_CRIT    2
 #define FLUX_FSM_LOG_ERR     3
 #define FLUX_FSM_LOG_WARN    4
 #define FLUX_FSM_LOG_NOTICE  5
 #define FLUX_FSM_LOG_INFO    6
 #define FLUX_FSM_LOG_DEBUG   7
 
 /* Log configuration */
 typedef struct {
     FILE* file;           /* Log file handle */
     int level;            /* Current log level */
     int use_colors;       /* Enable colored output */
 } flux_fsm_log_t;
 
 /* Log API */
 flux_fsm_log_t* flux_fsm_log_create(const char* filename, int level);
 void flux_fsm_log_destroy(flux_fsm_log_t* log);
 
 void flux_fsm_log_set_level(flux_fsm_log_t* log, int level);
 int flux_fsm_log_get_level(flux_fsm_log_t* log);
 
 void flux_fsm_log_write(flux_fsm_log_t* log, int level, const char* fmt, ...);
 void flux_fsm_log_write_va(flux_fsm_log_t* log, int level, const char* fmt, va_list args);
 
 /* Helper macros */
 #define flux_fsm_log_emergency(log, ...) flux_fsm_log_write(log, FLUX_FSM_LOG_EMERG, __VA_ARGS__)
 #define flux_fsm_log_alert(log, ...)     flux_fsm_log_write(log, FLUX_FSM_LOG_ALERT, __VA_ARGS__)
 #define flux_fsm_log_critical(log, ...)  flux_fsm_log_write(log, FLUX_FSM_LOG_CRIT, __VA_ARGS__)
 #define flux_fsm_log_error(log, ...)     flux_fsm_log_write(log, FLUX_FSM_LOG_ERR, __VA_ARGS__)
 #define flux_fsm_log_warning(log, ...)   flux_fsm_log_write(log, FLUX_FSM_LOG_WARN, __VA_ARGS__)
 #define flux_fsm_log_notice(log, ...)    flux_fsm_log_write(log, FLUX_FSM_LOG_NOTICE, __VA_ARGS__)
 #define flux_fsm_log_info(log, ...)      flux_fsm_log_write(log, FLUX_FSM_LOG_INFO, __VA_ARGS__)
 #define flux_fsm_log_debug(log, ...)     flux_fsm_log_write(log, FLUX_FSM_LOG_DEBUG, __VA_ARGS__)
 
 #endif /* _FLUX_FSM_LOG_H_INCLUDED_ */
