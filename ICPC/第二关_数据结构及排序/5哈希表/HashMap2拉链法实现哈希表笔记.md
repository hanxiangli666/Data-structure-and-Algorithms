# 拉链法实现哈希表 — 学习笔记

---

## 一、前置回顾

上一篇笔记我们学了哈希表的核心原理，知道了：

- 哈希表 = 数组 + 哈希函数
- 哈希冲突不可避免
- 解决冲突有两种方法：**拉链法**和**线性探查法（开放寻址法）**

本篇就来深入学习**拉链法**是怎么具体实现的。

---

## 二、拉链法的核心思想

### 2.1 一句话概括

> 底层数组的每个位置不直接存 value，而是挂一个**链表**。多个 key 哈希到同一个位置时，它们的 key-value 对都串在这个链表上。

### 2.2 类比理解

想象一个学校的储物柜墙：

- 每个柜子编号就是数组索引（0, 1, 2, ...）
- 正常情况下，一个柜子放一个学生的东西
- 如果两个学生被分配到同一个柜子怎么办？**在柜子门上挂一串收纳袋**，每个袋子装一个学生的东西，袋子上写着学生的名字（key）
- 找东西时：先找到柜子（哈希函数算索引），再在收纳袋里按名字找（遍历链表）

### 2.3 底层数据结构

```
table 数组：
index 0: → [k1:v1] → [k3:v3] → null    （k1 和 k3 哈希冲突，都映射到 0）
index 1: → null                          （空的，没有元素）
index 2: → [k2:v2] → null               （只有一个元素）
index 3: → [k4:v4] → [k5:v5] → null     （k4 和 k5 哈希冲突）
...
```

每个数组元素是一个链表的头，链表里的每个节点都存着一个 key-value 对。

---

## 三、简化版实现（先理解核心逻辑）

为了聚焦核心逻辑，先做几个简化：

| 简化项 | 说明 |
|--------|------|
| key 和 value 都是 `int` | 不用泛型，降低理解难度 |
| 哈希函数 = 简单取模 | `hash(key) = key % table.size()` |
| 数组大小固定 | 不考虑扩缩容 |
| key 不存在时返回 -1 | 简化返回值处理 |

### 3.1 链表节点的设计

```cpp
struct KVNode {
    int key;
    int value;
    KVNode(int key, int value) : key(key), value(value) {}
};
```

**为什么要同时存 key 和 value？**

因为一个链表里可能挂了好几个不同的 key-value 对（它们哈希冲突了）。你查找的时候，光靠 value 分不清谁是谁，必须用 key 来辨认。

> **类比**：收纳袋上必须写名字（key），不然你打开柜子看到一堆袋子，怎么知道哪个是你的？

### 3.2 整体结构

```cpp
class ExampleChainingHashMap {
    // 底层数组，每个元素是一个链表
    // 链表里的每个节点是一个 KVNode
    vector<list<KVNode>> table;

public:
    // 构造函数：创建指定容量的数组
    ExampleChainingHashMap(int capacity) : table(capacity) {}

    // 哈希函数：简单取模
    int hash(int key) {
        return key % table.size();
    }
};
```

**C++ 知识点补充**：

- `vector<list<KVNode>>`：这是一个"二维"结构。外层是动态数组 `vector`，里面每个元素是一个 `list`（双向链表），链表里装的是 `KVNode`
- `list` 是 C++ 标准库提供的双向链表容器，头尾插入/删除都是 O(1)

### 3.3 查找操作（get）

**思路**：

1. 用哈希函数算出 key 对应的数组索引
2. 去那个位置的链表里，挨个对比 key，找到就返回 value
3. 找不到就返回 -1

```cpp
int get(int key) {
    int index = hash(key);

    // 链表为空，key 肯定不存在
    if (table[index].empty()) {
        return -1;
    }

    // 遍历链表，逐个对比 key
    for (const auto& node : table[index]) {
        if (node.key == key) {
            return node.value;  // 找到了！
        }
    }

    // 遍历完了也没找到
    return -1;
}
```

**C++ 语法说明**：

- `for (const auto& node : table[index])`：范围 for 循环，遍历链表中的每个节点
  - `const`：我们只读不改，加 const 更安全
  - `auto&`：自动推导类型，`&` 表示引用（不拷贝，直接访问原始数据）
  - 没有 `&` 的话，每次循环都会拷贝一份节点，浪费性能

### 3.4 插入/修改操作（put）

**思路**：

1. 算出索引
2. 遍历链表，看 key 是否已经存在
   - 存在 → 更新 value（哈希表的 key 是唯一的，不会有重复）
   - 不存在 → 在链表尾部添加新节点

```cpp
void put(int key, int value) {
    int index = hash(key);

    // 链表为空，直接插入
    if (table[index].empty()) {
        table[index].push_back(KVNode(key, value));
        return;
    }

    // 链表不为空，先看 key 是否已存在
    for (auto& node : table[index]) {
        if (node.key == key) {
            node.value = value;  // key 已存在，更新 value
            return;
        }
    }

    // key 不存在，添加到链表尾部
    table[index].push_back(KVNode(key, value));
}
```

**注意**：这里的 `for` 循环用的是 `auto&`（不带 const），因为我们可能要**修改** node 的 value。

### 3.5 删除操作（remove）

**思路**：

1. 算出索引
2. 在链表中找到 key 对应的节点，删掉它

```cpp
void remove(int key) {
    auto& lst = table[hash(key)];  // 注意变量名避开了 std::list
    if (lst.empty()) {
        return;  // 链表为空，没什么好删的
    }

    // 删除 key 匹配的节点
    lst.remove_if([key](KVNode& node) {
        return node.key == key;
    });
}
```

**C++ 语法说明**：

- `remove_if`：C++ `list` 的成员函数，删除所有满足条件的元素
- `[key](KVNode& node) { return node.key == key; }`：这是一个 **lambda 表达式**（匿名函数）
  - `[key]`：捕获外面的 `key` 变量，让 lambda 内部能用
  - `(KVNode& node)`：参数，链表中的每个节点
  - `return node.key == key`：如果节点的 key 匹配，返回 true，表示"删掉这个"

> **不熟悉 lambda？** 你可以把它理解成一个"临时创建的小函数"。`[key]` 括号里放的是这个小函数需要用到的外部变量。

---

## 四、完整版实现（带扩缩容）

简化版让我们理解了核心逻辑，完整版在此基础上增加了：

1. **泛型支持**：key 和 value 可以是任意类型
2. **动态扩缩容**：根据负载因子自动调整数组大小
3. **完善的哈希函数**：处理负数、取模映射
4. **keys() 方法**：返回所有的 key

### 4.1 完整代码（带详细注释）

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <vector>

template<typename K, typename V>
class MyChainingHashMap {
    // 链表节点，存储 key-value 对
    struct KVNode {
        K key;
        V value;
        KVNode(K key, V val) : key(key), value(move(val)) {}
    };

    // 底层数组，每个元素是一个链表
    vector<list<KVNode>> table;

    // 当前存储的键值对数量
    int size_;

    // 初始容量
    static constexpr int INIT_CAP = 4;

    // 哈希函数
    int hash(K key) {
        // hash<K>{}(key)：调用 C++ 标准库的哈希函数
        // & 0x7fffffff：去掉符号位，保证非负
        // % table.size()：映射到合法索引范围
        return (std::hash<K>{}(key) & 0x7fffffff) % table.size();
    }

    // 扩缩容
    void resize(int newCap) {
        newCap = max(newCap, 1);  // 防止容量为 0
        MyChainingHashMap<K, V> newMap(newCap);

        // 把旧表中的所有键值对搬到新表
        for (auto& lst : table) {
            for (auto& node : lst) {
                newMap.put(node.key, node.value);
            }
        }

        // 用新表的底层数组替换旧的
        this->table = newMap.table;
    }

public:
    // 默认构造函数
    MyChainingHashMap() : MyChainingHashMap(INIT_CAP) {}

    // 指定初始容量的构造函数
    explicit MyChainingHashMap(int initCapacity) {
        size_ = 0;
        initCapacity = max(initCapacity, 1);
        table.resize(initCapacity);
    }

    // ======== 增/改 ========
    void put(K key, V val) {
        auto& lst = table[hash(key)];

        // 检查 key 是否已存在
        for (auto& node : lst) {
            if (node.key == key) {
                node.value = val;  // 已存在，更新
                return;
            }
        }

        // 不存在，插入新节点
        lst.emplace_back(key, val);
        size_++;

        // 负载因子 >= 0.75，扩容为原来的 2 倍
        if (size_ >= table.size() * 0.75) {
            resize(table.size() * 2);
        }
    }

    // ======== 删 ========
    void remove(K key) {
        auto& lst = table[hash(key)];

        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it->key == key) {
                lst.erase(it);
                size_--;

                // 负载因子 <= 0.125，缩容为原来的 1/4
                if (size_ <= table.size() / 8) {
                    resize(table.size() / 4);
                }
                return;
            }
        }
    }

    // ======== 查 ========
    V get(K key) {
        const auto& lst = table[hash(key)];

        for (const auto& node : lst) {
            if (node.key == key) {
                return node.value;
            }
        }
        return nullptr;  // 未找到
    }

    // ======== 获取所有 key ========
    list<K> keys() {
        list<K> allKeys;
        for (const auto& lst : table) {
            for (const auto& node : lst) {
                allKeys.push_back(node.key);
            }
        }
        return allKeys;
    }

    int size() const {
        return size_;
    }
};
```

### 4.2 新增功能详解

#### 泛型（template）

```cpp
template<typename K, typename V>
class MyChainingHashMap { ... };
```

- `template<typename K, typename V>`：告诉编译器，K 和 V 是占位符，使用时再确定具体类型
- 这样同一套代码就能支持 `<string, int>`、`<int, string>` 等各种组合

> **类比**：模板就像一个"万能插座"，你不需要为每种插头做一个插座，只要一个万能的就够了。

#### 动态扩缩容（resize）

扩缩容的触发条件：

| 操作 | 条件 | 动作 |
|------|------|------|
| 插入后 | `size >= capacity * 0.75`（负载因子 ≥ 75%） | 容量翻倍 |
| 删除后 | `size <= capacity / 8`（负载因子 ≤ 12.5%） | 容量缩为 1/4 |

**扩缩容的过程**：

1. 创建一个新的、容量不同的哈希表
2. 把旧表中的所有键值对重新 `put` 到新表（因为容量变了，哈希函数的结果也变了，每个 key 的位置需要重新计算）
3. 用新表的底层数组替换旧表的

> **类比**：停车场扩建了。车位从 10 个变成 20 个，每辆车的分配规则（车牌号 % 车位数）结果不同了，所以所有车都得重新分配车位。

**为什么扩容阈值是 0.75？**

这是一个经验值，在"空间浪费"和"冲突频率"之间取了个平衡：
- 太小（比如 0.5）：空间浪费多，一半的位置都空着
- 太大（比如 0.9）：冲突太多，链表变长，查找变慢

#### 删除操作中的迭代器用法

```cpp
for (auto it = lst.begin(); it != lst.end(); ++it) {
    if (it->key == key) {
        lst.erase(it);  // 用迭代器精确删除
        size_--;
        return;
    }
}
```

**C++ 语法说明**：

- `auto it = lst.begin()`：`it` 是一个**迭代器**，你可以把它理解成一个"指向链表中某个节点的指针"
- `it->key`：通过迭代器访问节点的 key 字段（等价于 `(*it).key`）
- `lst.erase(it)`：删除迭代器指向的那个节点
- 为什么不用 `remove_if`？因为完整版删除后还要做缩容等操作，用迭代器更灵活

> **迭代器类比**：迭代器就像你的手指，指着链表中的某个节点。`begin()` 是指向第一个节点，`end()` 是指向链表结尾的"虚拟位置"（表示遍历结束），`++it` 是手指往后移一个节点。

---

## 五、增删查改操作流程总结

### 5.1 get(key) 查找流程

```
key → hash(key) 算出索引 index
       ↓
table[index] 取出链表
       ↓
遍历链表，逐个对比 node.key == key？
       ↓
找到 → 返回 node.value
没找到 → 返回 -1 / nullptr
```

### 5.2 put(key, value) 插入/修改流程

```
key → hash(key) 算出索引 index
       ↓
table[index] 取出链表
       ↓
遍历链表，看 key 是否已存在？
       ↓
已存在 → 更新 value，结束
不存在 → 在链表尾部添加新节点，size++
       ↓
检查负载因子，是否需要扩容？
  size >= capacity * 0.75 → 扩容（容量翻倍）
```

### 5.3 remove(key) 删除流程

```
key → hash(key) 算出索引 index
       ↓
table[index] 取出链表
       ↓
遍历链表，找到 key 匹配的节点
       ↓
找到 → 删除该节点，size--
       ↓
检查负载因子，是否需要缩容？
  size <= capacity / 8 → 缩容（容量变为 1/4）
```

---

## 六、时间复杂度分析

| 操作 | 平均情况 | 最坏情况 |
|------|---------|---------|
| get | O(1) | O(N) |
| put | O(1) | O(N) |
| remove | O(1) | O(N) |

**为什么平均是 O(1)？**

- 哈希函数把 key 均匀分散到各个位置
- 负载因子控制在 0.75 以下，每个链表平均长度很短（接近常数）
- 所以遍历链表的时间可以看作 O(1)

**什么时候退化成 O(N)？**

- 所有 key 都哈希到同一个位置（极端情况），链表变成一条长链
- 这时候查找就变成了遍历整个链表，复杂度 O(N)

> **类比**：正常情况下，每个柜子只挂了 1-2 个收纳袋，找东西很快。但如果所有人的东西都被分配到了同一个柜子，那个柜子上挂了 N 个袋子，你就得一个一个翻，这就是 O(N)。

---

## 七、竞赛中的实际应用

在竞赛编程中，你**不需要自己实现哈希表**，直接用 C++ 标准库就行：

```cpp
#include <unordered_map>
using namespace std;

unordered_map<int, int> mp;
mp[1] = 100;          // put(1, 100)
cout << mp[1];         // get(1) → 100
mp.erase(1);           // remove(1)
mp.count(1);           // 判断 key 1 是否存在（返回 0 或 1）
```

**但理解底层原理很重要**，因为：

1. 面试会问哈希表的实现原理
2. 理解了原理，你才知道什么时候该用 `unordered_map`，什么时候该用 `map`
3. 竞赛中如果 `unordered_map` 被卡（恶意构造哈希冲突导致 O(N)），你得知道为什么，以及如何切换到 `map`

---

## 八、关键 C++ 知识点回顾

本篇出现了不少 C++ 新语法，这里集中总结一下：

| 语法 | 含义 | 示例 |
|------|------|------|
| `vector<list<KVNode>>` | 数组套链表的二维结构 | 哈希表的底层存储 |
| `for (auto& x : container)` | 范围 for 循环，遍历容器 | 遍历链表中的每个节点 |
| `const auto&` | 只读引用，不拷贝不修改 | 查找时遍历 |
| `auto&` | 可写引用，不拷贝可修改 | 需要修改 value 时 |
| `it->key` | 迭代器访问成员 | 等价于 `(*it).key` |
| `lst.erase(it)` | 用迭代器删除元素 | 精确删除链表中的某个节点 |
| `[x](Type& y) { ... }` | lambda 表达式 | 传给 `remove_if` 的条件函数 |
| `template<typename K, V>` | 模板/泛型 | 让类支持任意类型 |
| `emplace_back(args...)` | 直接在容器末尾构造对象 | 比先构造再 push_back 更高效 |
| `static constexpr` | 编译期常量 | `INIT_CAP = 4` |
| `explicit` | 禁止隐式类型转换 | 防止 `MyChainingHashMap m = 4;` 这种写法 |

---

## 九、知识图谱

```
拉链法实现哈希表
├── 底层结构：vector<list<KVNode>>（数组 + 链表）
├── KVNode 节点
│   ├── 必须同时存 key 和 value
│   └── 通过 key 区分同一链表中的不同元素
├── 核心操作
│   ├── get：算索引 → 遍历链表找 key → 返回 value
│   ├── put：算索引 → 遍历链表查重 → 更新或插入 → 检查扩容
│   └── remove：算索引 → 遍历链表找 key → 删除 → 检查缩容
├── 扩缩容
│   ├── 扩容条件：size >= capacity * 0.75
│   ├── 缩容条件：size <= capacity / 8
│   └── 过程：新建更大/更小的表 → 重新哈希所有元素 → 替换旧表
├── 时间复杂度
│   ├── 平均 O(1)（负载因子控制得当，链表短）
│   └── 最坏 O(N)（所有 key 冲突到同一位置）
└── 竞赛实践
    └── 直接用 unordered_map，但要理解原理
```
