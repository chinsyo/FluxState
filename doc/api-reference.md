# FluxState API 参考手册

## 核心模块

### 数据结构
```doxygen
/// 有限状态机转移规则结构体
struct flux_fsm_transition_t {
    int from;          ///< 源状态标识
    int event;         ///< 触发事件标识
    int to;            ///< 目标状态标识
    int (*guard)(void*);  ///< 守卫条件函数指针
    void (*action)(void*); ///< 转移动作函数指针
};
```

## API函数

### 生命周期管理
```doxygen
/**
 * @brief 初始化状态机上下文
 * @param[in] config 配置参数结构体指针
 * @param[in] pool 内存池对象（可选）
 * @return 状态机上下文句柄
 * @retval NULL 初始化失败
 */
flux_fsm_ctx_t* flux_fsm_init(const fsm_config_t* config, fsm_pool_t* pool);
```

## 使用示例
```c
/* 创建状态机实例 */
flux_fsm_t* fsm = flux_fsm_create(INIT_STATE, NULL);

/* 添加状态转移规则 */
flux_fsm_transition_t transition = {
    .from = STATE_A,
    .event = EVENT_X,
    .to = STATE_B,
    .guard = voltage_check,
    .action = led_blink
};
flux_fsm_add_transition(fsm, &transition);
```

> 注意：本文档需通过Doxygen生成完整API文档，运行`doxygen Doxyfile`生成HTML格式文档。