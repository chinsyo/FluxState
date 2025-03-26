#include "flux_fsm_viz.h"
#include "flux_fsm_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 验证 FSM 的有效性 */
flux_fsm_viz_validation_result_t flux_fsm_viz_validate(const flux_fsm_t* fsm) {
    flux_fsm_viz_validation_result_t result = {0, NULL};
    
    /* 检查初始状态有效性 */
    if (fsm->current_state < 0 || fsm->current_state >= (int)fsm->state_count) {
        result.error_code = 1;
        result.description = "初始状态无效";
        return result;
    }

    /* 检查状态转换完整性 */
    for (size_t i = 0; i < fsm->transition_count; ++i) {
        const flux_fsm_transition_t* t = &fsm->transitions[i];
        
        if (t->from < 0 || t->from >= (int)fsm->state_count) {
            result.error_code = 2;
            result.description = "转换源状态无效";
            return result;
        }
        
        if (t->to < 0 || t->to >= (int)fsm->state_count) {
            result.error_code = 3;
            result.description = "转换目标状态无效";
            return result;
        }
    }

    return result;
}

/* 生成 FSM 的可视化图表 */
char* flux_fsm_viz_generate(flux_fsm_t* fsm, flux_fsm_viz_config_t* cfg) {
    if (!fsm || !cfg) {
        return NULL;
    }

    /* 创建 DOT 格式的字符串 */
    FILE* temp_file = tmpfile();
    if (!temp_file) {
        return NULL;
    }

    fprintf(temp_file, "digraph FSM {\n");

    /* 设置全局属性 */
    if (cfg->title) {
        fprintf(temp_file, "    label=\"%s\";\n", cfg->title);
        fprintf(temp_file, "    labelloc=top;\n");
    }
    if (cfg->font_name) {
        fprintf(temp_file, "    fontname=\"%s\";\n", cfg->font_name);
    }
    if (cfg->font_size > 0) {
        fprintf(temp_file, "    fontsize=%d;\n", cfg->font_size);
    }
    if (cfg->bgcolor) {
        fprintf(temp_file, "    bgcolor=\"%s\";\n", cfg->bgcolor);
    }

    /* 设置节点和边的默认样式 */
    if (cfg->node_shape) {
        fprintf(temp_file, "    node [shape=%s];\n", cfg->node_shape);
    }
    if (cfg->edge_style) {
        fprintf(temp_file, "    edge [style=%s];\n", cfg->edge_style);
    }

    /* 添加状态节点 */
    for (size_t i = 0; i < fsm->state_count; ++i) {
        fprintf(temp_file, "    %zu;\n", i);
    }

    /* 添加状态转换 */
    for (size_t i = 0; i < fsm->transition_count; ++i) {
        const flux_fsm_transition_t* t = &fsm->transitions[i];
        fprintf(temp_file, "    %d -> %d;\n", t->from, t->to);
    }

    fprintf(temp_file, "}\n");

    /* 获取 DOT 数据 */
    fseek(temp_file, 0, SEEK_END);
    long size = ftell(temp_file);
    fseek(temp_file, 0, SEEK_SET);

    char* dot_data = (char*)malloc(size + 1);
    if (!dot_data) {
        fclose(temp_file);
        return NULL;
    }

    if (fread(dot_data, 1, size, temp_file) != (size_t)size) {
        free(dot_data);
        fclose(temp_file);
        return NULL;
    }
    dot_data[size] = '\0';

    fclose(temp_file);

    /* 如果需要其他格式（如 JSON 或 YAML），可以在此扩展 */
    if (cfg->format == FLUX_FSM_VIZ_DOT) {
        return dot_data;
    } else {
        free(dot_data);
        return NULL; // 暂不支持其他格式
    }
}

/* 释放生成的可视化数据 */
void flux_fsm_viz_free(char* viz_data) {
    if (viz_data) {
        free(viz_data);
    }
}

/* 初始化可视化配置 */
void flux_fsm_viz_init(flux_fsm_viz_config_t* cfg) {
    if (!cfg) {
        return;
    }
    
    memset(cfg, 0, sizeof(flux_fsm_viz_config_t));
    cfg->format = FLUX_FSM_VIZ_DOT; /* 默认使用DOT格式 */
    cfg->font_size = 12;            /* 默认字体大小 */
    cfg->node_shape = "ellipse";    /* 默认节点形状 */
    cfg->edge_style = "solid";      /* 默认边线样式 */
}

/* 设置全局可视化配置 */
static flux_fsm_viz_config_t g_viz_config = {0};

void flux_fsm_viz_set_config(flux_fsm_viz_config_t* cfg) {
    if (cfg) {
        memcpy(&g_viz_config, cfg, sizeof(flux_fsm_viz_config_t));
    }
}

/* 导出状态机图表到文件 */
int flux_fsm_viz_export(flux_fsm_t* fsm, const char* filename) {
    if (!fsm || !filename) {
        return -1;
    }
    
    /* 使用全局配置或创建默认配置 */
    flux_fsm_viz_config_t cfg;
    if (g_viz_config.format != 0) {
        memcpy(&cfg, &g_viz_config, sizeof(flux_fsm_viz_config_t));
    } else {
        flux_fsm_viz_init(&cfg);
    }
    
    /* 生成DOT格式数据 */
    char* dot_data = flux_fsm_viz_generate(fsm, &cfg);
    if (!dot_data) {
        return -1;
    }
    
    /* 写入文件 */
    FILE* file = fopen(filename, "w");
    if (!file) {
        flux_fsm_viz_free(dot_data);
        return -1;
    }
    
    fprintf(file, "%s", dot_data);
    fclose(file);
    
    flux_fsm_viz_free(dot_data);
    return 0;
}
