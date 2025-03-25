/*
 * Copyright (C) 2024 FluxState. All rights reserved.
 */

#ifndef _FLUX_FSM_VIZ_H_INCLUDED_
#define _FLUX_FSM_VIZ_H_INCLUDED_

#include "flux_fsm_core.h"

/* Visualization output formats */
#define FLUX_FSM_VIZ_DOT    0  /* Graphviz DOT format */
#define FLUX_FSM_VIZ_JSON   1  /* JSON format */
#define FLUX_FSM_VIZ_YAML   2  /* YAML format */

/* Visualization options */
typedef struct {
    int format;                /* 输出格式 */
    const char* title;         /* 图表标题 */
    const char* font_name;     /* 字体名称 */
    int font_size;             /* 字体大小 */
    const char* node_shape;    /* 节点形状 */
    const char* edge_style;    /* 边线样式 */
    const char* bgcolor;       /* 背景颜色 */
} flux_fsm_viz_config_t;

/* 验证结果结构体 */
typedef struct {
    int error_code;          /* 错误代码 */
    const char* description; /* 错误描述 */
} flux_fsm_viz_validation_result_t;

/* Visualization API */
flux_fsm_viz_validation_result_t flux_fsm_viz_validate(const flux_fsm_t* fsm);
char* flux_fsm_viz_generate(flux_fsm_t* fsm, flux_fsm_viz_config_t* cfg);
void flux_fsm_viz_free(char* viz_data);

/* Configuration API */
void flux_fsm_viz_init(flux_fsm_viz_config_t* cfg);
void flux_fsm_viz_set_config(flux_fsm_viz_config_t* cfg);
int flux_fsm_viz_export(flux_fsm_t* fsm, const char* filename);

#endif /* FLUX_FSM_VIZ_H_INCLUDED_ */
