# Trie 树（字典树/前缀树）原理 学习笔记

> **前置知识**：二叉树基础、多叉树遍历  
> **核心认知**：Trie 树 = 针对字符串特殊优化的多叉树，用「树枝」存字符，用「节点」存值  
> **本篇定位**：理解原理和 API，代码实现等刷完二叉树习题后再学。竞赛中遇到相关题目可以直接套模板

---

## 一、Trie 树是什么？

### 1.1 一句话定义

> Trie 树是一种**多叉树**，专门用来高效存储和查找字符串。多个字符串的**公共前缀只存一份**，所以也叫「前缀树」。

你可以把 Trie 的价值理解成“把重复前缀压缩成公共路径”。字符串越长、前缀越相似，这个优势越明显。它不是为了替代所有 Map，而是专门解决字符串前缀问题。场景对了，它会非常强。

> 💡 **老师提醒：** Trie 最适合“大量字符串 + 前缀查询频繁”的题型，别拿它硬解普通整数映射题。

### 1.2 从二叉树到 Trie 树的演化

```
二叉树节点：                多叉树节点：              Trie 树节点：
  val                        val                      val
  left, right               children[]               children[256]
  (固定 2 个孩子)            (任意个孩子)              (256 个孩子槽位，索引=字符)
```

Trie 树的 `children` 数组大小是 256（ASCII 字符数），**索引本身代表一个字符**。比如 `children[97]` 非空，表示有一条标着字符 `'a'` 的边（因为 `'a'` 的 ASCII 码是 97）。

### 1.3 图示理解

假设存储了 `"the"=2`、`"them"=3`、`"that"=1`、`"apple"=4`：

```
          (root)
         /      \
        t        a
        |        |
        h        p
       / \       |
      e   a      p
      |   |      |
     [2]  t      l
      |   |      |
      m  [1]     e
      |         [4]
     [3]

[数字] = 节点上存储的 val 值
没有数字的节点 = val 为空（只是路径中间的过渡节点）
```

**关键理解**：

- **字符存在树枝上**（从父到子的边），不存在节点里
- **值存在节点上**（`val` 字段）
- 只有 `val` 非空的节点才代表一个完整的键。比如 `"the"` 的路径 `t→h→e` 结尾处的节点 `val=2`，说明 `"the"` 是一个键
- `"th"` 的路径虽然存在，但结尾节点的 `val` 为空，说明 `"th"` 不是一个键，只是其他键的前缀

这组区分非常关键。很多同学一看到路径存在就以为字符串存在，结果 `search` 和 `startsWith` 全写混了。你一定要分清：路径存在只说明“是前缀”，节点打标记才说明“是完整单词”。这就是 Trie 判定逻辑的根。

> ✅ 你已经踩到 Trie 的核心判断点了：路径和完整键是两件事。

### 1.4 C++ 节点结构

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
char ch = 'a';                    // 像路牌上的一个字母
int idx = ch - 'a';               // 把字母映射成数组位置
cout << idx;                      // 看这个字母该走哪条分支
```

> 上面这个例子演示的是“字符映射到分支索引”，下面的真实代码做的事情一样，只是把分支放进 Trie 节点结构里。

```cpp
template<typename V>
struct TrieNode {
    V val = NULL;                      // 节点值：为空表示这里只是前缀中转站
    TrieNode<V>* children[256] = {};   // ⚠️ 指针数组：每个槽位对应一个 ASCII 字符分支
};
```

**题目优化**：如果题目说只包含小写字母 `a-z`，可以把 256 改成 26，`children` 的索引用 `c - 'a'` 映射。或者直接用哈希表 `unordered_map<char, TrieNode*>` 代替数组。

这就是空间和时间的权衡。数组访问快，但可能浪费空间；哈希表更省空间，但常数会大一些。比赛里如果字符集很小，数组通常更稳更快。字符集不固定再考虑哈希。

*就像“固定格子的收纳盒”和“按需挂袋”的区别。*

---

## 二、Trie 树的四大优势

### 2.1 节约存储空间——公共前缀只存一份

```
用 HashMap 存 "apple", "app", "appl"：
  → 真的创建 3 个字符串，占 5+3+4 = 12 个字符的空间

用 Trie 树存 "apple", "app", "appl"：
  → 公共前缀共享，只需 "apple" 这 5 个字符的空间

  (root)
    |
    a
    |
    p
    |
    p ← [val=2] "app" 的值
    |
    l ← [val=3] "appl" 的值
    |
    e ← [val=1] "apple" 的值
```

当键很多、很长、且有大量公共前缀时（比如身份证号、URL），节约效果非常显著。

### 2.2 方便处理前缀操作

这是 Trie 树最核心的优势，HashMap 和 TreeMap 都做不到（只能暴力遍历所有键）。

| 操作 | 功能 | 示例 | 复杂度 |
|------|------|------|--------|
| `shortestPrefixOf(query)` | 在所有键中找 query 的最短前缀 | `shortestPrefixOf("themxyz")` → `"the"` | O(L) |
| `longestPrefixOf(query)` | 在所有键中找 query 的最长前缀 | `longestPrefixOf("themxyz")` → `"them"` | O(L) |
| `hasKeyWithPrefix(prefix)` | 是否有以 prefix 开头的键 | `hasKeyWithPrefix("tha")` → `true` | O(L) |
| `keysWithPrefix(prefix)` | 返回所有以 prefix 开头的键 | `keysWithPrefix("th")` → `["that","the","them"]` | O(L+结果数) |

其中 L 是前缀/查询字符串的长度。

**类比**：Trie 树就像一本真正的字典。你要找所有以 "pre-" 开头的单词，只需要翻到 "pre-" 那一页开始往后看就行了。而 HashMap 就像一堆散乱的卡片，你只能一张一张翻过去看是不是以 "pre-" 开头。

**实际应用**：搜索引擎的**自动补全**功能。你输入 "th"，系统用 `keysWithPrefix("th")` 找到所有以 "th" 开头的关键词推荐给你。

前缀操作能高效，是因为 Trie 在结构上把同前缀词天然放在同一片子树里。先定位前缀末尾节点，再向下 DFS，就能拿到所有候选。这个过程不是“全量扫描”，而是“局部展开”。所以复杂度才会好看。

> 💡 **老师提醒：** 写 `keysWithPrefix` 时要注意结果可能很大，复杂度里要把“结果数量”算进去。

### 2.3 支持通配符匹配

```
存储了 "that"=1, "the"=2, "team"=3, "zip"=4

keysWithPattern("t.a.")  → ["team", "that"]    // . 匹配任意字符
hasKeyWithPattern(".ip")  → true                 // 匹配 "zip"
hasKeyWithPattern("z.o")  → false                // 没有匹配的
```

这个功能 HashMap 和 TreeMap **完全做不到**。

**实际应用**：搜索引擎的模糊匹配、正则表达式的简化版。

### 2.4 按字典序遍历键

```
存储了 "that", "the", "them", "zip", "apple"

keys() → ["apple", "that", "the", "them", "zip"]   // 自动按字典序排列
```

这个功能 TreeMap 也能做到（TreeMap 按键排序），但 HashMap 做不到。

*就像“按拼音排好的词典”和“随手塞卡片盒”的区别。*

> ✅ 你已经能看出 Trie 和 HashMap 的边界了，这个判断能力很重要。

---

## 三、TrieMap 完整 API 一览

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
string prefix = "ap";              // 像你先说了开头两个字母
vector<string> ans;                 // 准备一个清单装候选词
ans.push_back("apple");             // 把匹配到的词放进去
```

> 上面的最小例子是“给前缀收集候选词”，下面这段 API 就是把这些动作系统化封装成 TrieMap 的接口。

```cpp
template <typename V>
class TrieMap {
public:
    // ===== 基本增删查改 =====
    void put(string key, V val);         // 插入/更新键值对
    V get(string key);                    // 查找键对应的值
    void remove(string key);              // 删除键值对
    bool containsKey(string key);         // 是否包含某个键
    int size();                           // 返回键值对总数

    // ===== 前缀操作（Trie 独有优势）=====
    string shortestPrefixOf(string query);       // query 的最短前缀键
    string longestPrefixOf(string query);        // query 的最长前缀键
    vector<string> keysWithPrefix(string prefix);// 返回所有以 prefix 开头的键
    bool hasKeyWithPrefix(string prefix);        // 判断是否存在此前缀

    // ===== 通配符匹配（Trie 独有优势）=====
    vector<string> keysWithPattern(string pattern);    // . 匹配任意字符
    bool hasKeyWithPattern(string pattern);            // 是否存在匹配模式的键

    // ===== 遍历 =====
    vector<string> keys();               // 返回所有键（按字典序）
};
```

TrieSet 就是 TrieMap 的简单封装，只存键，不存值（和 HashSet 包装 HashMap 一样的套路）。

接口拆清楚以后，写题时你会更快定位要调用哪个函数。比如“判断存在”选 `has...`，“拿具体列表”选 `keys...`。把 API 名字当成意图表达，代码会更像自然语言。可读性会上一个台阶。

---

## 四、Trie 树 vs HashMap vs TreeMap

| 对比维度 | HashMap | TreeMap | **TrieMap** |
|---------|---------|---------|------------|
| 键类型 | 任意 | 任意（需可比较） | **只能是字符串** |
| 增删查改 | O(1) | O(log N) | **O(L)**，L=键长度 |
| 按序遍历 | ❌ | ✅ | ✅（字典序） |
| 前缀操作 | ❌ | ❌ | **✅ O(L)** |
| 通配符匹配 | ❌ | ❌ | **✅** |
| 空间效率 | 每个键独立存储 | 每个键独立存储 | **公共前缀共享** |
| 适用场景 | 通用 | 需要有序 | **字符串密集型操作** |

**关键认知**：Trie 树不是万能的，它只能处理字符串键。但在字符串场景下，它的前缀操作和通配符匹配是其他数据结构**无法替代的**。

容器选型不是“谁更高级”，而是“谁更匹配问题结构”。Trie 把键拆成字符路径，所以它天然适配前缀。HashMap 把键打散到桶里，所以它擅长等值查找。先看题目操作类型，再选结构，成功率更高。

> 💡 **老师提醒：** 题目关键词出现“前缀、字典序、模糊匹配”，优先想到 Trie。

---

## 五、Map/Set 家族全景图

到目前为止学过的所有 Map/Set 结构：

```
Map/Set 家族
│
├── 基于哈希表
│   ├── HashMap / HashSet          → O(1) 增删查改，无序
│   ├── LinkedHashMap / Set        → O(1) + 按插入顺序遍历
│   └── ArrayHashMap / Set         → O(1) + O(1) 随机返回键
│
├── 基于二叉搜索树
│   └── TreeMap / TreeSet          → O(logN)，按大小有序，支持排名/区间
│       └── 底层：红黑树（自平衡）
│
└── 基于 Trie 树
    └── TrieMap / TrieSet          → O(L)，字符串专用，前缀/通配符
```

**选择指南**：

| 需求 | 选哪个 |
|------|-------|
| 普通增删查改，不关心顺序 | HashMap |
| 需要按插入顺序遍历 | LinkedHashMap |
| 需要 O(1) 随机返回 | ArrayHashMap |
| 需要按大小排序 / 排名 / 区间查询 | TreeMap |
| **字符串的前缀操作 / 通配符匹配** | **TrieMap** |

---

## 六、竞赛中的 Trie 树

### 6.1 常见考题类型

| 题型 | 说明 |
|------|------|
| **前缀匹配** | 判断某个字符串是否是已知字符串集的前缀 |
| **自动补全** | 给出前缀，返回所有匹配的字符串 |
| **最长公共前缀** | 多个字符串的最长公共前缀 |
| **字符串计数** | 统计以某前缀开头的字符串数量 |
| **异或最大值** | 把数字的二进制表示存入 Trie，找异或最大的一对（进阶） |

### 6.2 竞赛中的简化实现

题目通常只需要 Trie 的核心功能（插入、查找、前缀判断），可以用非常精简的代码：

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
bool isEnd = false;                 // 这站是不是终点站
isEnd = true;                       // 到了完整单词结尾就打标记
cout << isEnd;                      // 读取是否为完整单词
```

> 上面的例子演示“终点标记”的核心思想，下面的完整代码把这个标记和分支数组结合成可用的 Trie。

```cpp
// 竞赛中常用的精简 Trie 实现（只处理小写字母 a-z）
struct TrieNode {
    TrieNode* children[26] = {};    // 26 个字母分支，初始全是 nullptr
    bool isEnd = false;             // 到这里是否形成完整单词
    int count = 0;                  // 经过该节点的字符串数量（可选统计）
};

class Trie {
    TrieNode* root;                 // ⚠️ 指针：根节点地址
public:
    Trie() { root = new TrieNode(); } // 构造时创建根节点

    // 插入字符串
    void insert(string word) {
        TrieNode* node = root;      // 从根开始走
        for (char c : word) {       // 逐字符处理
            int idx = c - 'a';      // 把字符映射到 0~25
            if (node->children[idx] == nullptr) {
                node->children[idx] = new TrieNode(); // 没路就新建节点
            }
            node = node->children[idx]; // 往下一层走
            node->count++;             // 经过此节点的字符串数 +1
        }
        node->isEnd = true;            // 到末尾后标记完整单词
    }

    // 查找字符串是否存在
    bool search(string word) {
        TrieNode* node = root;      // 从根开始匹配
        for (char c : word) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                return false;       // 路径断了，单词不存在
            }
            node = node->children[idx];
        }
        return node->isEnd;         // ⚠️ 末尾还要看 isEnd，防止把前缀当完整词
    }

    // 是否存在以 prefix 开头的字符串
    bool startsWith(string prefix) {
        TrieNode* node = root;      // 同样从根出发
        for (char c : prefix) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                return false;
            }
            node = node->children[idx];
        }
        return true;                // 路径没断，说明存在此前缀
    }
};
```

**C++ 知识点**：
- `c - 'a'`：把字符 `'a'`~`'z'` 映射到索引 0~25
- `for (char c : word)`：范围 for 循环，遍历字符串的每个字符
- `TrieNode* children[26] = {}`：初始化数组所有元素为 `nullptr`

> ✅ 你已经掌握了 Trie 最核心的三板斧：insert、search、startsWith。

### 6.3 search vs startsWith 的区别

```
Trie 中存了 "apple"

search("app")     → false   （路径存在，但 "app" 结尾处 isEnd=false）
startsWith("app") → true    （路径存在就够了，不要求是完整的键）
search("apple")   → true    （路径存在，且 isEnd=true）
```

---

## 七、关键概念速查表

| 概念 | 说明 |
|------|------|
| **Trie 树** | 针对字符串优化的多叉树，公共前缀共享存储 |
| **树枝存字符** | 字符存在父→子的边上（children 数组的索引） |
| **节点存值** | val 字段存键对应的值，val 为空表示不是完整的键 |
| **isEnd** | 简化版中标记是否是某个字符串的结尾 |
| **children[256]** | 完整版，支持所有 ASCII 字符 |
| **children[26]** | 简化版，只支持小写字母 |
| **前缀操作** | Trie 的核心优势，O(L) 复杂度 |
| **通配符匹配** | 用 DFS + 回溯实现 `.` 匹配任意字符 |
| `c - 'a'` | 字符到索引的映射技巧 |

---

## 八、练习题

| 题号 | 题目 | 说明 |
|------|------|------|
| **LeetCode 208** | 实现 Trie（前缀树） | 入门必做题，实现 insert/search/startsWith |
| LeetCode 211 | 添加与搜索单词 | 通配符 `.` 匹配，练习 DFS |
| LeetCode 212 | 单词搜索 II | Trie + 回溯，综合题 |
| LeetCode 648 | 单词替换 | 最短前缀操作 |
| LeetCode 677 | 键值映射 | TrieMap 的前缀求和 |
| LeetCode 14 | 最长公共前缀 | 可以用 Trie 解，也可以直接比较 |

**建议**：先做 208 把基础 Trie 实现一遍，然后做 211 练习通配符匹配。这两道够了，其他进阶题等刷到相关专题时再做。

---

*笔记整理时间：2026-03-17*
