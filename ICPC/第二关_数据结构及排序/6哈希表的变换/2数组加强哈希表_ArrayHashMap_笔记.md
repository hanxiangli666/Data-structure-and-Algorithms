# 数组加强哈希表（ArrayHashMap）学习笔记

> **前置知识**：哈希表基本原理、上一篇「哈希链表 LinkedHashMap」  
> **核心问题**：如何在 O(1) 时间内从哈希表中**均匀随机**返回一个 key？  
> **答案**：用一个**紧凑数组**维护所有 key，配合哈希表记录每个 key 在数组中的索引。

---

## 一、问题引入：randomKey() 为什么难？

### 1.1 需求描述

在普通哈希表（增删查改都是 O(1)）的基础上，新增一个 API：

```
K randomKey()   // O(1) 时间，均匀随机返回一个 key
```

**均匀随机**：如果哈希表里有 n 个 key，每个 key 被选中的概率都必须是 1/n。

### 1.2 为什么不能直接在哈希表底层数组上随机？

哈希表底层是一个 `table` 数组，但这个数组**有空洞**（很多位置是空的）。

**类比**：想象一栋公寓楼有 100 间房，但只住了 5 个人，其余 95 间都是空的。你随机挑一个房间号，大概率敲开门发现没人。

#### 尝试 1：随机到空位就往右找

```cpp
// 伪码
int i = rand() % arr.size();
while (arr[i] == null) {
    i = (i + 1) % arr.size();  // 往右找
}
return arr[i];
```

**两个问题**：
- 最坏情况要遍历整个数组，复杂度变成 O(N)
- **不是均匀随机**！空洞右边的元素被选中概率更高

举个例子：`arr = [1, 2, 空, 4]`
- 随机到索引 0 → 返回 1（概率 1/4）
- 随机到索引 1 → 返回 2（概率 1/4）
- 随机到索引 2 → 是空的，往右走到索引 3 → 返回 4
- 随机到索引 3 → 返回 4（概率 1/4）

所以元素 4 被选中的概率是 2/4 = 1/2，而 1 和 2 只有 1/4，**不公平**。

#### 尝试 2：随机到空位就重新随机

```cpp
int i = rand() % arr.size();
while (arr[i] == null) {
    i = rand() % arr.size();  // 重新随机
}
return arr[i];
```

这次是均匀随机了，但复杂度**取决于运气**——如果数组很大、元素很少（空洞很多），可能要随机很多次才能命中，不是 O(1)。

#### 尝试 3：先收集所有 key 到数组再随机

```cpp
vector<K> allKeys = map.keys();  // O(N) 遍历
return allKeys[rand() % allKeys.size()];
```

均匀随机，但每次调用都要 O(N) 遍历，还是不行。

### 1.3 拉链法更麻烦

如果哈希表用拉链法（每个槽位挂一个链表），情况更复杂：

```
table[0]: k1 → k2 → k3    （3 个元素）
table[1]: k4 → k5          （2 个元素）
```

就算你均匀随机选了一个槽位（1/2 概率），再均匀随机选了该槽位链表中的一个节点：
- k1, k2, k3 被选中概率 = 1/2 × 1/3 = 1/6
- k4, k5 被选中概率 = 1/2 × 1/2 = 1/4

**1/6 ≠ 1/4，还是不均匀**。

### 1.4 结论

> 直接在哈希表内部做随机，无论怎么搞都很难同时满足 **O(1)** 和 **均匀随机**。  
> 我们需要换个思路。

---

## 二、核心思路：额外维护一个紧凑数组

### 2.1 关键洞察

紧凑数组（没有空洞的数组）天然支持 O(1) 均匀随机：

```cpp
return arr[rand() % arr.size()];  // 完美！
```

所以思路很简单：**在哈希表旁边额外维护一个紧凑数组，专门存所有的 key**。

### 2.2 但删除怎么办？

插入很简单——新 key 直接 `push_back` 到数组末尾，数组始终紧凑。

但删除就麻烦了。比如 `arr = [k1, k2, k3, k4, k5]`，要删除 `k2`：

**直接删除**会留下空洞：`arr = [k1, 空, k3, k4, k5]` ❌

**把后面元素前移**保持紧凑：需要搬移数据，O(N) ❌

### 2.3 妙招：交换到末尾再删

**类比**：一排人站在队伍里，你想让中间某个人离队。与其让后面所有人都往前挪一步（很慢），不如让那个人和队尾的人**换个位置**，然后队尾的人直接走掉就行了。

步骤：
1. 把要删除的元素和**数组最后一个元素**交换位置
2. 删除数组最后一个元素（`pop_back`，O(1)）

```
删除 k2 的过程：
原数组:   [k1, k2, k3, k4, k5]
交换:     [k1, k5, k3, k4, k2]   ← k2 和 k5 换了位置
pop_back: [k1, k5, k3, k4]       ← 删掉末尾的 k2，完成！
```

代价是数组元素的**顺序会被打乱**，但对于 randomKey() 来说，顺序根本不重要。

### 2.4 还有一个问题：怎么快速找到元素在数组中的位置？

交换操作需要知道「k2 在数组的索引 1」。如果遍历数组去找，又是 O(N)。

解决办法：**用哈希表记录每个 key 在数组中的索引**！

```
map[k1] = 0    →  arr[0] = k1
map[k2] = 1    →  arr[1] = k2
map[k3] = 2    →  arr[2] = k3
...
```

这样通过 `map[key]` 就能 O(1) 找到 key 在数组中的位置。

---

## 三、完整数据结构设计

```
┌──────────────────────────────────────────────────────┐
│  ArrayHashMap                                        │
│                                                      │
│  哈希表 map:  key → 该 key 在 arr 中的索引           │
│  数组 arr:    紧凑存储所有的 (key, value) 对          │
│                                                      │
│  map["a"] = 0  ──→  arr[0] = {key:"a", val:1}       │
│  map["b"] = 1  ──→  arr[1] = {key:"b", val:2}       │
│  map["c"] = 2  ──→  arr[2] = {key:"c", val:3}       │
└──────────────────────────────────────────────────────┘
```

各操作的流程：

| 操作 | 做了什么 | 复杂度 |
|------|---------|--------|
| `put(key, val)` | 新 key：`arr.push_back`，`map[key] = arr.size()-1`；旧 key：只改 val | O(1) |
| `get(key)` | `map[key]` 拿到索引 → `arr[索引].val` | O(1) |
| `remove(key)` | 交换到末尾 → `pop_back` → 更新 map | O(1) |
| `randomKey()` | `arr[rand() % arr.size()].key` | O(1) |

---

## 四、C++ 代码实现 + 逐行解读

### 4.1 节点结构

**第一步：最简单的例子**

想象你要管理一个存放学生信息的系统。你需要两样东西：一个本子（数组）记录学生；一个索引卡（哈希表）快速查找学生在本子里的位置。

```cpp
struct Student {
    string name;      // 学生名字
    int score;        // 学生成绩  
    Student(string n, int s) : name(n), score(s) {}  // 构造函数
};
vector<Student> students;                // 本子：连续存储
unordered_map<string, int> nameToIndex;  // 索引卡：名字 → 位置
```

> 下面的代码做的是同样的事，只是把「学生」换成了「key-value 对」。

```cpp
template<typename K, typename V>
class MyArrayHashMap {
    struct Node {              // ⚠️ 这个结构体存一对数据
        K key;
        V val;
        Node(K key, V val) : key(key), val(val) {}  // 初始化列表
    };

    unordered_map<K, int> map;   // ⚠️ 注意：这里存的不是 value，而是索引！
    vector<Node> arr;            // 紧凑数组存所有 (key, value) 对
    default_random_engine e;     // 随机数引擎（用于均匀随机）
    // ...
};
```

> ✅ 关键点：map 存的是「位置」，不是「数据」。
```

**注意 map 里存的不是 value！** 是 key 在 arr 中的**索引**。真正的 key-value 对存在 arr 里。

**C++ 知识点**：
- `struct` 定义在 `class` 内部：这叫嵌套类型，`Node` 只在 `MyArrayHashMap` 内部使用
- `default_random_engine`：C++ 标准库的随机数引擎，比 `rand()` 更规范

### 4.2 构造函数

**第一步：最简单的例子**

种下随机数种子，就像给一个骰子「按下启动键」。每次按不同时刻的启动键，骰子的随机序列都会不同：

```cpp
random_device rd;        // 硬件随机数生成器
default_random_engine e; // 伪随机数引擎
e.seed(rd());            // 用硬件随机数作为种子
```

> 下面的代码将这几步合并，直接一行完成。

```cpp
MyArrayHashMap() {
    e.seed(random_device()());  // ⚠️ random_device()() 生成随机数作为种子
}                               // 第一个() 创建对象，第二个() 调用生成值
```

> ✅ 如果不设种子，每次程序「看起来随机」其实跑的是同样的序列。

### 4.3 get —— 查找

**第一步：最简单的例子**

查找就像查学生成绩：先看成绩单（哈希表）有没有这个学生的名字，有的话查出他在本子里的位置，再从本子里翻出他的成绩：

```cpp
int getStudentScore(string name) {
    if (!index.count(name)) return -1;     // 成绩单有没有这个名字
    int pos = index[name];                 // 找到位置
    return students[pos].score;            // 从本子翻出成绩
}
```

> 下面的代码把「学生名字」换成了「泛型 key」，「成绩」换成了「value」。

```cpp
V get(K key) {
    if (!map.count(key)) {      // ⚠️ count() 返回 0（不存在）或 1（存在）
        return NULL;            // key 不存在
    }
    int index = map[key];       // 从 map 拿到 key 在数组中的位置
    return arr[index].val;      // 用位置直接从数组取值
}
```

> ✅ 两次 O(1)：map 查询 O(1)，数组下标 O(1)，总共 O(1)。

### 4.4 put —— 插入/更新

**第一步：最简单的例子**

插入一个新学生，或更新已有学生的信息。如果学生已经在本子里，直接改成绩；如果是新学生，先在本子末尾加上他，再在索引卡里记下他的位置：

```cpp
void addOrUpdateStudent(string name, int score) {
    if (index.count(name)) {               // 学生已有记录
        int pos = index[name];             // 找他的位置
        students[pos].score = score;       // 改成绩
        return;
    }
    students.push_back(Student(name, score));    // 新学生
    index[name] = students.size() - 1;           // 记下位置
}
```

> 下面代码模式完全相同，只是改用 containsKey() 判断是否存在。

```cpp
void put(K key, V val) {
    if (containsKey(key)) {                    // 如果 key 已存在
        int i = map[key];                      // 拿到它在数组中的位置
        arr[i].val = val;                      // 直接改 value
        return;
    }
    arr.push_back(Node(key, val));             // key 不存在 → 追加新 Node
    map[key] = arr.size() - 1;                 // 记录新元素的索引
}
```

> 💡 **老师提醒：** push_back 后，新元素的索引就是 arr.size()-1（数组从 0 编号）。

### 4.5 remove —— 删除（最精华的部分）

**第一步：最简单的例子**

删除一个学生有个聪明办法：与其从本子中间撕掉他的页面（会留空白），不如把他和本子末尾的学生*互换位置*，然后把末尾撕掉。这样本子始终是紧凑的：

```cpp
void removeStudent(string name) {
    if (!index.count(name)) return;        // 没有这个学生
    int toDelete = index[name];            // 要删学生的位置
    Student lastOne = students.back();     // 末尾学生
    swap(students[toDelete], students.back());  // 两个互换
    index[lastOne.name] = toDelete;        // 更新末尾学生的新位置
    students.pop_back();                   // 撕掉末页
    index.erase(name);                     // 删掉索引卡
}
```

> 下面的代码思路完全相同，中间多了些临时变量来保存信息。

```cpp
void remove(K key) {
    if (!map.count(key)) {
        return;                              // key 不存在，直接返回
    }
    int index = map[key];                    // 要删元素在数组中的索引
    Node node = arr[index];                  // 保存这个要删的节点

    // 交换：末尾元素和要删元素互换位置
    Node e = arr.back();                     // 拿到末尾元素
    swap(arr[index], arr.back());            // ⚠️ swap 交换两个元素的位置

    // 关键：交换后，末尾元素已经移到 index，要更新它的索引
    map[e.key] = index;                      // 更新 e 所在的新位置

    arr.pop_back();                          // O(1) 删除末尾
    map.erase(node.key);                     // 从 map 也删掉
}
```

> 💡 **老师提醒：** 这是竞赛中的经典技巧——「交换到末尾，pop 掉」。任何时候需要 O(1) 删除且不在乎顺序，就用这招。

> ✅ 关键理解：交换之后，末尾元素的 map 索引变了，*一定要更新*！

**图解**（从 `[a, b, c, d, e]` 中删除 `b`）：

```
初始状态:
  arr = [{a,1}, {b,2}, {c,3}, {d,4}, {e,5}]
  map = {a:0, b:1, c:2, d:3, e:4}

第一步 - 交换 b 和末尾元素 e：
  arr = [{a,1}, {e,5}, {c,3}, {d,4}, {b,2}]
  
第二步 - 更新 map 中 e 的索引（从 4 改为 1）：
  map = {a:0, b:1, c:2, d:3, e:1}   ← e 的索引改了

第三步 - pop_back 删除末尾（也就是 b）：
  arr = [{a,1}, {e,5}, {c,3}, {d,4}]
  
第四步 - 从 map 中删除 b：
  map = {a:0, c:2, d:3, e:1}

最终:
  arr = [{a,1}, {e,5}, {c,3}, {d,4}]    ← 紧凑，无空洞！
  map = {a:0, e:1, c:2, d:3}            ← 索引全部正确！
```

> **这个删除技巧非常重要**，是竞赛和面试中的经典手法。
> 记住口诀：**「交换到末尾，pop 掉」**。

### 4.6 randomKey —— 均匀随机

**第一步：最简单的例子**

从本子里均匀随机抽一个学生。由于本子是紧凑的（没有空白页），随机选一个页码就行了：

```cpp
string getRandomStudent() {
    uniform_int_distribution<int> picker(0, students.size() - 1);
    int randomPos = picker(randomEngine);  // randomEngine 是全局引擎
    return students[randomPos].name;       // 用随机位置直接取学生
}
```

> 下面的代码完全相同，只是把「学生」换成了「key」。

```cpp
K randomKey() {
    int n = arr.size();
    uniform_int_distribution<int> u(0, n - 1);  // ⚠️ 定义 [0, n-1] 的均匀分布
    int randomIndex = u(e);                      // e 是成员的随机引擎，u(e) 生成数
    return arr[randomIndex].key;                 // 用随机索引直接取元素的 key
}
```

> ✅ 紧凑数组的好处就在这里：任何索引都有数据，所以概率均匀（每个是 1/n）。

---

## 五、对比：LinkedHashMap vs ArrayHashMap

| 特性 | LinkedHashMap（上一篇） | ArrayHashMap（本篇） |
|------|----------------------|---------------------|
| 额外结构 | 双链表 | 紧凑数组 |
| 维护的顺序 | 插入顺序 | 无特定顺序（会被打乱） |
| 特殊能力 | 按插入顺序遍历 | O(1) 均匀随机返回 key |
| 增删查改 | 全部 O(1) | 全部 O(1) |
| 典型应用 | LRU Cache | RandomizedSet |

**核心思想是一样的**：把哈希表当成黑盒，在外面套一层辅助结构（链表或数组），让它获得新能力。

---

## 六、思维要点总结

### 6.1 不要陷入哈希表底层细节

文中反复强调的一点：不要去想拉链法、开放寻址法这些底层实现。做题时，哈希表就是一个 O(1) 增删查改的黑盒。

**类比**：你开车不需要懂发动机怎么做功，你只要知道踩油门能加速、踩刹车能减速就行了。

### 6.2 「交换到末尾再删除」是通用技巧

只要你需要从一个**无序**集合中 O(1) 删除元素，都可以用这个技巧：

1. 找到要删除元素的位置（用哈希表辅助，O(1)）
2. 和最后一个元素交换
3. `pop_back()` 删除末尾

代价是顺序被打乱，但很多场景不关心顺序。

### 6.3 哈希表是万能胶

哈希表最大的用处之一就是**加速查找**。任何时候你发现「我需要遍历才能找到某个东西」，都可以考虑用哈希表来优化：

- 「数组元素 → 索引」的映射 → 用哈希表
- 「链表节点快速定位」→ 用哈希表
- 「判断某个值是否出现过」→ 用哈希表（或哈希集合）

---

## 七、关键概念速查表

| 概念 | 说明 |
|------|------|
| **均匀随机** | 每个元素被选中概率相等（n 个元素则每个 1/n） |
| **紧凑数组** | 没有空洞的数组，所有元素连续存储 |
| **交换删除法** | 把要删的元素换到末尾再 pop，O(1) 删除且保持紧凑 |
| `vector::back()` | 返回最后一个元素 |
| `vector::pop_back()` | 删除最后一个元素，O(1) |
| `swap(a, b)` | 交换两个变量的值 |
| `map.count(key)` | 判断 key 是否存在（返回 0 或 1） |
| `uniform_int_distribution` | C++ 均匀随机整数分布 |

---

## 八、对应练习题

1. **LeetCode 380 - Insert Delete GetRandom O(1)**  
   → 这道题就是让你实现 ArrayHashMap 的思路，必做题！

2. **LeetCode 381 - Insert Delete GetRandom O(1) - Duplicates allowed**  
   → 进阶版，允许重复元素，需要更巧妙的索引管理

3. **LeetCode 710 - Random Pick with Blacklist**  
   → 类似思路，在一个范围内随机选数，但要排除黑名单中的数

**建议学习顺序**：先做 380（和本文思路完全一致），做通了再尝试 381 和 710。

---

*笔记整理时间：2026-03-16*
