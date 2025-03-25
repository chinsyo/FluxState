/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#ifndef _FLUX_FSM_CONFIG_H_INCLUDED_
#define _FLUX_FSM_CONFIG_H_INCLUDED_

/* Version */
#define FLUX_FSM_VERSION          "1.0.0"
#define FLUX_FSM_VER             "FluxState/" FLUX_FSM_VERSION

/* Memory pool configuration */
#define FLUX_FSM_POOL_SIZE       4096
#define FLUX_FSM_MAX_ALLOC_FROM_POOL  (FLUX_FSM_POOL_SIZE - 1)

/* FSM limits */
#define FLUX_FSM_MAX_STATES      32
#define FLUX_FSM_MAX_TRANSITIONS 64
#define FLUX_FSM_MAX_HANDLERS    16

/* Build configuration */
#if defined(FLUX_FSM_BARE_METAL)
#define FLUX_FSM_NO_MALLOC
#define FLUX_FSM_STATIC_POOL
#else
#define FLUX_FSM_HAVE_MALLOC
#define FLUX_FSM_HAVE_POOL
#endif

/* Logging configuration */
#if !defined(FLUX_FSM_NO_LOG)
#define FLUX_FSM_HAVE_LOG
#endif

/* Module configuration */
#if !defined(FLUX_FSM_NO_MODULES)
#define FLUX_FSM_HAVE_HIERARCHICAL
#define FLUX_FSM_HAVE_PARALLEL
#endif

#endif /* _FLUX_FSM_CONFIG_H_INCLUDED_ */
