# FluxState

一个轻量级、高性能、可扩展的有限状态机（FSM）实现。

## 特性

- 轻量级：核心代码简洁，内存占用小
- 高性能：针对嵌入式场景优化
- 可扩展：支持分层状态机和并行状态机
- 易用性：简单直观的API设计
- 可靠性：完整的单元测试覆盖

## 快速开始

### 构建

```bash
mkdir build 
cd build
cmake ..
make
```

### 运行示例

```bash
# 运行基础状态机示例
./examples/basic/basic_fsm

```

## 文档

详细文档请参考 [doc/design.md](doc/design.md)

## 许可证

MIT License

