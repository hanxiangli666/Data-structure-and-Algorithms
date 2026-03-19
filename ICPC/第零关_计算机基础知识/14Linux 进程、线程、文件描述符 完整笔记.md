# Linux 进程、线程、文件描述符 完整笔记

---

## 一、背景知识：计算机内存空间整体模型

在讲进程之前，先建立全局视角。

```text
┌──────────────────────────────────────────┐
│              内存空间                    │
├──────────────────────────────────────────┤
│   [ 进程A ]   [ 进程B ]   [ 进程C ]      │  <- 用户空间
├──────────────────────────────────────────┤
│               内核空间                    │  <- 用户不可直接访问
└──────────────────────────────────────────┘
       ↑                        ↑
     磁盘                  输入输出设备
```

| 区域 | 说明 |
|------|------|
| 用户空间 | 进程自己的代码、堆、栈、数据 |
| 内核空间 | 系统调用、调度、驱动、文件系统等核心能力 |

关键点：

- 应用不能直接操作硬件，要通过系统调用让内核代办。
- 进程隔离是操作系统稳定性的核心。

---

## 二、进程是什么

### 2.1 可执行文件不等于进程

可执行文件只是磁盘上的静态文件，运行后才由操作系统加载并变成进程。

### 2.2 进程在内核中的表示

Linux 用 `task_struct` 表示一个进程/任务。

```c
struct task_struct {
    long state;
    struct mm_struct *mm;
    pid_t pid;
    struct task_struct *parent;
    struct list_head children;
    struct files_struct *files;
};
```

高频字段：

- `mm`：虚拟内存
- `files`：该进程打开的文件描述符表

---

## 三、进程状态机（运维高频）

很多人只记得“运行/等待”，但实际排障必须看完整状态。

### 3.1 常见状态

| 状态 | 含义 | 常见表现 |
|------|------|----------|
| R | Running / Runnable | 正在运行或等待 CPU 调度 |
| S | Interruptible Sleep | 可中断睡眠（最常见） |
| D | Uninterruptible Sleep | 不可中断睡眠（常见于 IO 阻塞） |
| T | Stopped / Traced | 被停止（如调试、SIGSTOP） |
| Z | Zombie | 子进程已退出，父进程未回收 |

### 3.2 僵尸进程（Zombie）

定义：子进程已经结束，但父进程还没 `wait()/waitpid()` 回收退出状态，进程表里会留下 `Z`。

风险：

- 少量僵尸影响不大。
- 大量僵尸会耗尽 PID/进程表资源。

### 3.3 孤儿进程（Orphan）

定义：父进程先退出，子进程仍在运行。

行为：

- 孤儿进程会被 `init/systemd`（PID 1）接管并最终回收。

### 3.4 怎么看进程状态（ps aux）

```bash
ps aux
```

重点看 `STAT` 列：

- `R` 运行中
- `S` 睡眠
- `D` 不可中断等待
- `T` 停止
- `Z` 僵尸

可快速筛僵尸：

```bash
ps aux | grep ' Z '
```

---

## 四、文件描述符是什么

每个进程都有一个打开文件表，文件描述符（FD）就是索引号。

默认前三个 FD：

- `0` 标准输入 stdin
- `1` 标准输出 stdout
- `2` 标准错误 stderr

可以把它理解成“插槽编号”：程序通过 FD 读写文件、终端、socket、管道。

---

## 五、重定向与管道的本质

### 5.1 输入重定向 `<`

```bash
command < input.txt
```

本质：把 FD 0 从键盘切到 `input.txt`。

### 5.2 输出重定向 `>`

```bash
command > output.txt
```

本质：把 FD 1 从终端切到 `output.txt`。

### 5.3 管道 `|`

```bash
cmd1 | cmd2
```

本质：`cmd1` 的 FD 1 接到 `cmd2` 的 FD 0。

---

## 六、信号（Signal）基础：进程管理最常用交互

信号是内核给进程发送的异步通知。

### 6.1 常见信号

| 信号 | 编号 | 含义 | 常见触发 |
|------|------|------|----------|
| SIGINT | 2 | 中断进程 | 终端按 Ctrl+C |
| SIGTERM | 15 | 请求优雅退出 | `kill <pid>` 默认 |
| SIGKILL | 9 | 强制立即杀死 | `kill -9 <pid>` |

### 6.2 SIGTERM vs SIGKILL

- SIGTERM：给进程“收尾机会”（释放资源、写回数据）。
- SIGKILL：内核直接终止，进程无法捕获或清理。

实战建议：

1. 先发 SIGTERM。
2. 等待短时间。
3. 仍不退出再用 SIGKILL。

---

## 七、/proc/{pid} 的实战用途（把理论落地）

`/proc` 是内核暴露给用户态的运行时视图。

### 7.1 看进程内存布局

```bash
cat /proc/<pid>/maps
```

用途：

- 查看代码段、堆、栈、共享库映射。
- 排查内存映射异常、动态库加载问题。

### 7.2 看进程打开了哪些 FD

```bash
ls -l /proc/<pid>/fd
```

用途：

- 直观看到 FD 指向哪些文件、socket、pipe。
- 排查“文件句柄泄漏”“端口占用来源”。

示例：

```bash
ls -l /proc/1234/fd
```

如果看到大量重复 FD 指向同类文件或 socket，常提示资源泄漏。

---

## 八、线程是什么（Linux 视角）

Linux 内核视角下，线程和进程都可看作 task；关键区别是“是否共享资源”。

| 对比项 | 进程（fork） | 线程（pthread） |
|--------|--------------|-----------------|
| 地址空间 mm | 独立 | 共享 |
| 打开文件 files | 通常独立 | 共享 |
| 通信成本 | 高（IPC） | 低（共享内存） |
| 隔离性 | 高 | 低 |

---

## 九、线程安全反例：为什么必须加锁

下面用 Python 展示一个最小竞态条件示例。

> 说明：`counter += 1` 不是原子操作，多线程下可能丢失更新。

```python
import threading

counter = 0


def worker():
    global counter
    for _ in range(10000):
        counter += 1


t1 = threading.Thread(target=worker)
t2 = threading.Thread(target=worker)

t1.start()
t2.start()
t1.join()
t2.join()

print(counter)  # 期望 20000，但多次运行可能小于 20000
```

加锁版本：

```python
import threading

counter = 0
lock = threading.Lock()


def worker():
    global counter
    for _ in range(10000):
        with lock:
            counter += 1


t1 = threading.Thread(target=worker)
t2 = threading.Thread(target=worker)

t1.start()
t2.start()
t1.join()
t2.join()

print(counter)  # 稳定 20000
```

这就是“共享数据 + 并发写入”需要锁的最直观证据。

---

## 十、补充：多进程性能优化 Copy-on-Write

`fork()` 并不会立即复制全部内存，而是采用写时复制（COW）：

1. 父子进程先共享只读页。
2. 发生写入时才复制对应页。

好处：

- 降低 fork 成本。
- 保留进程隔离优势。

---

## 十一、总结速查

```text
1) 进程是内核管理的数据结构（task），不是可执行文件本身
2) 进程状态要会看：R/S/D/T/Z，特别关注僵尸进程 Z
3) 信号是进程管理基本手段：Ctrl+C=SIGINT，kill默认SIGTERM，kill -9=SIGKILL
4) 文件描述符是进程资源访问统一入口：0/1/2 + /proc/<pid>/fd
5) /proc/<pid>/maps 与 /proc/<pid>/fd 是排障高频工具
6) 多线程共享资源效率高，但必须用锁防竞态
```

---

## 十二、延伸阅读

- 前置：Linux 文件系统、Shell 命令
- 后续：文件描述符泄漏排查、epoll、进程调度与上下文切换
