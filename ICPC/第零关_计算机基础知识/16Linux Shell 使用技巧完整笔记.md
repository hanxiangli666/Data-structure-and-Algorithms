# Linux Shell 使用技巧完整笔记

---

## 一、为什么“标准输入 vs 命令参数”最重要

这部分虽然抽象，但它决定你写出来的命令是“能跑”还是“无效”。

- 你以为在“传参数”，实际上在“喂标准输入”，命令可能直接失败。
- 你以为是串行执行，Shell 实际在做管道并行和重定向展开。

所以先理解输入模型，再学技巧，效率会高很多。

---

## 二、后台运行程序

### 技巧 1：`cmd &`

```bash
command &
```

作用：命令转后台执行，终端不阻塞。

不用这个技巧会怎样：当前终端被占住，没法继续输入其他命令。

### 技巧 2：`(cmd &)` / `nohup cmd &`

作用：让命令尽量脱离当前交互终端影响。

不用这个技巧会怎样：你一关终端，后台任务可能跟着退出。

---

## 三、单引号 vs 双引号

### 技巧：变量和命令替换要用双引号保护

```bash
echo '$(date)'   # 原样输出
echo "$(date)"   # 执行替换
```

含空格变量建议：

```bash
echo "$var"
```

不用这个技巧会怎样：参数被错误拆分，脚本在路径含空格时经常炸。

---

## 四、alias：提效最立竿见影

### 技巧 1：定义临时别名

```bash
alias ll='ls -alF'
alias gs='git status'
```

查看当前别名：

```bash
alias
```

取消别名：

```bash
unalias ll
```

不用这个技巧会怎样：高频长命令重复输入，手误和时间成本都更高。

### 技巧 2：写入配置文件永久生效

- bash: `~/.bashrc`
- zsh: `~/.zshrc`

示例：

```bash
# ~/.bashrc 或 ~/.zshrc
alias ll='ls -alF'
alias grep='grep --color=auto'
```

使配置生效：

```bash
source ~/.bashrc
# 或
source ~/.zshrc
```

不用这个技巧会怎样：每次开新终端都要重新配置一遍。

---

## 五、环境变量：export、PATH 到底在干什么

### 5.1 export 是什么

`export` 把变量放进当前 Shell 的环境，子进程可见。

```bash
MY_TOOL_HOME=/opt/mytool
export MY_TOOL_HOME
```

也可一步写：

```bash
export MY_TOOL_HOME=/opt/mytool
```

不用这个技巧会怎样：子进程拿不到变量，程序配置“看起来写了但不生效”。

### 5.2 PATH 是什么

`PATH` 是“命令搜索目录列表”。你输入命令名时，Shell 会按 PATH 里的目录顺序查找可执行文件。

查看 PATH：

```bash
echo $PATH
```

追加目录：

```bash
export PATH="$HOME/.local/bin:$PATH"
```

永久生效写入 `~/.bashrc` 或 `~/.zshrc`。

不用这个技巧会怎样：你写好的脚本必须每次输入绝对路径才能执行。

---

## 六、sudo 找不到命令

现象：普通用户能执行，`sudo` 提示 command not found。

原因：`sudo` 使用的安全 PATH 与当前用户 PATH 可能不同。

解决：

1. 直接写绝对路径。
2. 按需调整 sudoers 的 secure_path（谨慎）。

不用这个技巧会怎样：部署脚本在本地可用，上 sudo 或线上环境直接失败。

---

## 七、花括号扩展

```bash
cp file{,.bak}
rm file{1,3,5}.txt
mv *.{c,cpp} src/
```

不用这个技巧会怎样：重复输入大量相似文件名，效率低且易输错。

---

## 八、路径与历史命令技巧

### 技巧 1：`cd -`

作用：快速返回上一个目录。

不用这个技巧会怎样：深路径反复复制粘贴，切目录成本高。

### 技巧 2：`!$`、`!*`、`!!`

- `!$`：上一条命令最后一个参数
- `!*`：上一条命令全部参数
- `!!`：整条上一命令

不用这个技巧会怎样：修复上一条命令时要整段重打。

### 技巧 3：`history | grep`

```bash
history | grep ssh
```

不用这个技巧会怎样：找旧命令要靠盲猜和翻屏。

---

## 九、标准输入 vs 命令参数（最容易混淆）

### 9.1 核心区别

| 方式 | 数据去向 | 典型场景 |
|------|----------|----------|
| 管道 `|`、重定向 `<` | 标准输入 stdin | 程序内部 read/scanf 读取 |
| `$(cmd)` | 命令行参数 argv | 程序从参数列表拿输入 |

### 9.2 典型错误

```bash
# 错误：rm 不从 stdin 读待删文件
where connect.sh | rm

# 正确：把结果当参数传入
rm $(where connect.sh)
```

### 9.3 如何快速判断命令是否读 stdin

单独运行命令：

- 会阻塞等待输入：通常支持 stdin（如 `cat`）。
- 直接报错退出：通常不读 stdin（如多数 `rm` 用法）。

不用这个技巧会怎样：管道链看起来合理，结果却“什么都没做”。

---

## 十、Shell 脚本最小入门桥梁

### 10.1 基本结构

```bash
#!/bin/bash

name="world"
echo "hello $name"
```

`#!/bin/bash` 叫 shebang，指定脚本解释器。

不用这个技巧会怎样：脚本可能被错误解释器执行，行为不一致。

### 10.2 if 语法

```bash
#!/bin/bash

if [ -f /etc/hosts ]; then
  echo "hosts exists"
else
  echo "hosts missing"
fi
```

不用这个技巧会怎样：你只能手工执行判断，无法自动化流程。

### 10.3 循环语法

```bash
#!/bin/bash

for f in *.log; do
  echo "processing $f"
done
```

不用这个技巧会怎样：批量任务只能靠人工重复。

### 10.4 让脚本可执行

```bash
chmod +x script.sh
./script.sh
```

不用这个技巧会怎样：每次都要 `bash script.sh`，且权限语义不清晰。

---

## 十一、文本处理三件套：grep / sed / awk

### 11.1 grep：筛选匹配行

```bash
grep -n "ERROR" app.log
```

用途：快速找报错行并带行号。

不用这个技巧会怎样：日志排查要人工逐行翻。

### 11.2 sed：流式替换

```bash
sed 's/http:/https:/g' urls.txt
```

用途：批量替换文本（默认输出到终端）。

就地修改：

```bash
sed -i 's/http:/https:/g' urls.txt
```

不用这个技巧会怎样：批量改文本要手工编辑，容易漏改。

### 11.3 awk：按列处理

```bash
awk '{sum += $3} END {print sum}' sales.txt
```

用途：对结构化文本做统计（如第 3 列求和）。

不用这个技巧会怎样：列处理统计只能导出到别的工具，流程变长。

---

## 十二、其他高频小技巧

### 技巧 1：`yes | cmd`

自动给交互命令喂确认输入。

不用这个技巧会怎样：批量安装/批处理被人工确认步骤卡住。

### 技巧 2：`$?`

查看上一条命令退出码（0 成功，非 0 失败）。

不用这个技巧会怎样：脚本无法可靠判断上一步是否成功。

### 技巧 3：`$$`

当前 Shell 进程 PID，常用于临时文件名去重。

```bash
tmpfile="/tmp/my_job_$$"
```

不用这个技巧会怎样：临时文件名冲突覆盖。

---

## 十三、总结速查表

```text
1) alias: 给高频命令起短名，立刻提效
2) export/PATH: 决定变量传播和命令可发现性
3) stdin vs argv: 决定管道和 $() 是否写对
4) 脚本最小骨架: shebang + if + loop + chmod +x
5) 文本三件套: grep筛选、sed替换、awk按列计算
```
