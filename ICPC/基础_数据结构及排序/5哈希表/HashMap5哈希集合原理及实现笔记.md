# 哈希集合的原理及代码实现 — 学习笔记

---

## 一、一句话搞懂哈希集合

> **哈希集合 = 只用了"键"的哈希表，把"值"扔掉不管。**

就这么简单。如果你已经理解了哈希表，那哈希集合就是白送的知识点。

---

## 二、哈希集合是干什么用的？

核心场景：**去重**。

它有三个特性：

| 特性 | 说明 |
|------|------|
| 不会有重复元素 | 加入已存在的元素时，什么都不会发生 |
| O(1) 增删 | 添加和删除元素都是常数时间 |
| O(1) 查找 | 判断一个元素是否存在也是常数时间 |

> **类比**：哈希集合就像一个"签到簿"。每个人只能签一次名（去重），你可以瞬间签到（O(1) 添加），瞬间划掉自己的名字（O(1) 删除），瞬间查看某个人有没有签过到（O(1) 查找）。

---

## 三、为什么说它就是哈希表的封装？

回忆哈希表的 API：

```cpp
// 哈希表
mp.put(key, value);    // 插入键值对，O(1)
mp.get(key);           // 通过 key 查 value，O(1)
mp.remove(key);        // 删除键值对，O(1)
```

哈希集合的 API：

```cpp
// 哈希集合
set.add(key);          // 添加元素，O(1)
set.contains(key);     // 判断元素是否存在，O(1)
set.remove(key);       // 删除元素，O(1)
```

对应关系一目了然：

| 哈希集合操作 | 实际上做的事 |
|-------------|------------|
| `add(key)` | `map.put(key, 随便一个占位值)` |
| `contains(key)` | `map.get(key) != null` |
| `remove(key)` | `map.remove(key)` |

value 放什么？**随便放，比如 `true`**。反正我们根本不关心 value 是什么，只关心 key 存不存在。

> **类比**：哈希表是一本"通讯录"，每个人名（key）对应一个电话号码（value）。哈希集合是一份"签到表"，你只关心名字（key）有没有出现，不需要写电话号码，但格式要求你必须填点什么，那就随便写个"到"（占位符）。

---

## 四、代码实现

### 4.1 用 C++ 标准库的 unordered_map 封装

```cpp
#include <unordered_map>

template<typename K>
class MyHashSet {
private:
    // 底层就是一个哈希表
    // value 用 bool 类型当占位符，存什么无所谓
    unordered_map<K, bool> map;

public:
    // 添加元素
    void add(const K& key) {
        map[key] = true;  // value 填 true 作占位
    }

    // 删除元素
    void remove(const K& key) {
        map.erase(key);
    }

    // 判断元素是否存在
    bool contains(const K& key) {
        return map.find(key) != map.end();
    }

    // 返回集合大小
    int size() {
        return map.size();
    }
};
```

**C++ 语法说明**：

- `const K& key`：常量引用传参。`const` 表示不会修改 key，`&` 表示不拷贝直接引用，提升效率
- `map.find(key) != map.end()`：`find` 返回一个迭代器，如果找到了就指向那个元素，没找到就等于 `end()`（表示"超出末尾"的位置）
- `map.erase(key)`：删除指定 key 的键值对，如果 key 不存在也不会报错

### 4.2 使用示例

```cpp
MyHashSet<int> s;
s.add(1);
s.add(2);
s.add(3);
s.add(2);          // 2 已存在，什么都不发生

s.contains(2);     // true
s.contains(4);     // false

s.remove(2);
s.contains(2);     // false

s.size();           // 2（剩下 1 和 3）
```

---

## 五、竞赛中直接用标准库

实际写题时不需要自己封装，直接用 `unordered_set`：

```cpp
#include <unordered_set>
using namespace std;

unordered_set<int> s;

s.insert(10);          // 添加
s.erase(10);           // 删除
s.count(10);           // 判断是否存在（返回 0 或 1）

// 遍历（顺序不确定）
for (int x : s) {
    cout << x << " ";
}
```

### unordered_set vs set

| | `unordered_set` | `set` |
|---|----------------|-------|
| 底层实现 | 哈希表 | 红黑树 |
| 增删查 | 平均 O(1) | O(log N) |
| 元素有序？ | 无序 | 自动排序 |
| 适用场景 | 只需要去重/判断存在 | 需要有序遍历 |

---

## 六、哈希集合的限制（继承自哈希表）

因为底层就是哈希表，所以哈希表的那些限制都适用：

| 限制 | 原因 |
|------|------|
| 遍历顺序不可依赖 | 扩容后元素位置会变 |
| 元素必须是不可变类型 | 如果元素变了，hash 值变了，就找不到了 |
| 遍历时不要增删元素 | 可能触发扩容导致混乱 |

---

## 七、常见应用场景

| 场景 | 示例 |
|------|------|
| **去重** | 从数组中找出不重复的元素 |
| **快速判断存在性** | 判断一个单词是否在字典中 |
| **集合运算** | 求两个数组的交集、并集、差集 |
| **标记已访问** | BFS/DFS 中记录已经访问过的节点 |

### 经典例题：两个数组的交集

```cpp
// LeetCode 349: 给两个数组，找出它们的交集
vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
    unordered_set<int> s(nums1.begin(), nums1.end()); // 把 nums1 去重放入集合
    vector<int> result;

    for (int x : nums2) {
        if (s.count(x)) {       // nums2 的元素在集合中存在
            result.push_back(x);
            s.erase(x);         // 避免重复加入结果
        }
    }
    return result;
}
```

---

## 八、知识图谱

```
哈希集合
├── 本质：只用了 key 的哈希表（value 用占位符填充）
├── 核心能力：去重 + O(1) 增删查
├── API 对应关系
│   ├── add(key)      → map[key] = true
│   ├── contains(key) → map.find(key) != map.end()
│   └── remove(key)   → map.erase(key)
├── 限制（继承自哈希表）
│   ├── 遍历顺序不可依赖
│   ├── 元素必须不可变
│   └── 遍历时不增删
├── C++ 标准库
│   ├── unordered_set → 哈希表实现，O(1)，无序
│   └── set → 红黑树实现，O(log N)，有序
└── 常见应用
    ├── 去重
    ├── 判断存在性
    ├── 集合运算（交集/并集/差集）
    └── BFS/DFS 标记已访问
```
