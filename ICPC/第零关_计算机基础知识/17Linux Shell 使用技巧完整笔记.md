# Linux Shell 使用技巧完整笔记

---

## 一、标准输入 vs 命令参数（最容易混淆）

### 核心区别

| 方式                           | 原理                                       | 使用场景                               |
| ------------------------------ | ------------------------------------------ | -------------------------------------- |
| 管道 `\|`和重定向 `<` `>` | 数据作为程序的**标准输入** （stdin） | 程序内部用 `scanf`、`readline`读取 |
| `$(cmd)`                     | 命令输出作为**命令行参数**传入       | 程序的 `main(args)`接收              |

> **类比理解：**
>
> 标准输入就像餐厅的"传菜口"——厨房（程序）主动开口等待食材进来；
>
> 命令行参数就像点菜单——服务员在你下单时就把信息写进单子，厨房直接看单子做菜。

### 典型错误案例

```bash
# ❌ 错误：rm 不接受标准输入，这样做没有效果
$ where connect.sh | rm

# ✅ 正确：用 $() 把结果作为参数传给 rm
$ rm $(where connect.sh)
```

### 如何判断一个命令接不接受标准输入？

**方法：** 单独运行这个命令，看终端是否阻塞。

```bash
$ cat          # 终端阻塞，等待输入 → 接受标准输入 ✅
$ rm           # 报错退出，不阻塞 → 不接受标准输入 ❌
```

### cat 命令的三种用法对比

```bash
$ cat filename          # 参数方式：读取文件
$ cat < filename        # 重定向方式：读取文件（等效）
$ echo 'hello' | cat    # 管道方式：读取标准输入
```

---

## 二、后台运行程序

### 问题场景

```bash
# 启动服务后，终端被阻塞，无法继续输入命令
$ python manager.py runserver 0.0.0.0
Listening on 0.0.0.0:8080...
```

### 三种方式对比

| 方式         | 命令        | 终端阻塞？ | 退出终端后继续运行？ |
| ------------ | ----------- | ---------- | -------------------- |
| 普通运行     | `cmd`     | ✅ 阻塞    | ❌ 停止              |
| 后台运行     | `cmd &`   | ❌ 不阻塞  | ❌ 停止              |
| 脱离终端运行 | `(cmd &)` | ❌ 不阻塞  | ✅ 继续运行          |

### 底层原理

```
终端（shell 进程）
    ├── 子进程 A（你运行的命令）
    ├── 子进程 B
    └── 子进程 C

关闭终端 → shell 进程退出 → 所有子进程一起退出
```

> **类比理解：**
>
> Shell 进程是"公司"，你运行的命令是"员工"。公司倒闭（关闭终端），员工全部失业。
>
> `(cmd &)` 相当于把这个员工"挂靠"到了政府机构（`systemd`，系统守护进程），公司倒不倒跟他没关系。

```bash
# 退出终端后服务继续运行
$ (python manager.py runserver 0.0.0.0 &)
$ logout   # 退出终端，服务依然在运行
```

> 类似的还有 `nohup some_cmd &`，但实践中 `(cmd &)` 更稳定。

---

## 三、单引号 vs 双引号

### 核心区别

| 引号类型         | 对 `$`、`()`、`)`等特殊字符 | 说明         |
| ---------------- | --------------------------------- | ------------ |
| 单引号 `'...'` | **不转义** ，原样输出       | 绝对字面量   |
| 双引号 `"..."` | **会转义** ，执行变量替换   | 允许变量扩展 |

```bash
$ echo '$(date)'     # 输出：$(date)（原样）
$ echo "$(date)"     # 输出：Tue Mar 17 12:00:00 2026（执行了命令）
```

### 重要细节：双引号防止空格拆分

```bash
# 假设 $var = "hello world"（含空格）

$ echo $var          # 可能被当成两个参数
$ echo "$var"        # ✅ 正确，整体作为一个参数
```

> **规则：** 如果变量或命令输出的结果 **可能包含空格** ，应该用双引号括起来。

### 开启命令回显调试

```bash
$ set -x    # 开启 shell 命令回显，可以看到 shell 实际执行的命令
$ set +x    # 关闭回显
```

---

## 四、sudo 找不到命令

### 问题现象

```bash
$ connect.sh            # ✅ 普通用户可以执行
$ sudo connect.sh       # ❌ sudo: command not found
```

### 原因

```
普通用户的 PATH：包含 /home/fdl/bin → 能找到 connect.sh
sudo 的 PATH：  来自 /etc/sudoers  → 不包含 /home/fdl/bin → 找不到
```

> **类比理解：**
>
> 就像你知道一家小餐馆（`connect.sh`）在哪，但你换了一个新司机（`sudo`），他只知道大型连锁餐厅（`/etc/sudoers` 中的标准路径），不知道那家小餐馆的位置。

### 解决方案

```bash
# 使用完整绝对路径，而不是命令名
$ sudo /home/fdl/bin/connect.sh
```

---

## 五、花括号扩展：避免输入重复文件名

### 基本语法

```bash
# 花括号中的每个字符串与前后字符串拼接
$ echo {one,two,three}file
onefile twofile threefile

$ echo {one,two,three}{1,2,3}
one1 one2 one3 two1 two2 two3 three1 three2 three3
```

> ⚠️ 花括号和逗号 **不能有空格** ，否则被当成普通字符串。

### 实用场景

```bash
# 给文件创建备份副本（注意第一个花括号内有空内容）
$ cp /very/long/path/file{,.bak}
# 等效于：cp /very/long/path/file /very/long/path/file.bak

# 删除多个文件
$ rm file{1,3,5}.txt
# 等效于：rm file1.txt file3.txt file5.txt

# 移动多种后缀的文件
$ mv *.{c,cpp} src/
# 等效于：mv *.c *.cpp src/
```

---

## 六、路径输入技巧

### 技巧 1：`cd -` 返回上一个目录

```bash
$ pwd
/very/long/path

$ cd              # 回家目录
$ pwd
/home/labuladong

$ cd -            # 返回上一个目录
$ pwd
/very/long/path
```

### 技巧 2：`!$` 替换上一条命令的最后一个路径

```bash
$ /usr/bin/script.sh
zsh: permission denied: /usr/bin/script.sh

$ chmod +x !$
# 等效于：chmod +x /usr/bin/script.sh
```

### 技巧 3：`!*` 替换上一条命令的所有参数

```bash
$ file script1.sh script2.sh script3.sh

$ chmod +x !*
# 等效于：chmod +x script1.sh script2.sh script3.sh
```

### 技巧 4：`CDPATH` 设置快速跳转目录

```bash
# 将常用目录加入 CDPATH
$ export CDPATH='~:/var/log'

# 现在 cd 找不到目标时，会自动在 CDPATH 中搜索
$ cd mysql
cd /var/log/mysql    # 自动跳转到 /var/log/mysql
```

> 相当于给 `cd` 命令设置了"收藏夹"，少写大量路径。

---

## 七、历史命令技巧

### 技巧 1：`!!` 重复上一条命令

```bash
$ apt install net-tools
E: Permission denied

$ sudo !!
sudo apt install net-tools   # 自动补全上一条命令
```

### 技巧 2：`Ctrl+R` 反向搜索历史（bash）

按下 `Ctrl+R`，输入关键词，找到最近匹配的命令后回车执行：

```
(reverse-i-search)`sudo': sudo apt install git
```

> 限制：只有 bash 支持，只能找到最近一条匹配的命令。

### 技巧 3：`history | grep` 搜索历史命令

```bash
$ history | grep 'config'
 7434  git config --global --unset https.proxy
10433  cd ~/.config
```

找到编号后，用 `!编号` 直接执行：

```bash
$ !7434
git config --global --unset https.proxy
```

### 技巧 4：封装 `his` 函数（推荐）

在 `.bashrc` 或 `.zshrc` 中添加：

```bash
his() {
    history | grep "$@"
}
```

使用时更简洁：

```bash
$ his 'config'    # 等效于 history | grep 'config'
```

---

## 八、其他实用小技巧

### 技巧 1：`yes` 自动确认交互式提问

```bash
# 安装软件时自动回答所有 y/n 问题
$ yes | sudo apt install XXX
```

**原理：** `yes` 命令会不断输出字符 `y`，通过管道传给 `apt` 的标准输入，每次程序等待输入时就读到 `y`，等效于手动输入。

---

### 技巧 2：`$?` 获取上一条命令的返回值

```bash
$ some_command
$ echo $?     # 0 = 正常退出，非 0 = 异常退出
```

**实际应用：** 在脚本中判断命令是否成功

```bash
#!/bin/bash
# 查看文件尾部是否已包含页脚
tail | grep 'page_footer' $filename
# grep 找到返回 0，找不到返回非 0
[ $? -ne 0 ] && { <add_page_footer_command> }
```

---

### 技巧 3：`$$` 获取当前进程 PID（用于命名临时文件）

```bash
# 用 PID 命名临时文件，保证唯一不重复
$ tmpfile="/tmp/my_script_$$"
$ echo "some data" > $tmpfile
$ cat $tmpfile
$ rm $tmpfile
```

> PID 在系统中是唯一的，用它命名临时文件既简单又不会重名。

---

## 九、总结速查表

```
┌─────────────────────────────────────────────────────────────┐
│                    Shell 技巧速查                           │
├────────────────────┬────────────────────────────────────────┤
│ 技巧               │ 用法                                   │
├────────────────────┼────────────────────────────────────────┤
│ 命令结果作为参数    │ rm $(where file)                       │
│ 后台脱离终端运行    │ (cmd &)                                │
│ 花括号扩展         │ cp file{,.bak} / rm f{1,3,5}.txt       │
│ 返回上一目录        │ cd -                                   │
│ 上条命令最后参数    │ !$                                     │
│ 上条命令所有参数    │ !*                                     │
│ 快速目录跳转        │ export CDPATH='~:/var/log'             │
│ 重复上一条命令      │ !!                                     │
│ 搜索历史命令        │ history | grep 'kw' 或 !编号           │
│ 自动输入 y 确认     │ yes | cmd                              │
│ 上条命令返回值      │ $?                                     │
│ 当前进程 PID        │ $$                                     │
└────────────────────┴────────────────────────────────────────┘
```
