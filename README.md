# coroutine

## CRIU 内存转储分析 (CRIU Memory Dump Analysis)

本仓库包含关于 CRIU (Checkpoint/Restore In Userspace) 技术的内存转储行为分析和演示程序。

### 核心问题 (Core Question)

**criu技术，如果内存没有被used，他会dump多大的数据？**

**答案**: CRIU **只会转储实际使用的内存页**，未使用的内存页**不会被包含**在转储文件中。因此，如果内存未被使用，转储大小接近 0 (仅包含元数据)。

### 文档和资源 (Documentation & Resources)

- 📄 [CRIU_MEMORY_DUMP_ANALYSIS.md](CRIU_MEMORY_DUMP_ANALYSIS.md) - 详细的分析文档
- 📖 [USAGE_GUIDE.md](USAGE_GUIDE.md) - 完整的实验指南
- 💻 [criu_memory_demo.cpp](criu_memory_demo.cpp) - C++ 演示程序

### 快速开始 (Quick Start)

```bash
# 1. 编译演示程序
make

# 2. 运行演示（选择一个场景）
./criu_memory_demo unused      # 未使用内存场景
./criu_memory_demo partial     # 部分使用场景  
./criu_memory_demo full        # 完全使用场景

# 3. 在另一个终端执行 CRIU 检查点
sudo criu dump -t <PID> -D ./checkpoint --shell-job

# 4. 查看转储大小
du -sh ./checkpoint/
```

### 关键发现 (Key Findings)

| 场景 | 分配内存 | 使用内存 | 转储大小 |
|------|---------|---------|---------|
| 未使用 | 100 MB | 0 MB | **~0 MB** |
| 部分使用 | 100 MB | 10 MB | **~10 MB** |
| 完全使用 | 100 MB | 100 MB | **~100 MB** |

**结论**: 转储大小 = 实际使用的内存大小，而非分配的内存大小。

---

## English Summary

This repository contains an analysis and demonstration of CRIU (Checkpoint/Restore In Userspace) memory dump behavior.

**Question**: How much data does CRIU dump if memory is not used?

**Answer**: CRIU **only dumps memory pages that are actually in use**. Unused memory pages are **NOT included** in the dump. Therefore, if memory is allocated but not used, the dump size is **close to 0** (only metadata).

See [CRIU_MEMORY_DUMP_ANALYSIS.md](CRIU_MEMORY_DUMP_ANALYSIS.md) for detailed analysis.