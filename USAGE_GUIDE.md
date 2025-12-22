# CRIU 内存转储实验指南 (CRIU Memory Dump Experiment Guide)

## 快速开始 (Quick Start)

### 1. 编译程序 (Build the Program)

```bash
# 使用 Makefile
make

# 或者直接编译
g++ -o criu_memory_demo criu_memory_demo.cpp -std=c++11
```

### 2. 安装 CRIU (Install CRIU)

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install criu

# CentOS/RHEL
sudo yum install criu

# Fedora
sudo dnf install criu

# 验证安装
criu --version
```

### 3. 运行实验 (Run Experiments)

## 实验场景 (Experiment Scenarios)

### 场景 1: 未使用的内存 (Unused Memory)

**问题**: 分配了内存但不使用，CRIU会转储多少数据？
**答案**: 几乎为0 (仅元数据)

```bash
# 终端 1: 运行程序
./criu_memory_demo unused

# 终端 2: 执行检查点
sudo criu dump -t <PID> -D ./checkpoint-unused --shell-job

# 检查转储大小
du -sh ./checkpoint-unused/
ls -lh ./checkpoint-unused/pages-*.img
```

**预期结果**: pages-*.img 文件几乎为空或很小（<1MB）

---

### 场景 2: 部分使用的内存 (Partially Used Memory)

**问题**: 分配100MB，只使用10MB，会转储多少？
**答案**: 约10MB

```bash
# 终端 1: 运行程序
./criu_memory_demo partial

# 终端 2: 执行检查点
sudo criu dump -t <PID> -D ./checkpoint-partial --shell-job

# 检查转储大小
du -sh ./checkpoint-partial/
ls -lh ./checkpoint-partial/pages-*.img
```

**预期结果**: pages-*.img 约10MB

---

### 场景 3: 完全使用的内存 (Fully Used Memory)

**问题**: 分配并使用全部100MB，会转储多少？
**答案**: 约100MB

```bash
# 终端 1: 运行程序
./criu_memory_demo full

# 终端 2: 执行检查点
sudo criu dump -t <PID> -D ./checkpoint-full --shell-job

# 检查转储大小
du -sh ./checkpoint-full/
ls -lh ./checkpoint-full/pages-*.img
```

**预期结果**: pages-*.img 约100MB

---

### 场景 4: 渐进式使用 (Progressive Usage)

**问题**: 观察内存使用量逐步增加时的转储大小变化
**答案**: 转储大小与实际使用量成正比

```bash
# 运行程序（自动演示）
./criu_memory_demo progressive

# 在不同阶段执行检查点
sudo criu dump -t <PID> -D ./checkpoint-step1 --shell-job
# ... (按提示在不同步骤执行)
```

---

## 详细步骤示例 (Detailed Step-by-Step Example)

### 完整的检查点和恢复流程

```bash
# 1. 编译程序
make

# 2. 在后台运行程序
./criu_memory_demo partial &
PID=$!
echo "Process PID: $PID"

# 3. 等待程序初始化
sleep 2

# 4. 创建检查点目录
mkdir -p checkpoint-test

# 5. 执行检查点
sudo criu dump -t $PID -D ./checkpoint-test --shell-job --leave-running

# 6. 查看转储文件
echo "=== Checkpoint Files ==="
ls -lh ./checkpoint-test/

echo "=== Memory Dump Size ==="
du -sh ./checkpoint-test/
du -h ./checkpoint-test/pages-*.img

echo "=== Detailed Statistics ==="
sudo criu show ./checkpoint-test/

# 7. 清理（可选）
# kill $PID
# make clean
```

---

## 高级用法 (Advanced Usage)

### 1. 查看详细统计信息

```bash
sudo criu dump -t <PID> -D ./checkpoint --shell-job --display-stats
```

### 2. 增量检查点

```bash
# 第一次完整检查点
sudo criu dump -t <PID> -D ./checkpoint1 --shell-job --leave-running --track-mem

# 后续增量检查点
sudo criu dump -t <PID> -D ./checkpoint2 --shell-job --leave-running --track-mem --prev-images-dir ../checkpoint1
```

### 3. 检查内存页信息

```bash
# 查看进程内存映射
cat /proc/<PID>/smaps

# 查看页面映射
sudo cat /proc/<PID>/pagemap | hexdump -C | head

# 使用 criu 工具分析
sudo criu show ./checkpoint/
```

### 4. 恢复检查点

```bash
# 恢复到之前的状态
sudo criu restore -D ./checkpoint --shell-job
```

---

## 预期结果总结 (Expected Results Summary)

| 场景 | 分配内存 | 使用内存 | 转储大小 | 原因 |
|------|---------|---------|---------|------|
| 未使用 | 100 MB | 0 MB | ~0 MB | 仅转储已使用的页面 |
| 部分使用 | 100 MB | 10 MB | ~10 MB | 只转储被访问的页面 |
| 完全使用 | 100 MB | 100 MB | ~100 MB | 转储所有使用的页面 |
| 渐进式 | 100 MB | 0→100 MB | 逐步增加 | 转储大小跟随使用量 |

---

## 故障排查 (Troubleshooting)

### 问题 1: CRIU 权限错误

```bash
# 解决方案: 使用 sudo
sudo criu dump -t <PID> ...
```

### 问题 2: 进程无法被转储

```bash
# 检查进程状态
ps aux | grep criu_memory_demo

# 检查 CRIU 日志
sudo criu check
sudo criu check --all
```

### 问题 3: 内核不支持

```bash
# 检查内核配置
sudo criu check --feature mem_dirty_track
```

### 问题 4: SELinux/AppArmor 干扰

```bash
# 临时禁用 SELinux
sudo setenforce 0

# 或禁用 AppArmor
sudo systemctl stop apparmor
```

---

## 学习要点 (Key Takeaways)

1. ✅ **CRIU 智能转储**: 只转储实际使用的内存页
2. ✅ **大小 = 使用量**: 转储大小与实际内存使用成正比
3. ✅ **优化友好**: 预分配但未使用的内存不增加转储大小
4. ✅ **透明行为**: 应用无需修改即可利用此优化
5. ✅ **生产可用**: 适合容器、HPC等需要检查点的场景

---

## 参考命令速查 (Command Reference)

```bash
# 编译
make

# 运行场景
./criu_memory_demo unused|partial|full|progressive|all

# 创建检查点
sudo criu dump -t <PID> -D <DIR> --shell-job [--leave-running]

# 恢复检查点
sudo criu restore -D <DIR> --shell-job

# 查看统计
sudo criu dump -t <PID> -D <DIR> --display-stats

# 检查转储大小
du -sh <DIR>/
ls -lh <DIR>/pages-*.img

# 清理
make clean
```

---

## 相关资源 (Related Resources)

- [CRIU Official Website](https://criu.org/)
- [CRIU Memory Dumping Documentation](https://criu.org/Memory_dumping_and_restoring)
- [CRIU GitHub](https://github.com/checkpoint-restore/criu)
- [Linux Kernel Memory Management](https://www.kernel.org/doc/html/latest/admin-guide/mm/index.html)
