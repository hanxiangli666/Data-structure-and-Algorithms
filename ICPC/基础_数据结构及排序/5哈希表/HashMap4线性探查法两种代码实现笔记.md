# 线性探查法的两种代码实现 — 学习笔记

---

## 一、前置回顾

上一篇我们学了线性探查法的两个难点：

1. **环形数组**：探查到数组末尾后要绕回开头
2. **删除操作**：不能直接置空，否则会打断探查链

删除的两种解决方案：

| 方案 | 核心思想 |
|------|---------|
| **数据搬移（rehash）** | 删除后，把后面的元素重新插入，填补空洞 |
| **占位符标记（DELETED）** | 删除时不真删，放一个特殊标记 |

本篇给出这两种方案的**具体代码实现**。

---

## 二、简化版设定（聚焦核心逻辑）

| 简化项 | 说明 |
|--------|------|
| key 和 value 都是 `int` | 不用泛型 |
| 哈希函数 = `key % table.size()` | 方便制造冲突 |
| 数组大小固定为 10 | 不考虑扩缩容 |
| key 不存在时返回 -1 | 简化返回值 |

---

## 三、方案一：数据搬移版（rehash）

### 3.1 核心思想

删除一个元素后，把它**后面连续的元素**全部取出来，重新用 `put` 方法插入。这样这些元素就会自动找到正确的位置，空洞也被填补了。

> **类比**：排队时有人离开了，后面的人不是简单往前挪一格，而是每个人重新按规则排队——因为有些人本来就该站在更前面的位置，趁这个机会可以"归位"。

### 3.2 关键方法：findKeyIndex

两个方案的核心区别就在于"怎么在 table 中找到 key 的位置"。rehash 版的查找逻辑很简单：

```cpp
// 从 hash(key) 开始，往后逐个找
// 找到 key → 返回它的索引
// 遇到空位（nullptr）→ 说明 key 不存在，返回这个空位的索引
int findKeyIndex(int key) {
    int index = hash(key);

    while (table[index] != NULL) {
        if (table[index]->key == key) {
            return index;  // 找到了 key
        }
        index = (index + 1) % table.size();  // 环形往后走
    }

    return index;  // 返回空位索引（可用于插入）
}
```

**这个方法有两个用途**（一箭双雕）：

| 调用者 | 返回值含义 |
|--------|-----------|
| `get(key)` | 如果 `table[index]` 不为空 → 找到了；为空 → key 不存在 |
| `put(key, val)` | 如果 `table[index]` 不为空 → key 已存在，更新 value；为空 → 这是插入位置 |

> **为什么一个方法能同时服务查找和插入？** 因为线性探查法的规则是"往后找到第一个空位或目标 key"。找到 key 就更新，找到空位就插入，逻辑天然统一。

### 3.3 增/改操作（put）

```cpp
void put(int key, int value) {
    int index = findKeyIndex(key);
    // 不管是找到了 key（更新）还是找到了空位（插入）
    // 都直接在这个位置放新节点就行
    table[index] = new Node(key, value);
}
```

简洁到令人发指。因为 `findKeyIndex` 已经帮你找到了正确的位置。

### 3.4 查找操作（get）

```cpp
int get(int key) {
    int index = findKeyIndex(key);
    // table[index] 为空说明没找到
    return table[index] == NULL ? -1 : table[index]->val;
}
```

### 3.5 删除操作（remove）— 本方案的重点

```cpp
void remove(int key) {
    int index = findKeyIndex(key);
    if (table[index] == NULL) {
        return;  // key 不存在，不需要删
    }

    // 第一步：删除目标元素
    table[index] = NULL;

    // 第二步：把后面连续的元素重新插入（rehash）
    index = (index + 1) % table.size();  // 从下一个位置开始
    while (table[index] != NULL) {
        Node* entry = table[index];
        table[index] = NULL;        // 先取出来（置空）
        put(entry->key, entry->val); // 重新插入
        index = (index + 1) % table.size();  // 继续看下一个
    }
    // 遇到空位就停止，因为空位后面的元素不受影响
}
```

### 3.6 删除过程图解

假设初始状态：

```
put(1, a), put(2, b), put(10, x), put(20, y), put(30, z), put(3, c)

table = [ a,  x,  y,  z,  c,  _,  _,  _,  _,  b ]
index     0   1   2   3   4   5   6   7   8   9
key       1  10  20  30   3                    2
hash      1   0   0   0   3                    2
```

> 注意：key=1 的 hash 是 1，key=10 的 hash 是 0 但 0 号位被 key=1 占了（不对，这里 key=1 的 hash(1)=1，key=10 的 hash(10)=0，所以 key=10 放在 index=1 也不对...）

让我用原文的例子来说明。执行 `remove(20)` 的过程：

**第一步**：找到 key=20，删除它（置空），产生空洞

**第二步**：从空洞的下一个位置开始，把连续的元素取出来重新 put

- 取出后面的元素，置空，调用 `put` 重新插入
- `put` 内部会调用 `findKeyIndex`，自动找到正确的位置
- 直到遇到一个空位，停止

**结果**：空洞被填补，所有元素都在正确的位置，探查链完整。

### 3.7 为什么 rehash 能正确工作？

关键在于：`put` 方法会根据 `hash(key)` 重新计算位置。

- 如果一个元素本来就该在更前面（只是因为冲突被挤到后面），重新 put 后它就会"归位"到更前面
- 如果一个元素的 hash 值本来就是当前位置附近，重新 put 后它还是会落在附近

> **类比**：就像洗牌后重新发牌。每个人按照自己的号码重新找座位，最终每个人都坐在了正确的位置上。

---

## 四、方案二：占位符标记版（DELETED）

### 4.1 核心思想

删除元素时，不真的清空，而是放一个特殊的"墓碑"标记。这个标记告诉 `get` 方法："这里曾经有人，别停下来，继续往后找。"

### 4.2 占位符的定义

```cpp
// 创建一个特殊的 KVNode 作为占位符
// 里面的值不重要，因为我们只比较指针地址
KVNode* DELETED = new KVNode{-2, -2};
```

**关键理解**：我们判断"是不是占位符"时，比较的是**指针地址**（`table[i] == DELETED`），不是比较里面的值。所以占位符里存什么值都无所谓。

> **类比**：DELETED 就像一个特殊的"牌子"。你不看牌子上写了什么字，你只看"这是不是那个牌子"。

### 4.3 关键方法：findKeyIndex（与 rehash 版的最大区别）

```cpp
int findKeyIndex(int key) {
    for (int i = hash(key), step = 0;
         table[i] != nullptr;
         i = (i + 1) % table.size()) {

        // 防死循环：如果走了超过 table.size() 步，说明绕了一圈
        if (++step > table.size()) return -1;

        // 遇到占位符 → 跳过，继续往后找
        if (table[i] == DELETED) continue;

        // 找到目标 key
        if (table[i]->key == key) return i;
    }

    // 遇到 nullptr（真正的空位），说明 key 不存在
    return -1;
}
```

**与 rehash 版的区别**：

| | rehash 版 findKeyIndex | 占位符版 findKeyIndex |
|---|----------------------|---------------------|
| 遇到空位 | 返回空位索引（可插入） | 返回 -1（表示没找到） |
| 遇到占位符 | 不存在占位符 | 跳过，继续找 |
| 防死循环 | 不需要（没有占位符，一定有空位） | 需要（可能全是占位符，绕一圈回来） |
| 用于插入 | 可以（返回的空位就是插入位置） | 不行（返回 -1 无法直接插入） |

### 4.4 为什么占位符版需要防死循环？

想象 table 里全是占位符（DELETED）：

```
table = [DEL, DEL, DEL, DEL, DEL, DEL, DEL, DEL, DEL, DEL]
```

此时调用 `get(5)`：
- `hash(5) = 5` → index 5 不是 nullptr → 是 DELETED，跳过
- index 6 → DELETED，跳过
- index 7 → DELETED，跳过
- ... 一直跳 ...
- 绕回 index 5 → 还是 DELETED ...
- **死循环！**

所以需要用 `step` 计数器来打破：走了超过 `table.size()` 步就放弃。

### 4.5 增/改操作（put）

```cpp
void put(int key, int val) {
    int index = findKeyIndex(key);

    if (index != -1 && table[index] != nullptr) {
        // key 已存在，更新 value
        table[index]->val = val;
        return;
    }

    // key 不存在，需要找一个位置插入
    // 注意：不能用 findKeyIndex 的返回值（它返回 -1）
    // 要自己从 hash(key) 开始找空位或占位符
    KVNode* node = new KVNode{key, val};
    index = hash(key);
    while (table[index] != nullptr && table[index] != DELETED) {
        index = (index + 1) % table.size();
    }
    table[index] = node;
}
```

**注意 put 的插入逻辑**：

占位符（DELETED）的位置**可以被新元素覆盖**。这是占位符的一个好处——被删的位置可以回收利用。

所以插入时的停止条件是：遇到 `nullptr`（真空位）**或** `DELETED`（可复用的位置）。

### 4.6 删除操作（remove）— 非常简单

```cpp
void remove(int key) {
    int index = findKeyIndex(key);
    if (index == -1) return;  // key 不存在

    // 直接标记为已删除
    table[index] = DELETED;
}
```

就这么简单！不需要搬移任何数据。

### 4.7 查找操作（get）

```cpp
int get(int key) {
    int index = findKeyIndex(key);
    return (index != -1) ? table[index]->val : -1;
}
```

---

## 五、两种方案的删除过程对比

假设初始状态都是：

```
table = [a,  b,  c,  A,  B,  d,  e,  _,  _,  _]
index    0   1   2   3   4   5   6   7   8   9
key      0  10  20   3  13  30  40
```

现在执行 `remove(10)`（删除 index 1）：

### rehash 版

```
第1步：table[1] = NULL → 产生空洞
  [a,  _,  c,  A,  B,  d,  e,  _,  _,  _]

第2步：取出 table[2]（key=20），重新 put → 放到 index 1
  [a, c',  _,  A,  B,  d,  e,  _,  _,  _]

第3步：取出 table[3]（key=3），重新 put → hash(3)=3，放回 index 3
  [a, c',  _,  A,  B,  d,  e,  _,  _,  _]
  （key=3 的 hash 是 3，前面有空位但它不该去那里，put 会把它放到 index 3）

  ... 实际上取出后重新 put，每个元素都会找到当前最优的位置 ...

最终结果：空洞被填补，所有元素位置正确
```

### 占位符版

```
table[1] = DELETED，结束

  [a,  ×,  c,  A,  B,  d,  e,  _,  _,  _]
  
完事。简单粗暴。
```

---

## 六、完整版新增功能

完整版在简化版基础上增加了：

### 6.1 泛型支持

```cpp
template<typename K, typename V>
class MyLinearProbingHashMap { ... };

// 使用时指定具体类型
MyLinearProbingHashMap<string, int> map;
MyLinearProbingHashMap<int, int> map2;
```

### 6.2 动态扩缩容

| 触发条件 | 动作 |
|---------|------|
| `size >= capacity * 0.75` | 扩容：容量翻倍 |
| `size <= capacity / 8` | 缩容：容量缩为 1/4（rehash 版）或 1/2（占位符版） |

**占位符版的 resize 有个额外好处**：resize 过程中会用 `put` 重新插入所有"活着的"元素，自然就**清理掉了所有占位符**。

```cpp
void resize(int cap) {
    MyLinearProbingHashMap2<K, V> newMap(cap);
    for (auto entry : table) {
        // 只搬移活着的元素，跳过 nullptr 和 DELETED
        if (entry != nullptr && entry != DUMMY) {
            newMap.put(entry->key, entry->val);
        }
    }
    this->table = newMap.table;
}
```

### 6.3 占位符版防死循环的处理

```cpp
int getKeyIndex(K key) {
    int step = 0;
    for (int i = hash(key); table[i] != nullptr; i = (i + 1) % table.size()) {
        step++;
        if (step > table.size()) {
            // 绕了一圈！触发 resize 清理占位符
            resize(table.size());
            return -1;
        }
        if (entry == DUMMY) continue;
        if (entry->key == key) return i;
    }
    return -1;
}
```

当检测到死循环时，调用 `resize(table.size())`——容量不变，但所有占位符都被清理了。

---

## 七、两种方案全面对比

| 对比维度 | rehash 版 | 占位符版 |
|---------|-----------|---------|
| **删除操作** | 复杂：要取出后续元素重新插入 | 简单：标记为 DELETED |
| **删除时间** | O(K)，K 是后续连续元素个数 | O(1) |
| **查找效率** | 好（没有无用元素干扰） | 可能变差（占位符拉长探查链） |
| **空间利用** | 好（删除即释放） | 可能浪费（占位符占位置） |
| **死循环风险** | 无 | 有（需要 step 计数器防御） |
| **findKeyIndex** | 返回空位索引（可直接用于插入） | 返回 -1（put 需要另外找位置） |
| **代码复杂度** | remove 复杂，其他简单 | findKeyIndex 复杂，remove 简单 |

---

## 八、竞赛实用总结

### 你需要自己实现线性探查法吗？

**99% 的情况下不需要。** 直接用标准库：

```cpp
#include <unordered_map>
unordered_map<int, int> mp;  // 底层用拉链法实现，够用了
```

### 什么时候可能用到线性探查的思想？

- 某些特殊题目需要你手写哈希表（极少见）
- 理解线性探查有助于理解"探查"这类思想，在其他算法中可能出现

### 哈希表系列知识总结

```
哈希表知识体系
│
├── 核心原理
│   ├── key → hash() → 数组索引 → 存取 value
│   ├── 哈希函数的要求（O(1)、确定性）
│   └── key 必须不可变
│
├── 解决冲突
│   ├── 拉链法（主流，推荐掌握）
│   │   ├── 底层：数组 + 链表
│   │   ├── 增删查改都在链表上操作
│   │   └── 负载因子可以 > 1
│   │
│   └── 线性探查法（了解即可）
│       ├── 底层：纯数组 + 环形遍历
│       ├── 难点1：环形数组（取模）
│       ├── 难点2：删除维护连续性
│       │   ├── rehash 版：删后重新插入后续元素
│       │   └── 占位符版：DELETED 标记 + 防死循环
│       └── 负载因子必须 < 1
│
├── 扩缩容
│   ├── 负载因子 = size / capacity
│   ├── 扩容阈值：0.75（经验值）
│   └── 扩容过程：新建大数组 → 全部重新 hash 插入
│
└── C++ 实战
    ├── unordered_map / unordered_set → O(1)，无序
    └── map / set → O(log N)，有序
```

到这里，哈希表的原理和实现就全部学完了。接下来可以通过 LeetCode 上的哈希表题目来巩固理解。
