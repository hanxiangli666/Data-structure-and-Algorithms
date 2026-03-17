# 并查集（Union-Find）原理 学习笔记

> **前置知识**：二叉树基础、图的连通性概念  
> **核心能力**：O(1) 时间内合并两个集合、O(1) 时间内判断两个节点是否连通、O(1) 时间内查询连通分量数量  
> **竞赛重要性**：⭐⭐⭐⭐⭐ ICPC 高频考点，Kruskal 算法的核心工具

---

## 一、什么问题需要并查集？

### 1.1 动态连通性问题

给你一堆节点，不断执行「连接」操作，同时随时查询：

- 节点 A 和节点 B **是否连通**？
- 当前有**多少个连通分量**？

```
初始：10 个孤立节点（10 个连通分量）
  0  1  2  3  4  5  6  7  8  9

执行 union(0, 1)：
  {0,1}  2  3  4  5  6  7  8  9   → 9 个连通分量

执行 union(1, 2)：
  {0,1,2}  3  4  5  6  7  8  9    → 8 个连通分量

查询 connected(0, 2) → true（0 和 2 通过 1 间接连通）
查询 connected(0, 3) → false
查询 count() → 8
```

### 1.2 没有并查集怎么做？

| 操作 | 用邻接表/BFS/DFS | 用并查集 |
|------|-----------------|---------|
| union（连接） | O(1)（加一条边） | **O(1)** |
| connected（查询连通） | **O(V+E)**（要 BFS/DFS 遍历） | **O(1)** |
| count（连通分量数） | **O(V+E)**（遍历整图） | **O(1)** |

用 BFS/DFS 判断连通性太慢了！并查集把这三个操作全部做到 O(1)。

### 1.3 实际应用

| 场景 | 说明 |
|------|------|
| **社交网络** | 判断两个人是否在同一个朋友圈 |
| **Kruskal 算法** | 判断加入一条边是否会形成环（上一篇！） |
| **编译器** | 判断多个变量是否引用同一个对象 |
| **连通分量计数** | 求图中有多少个独立的连通区域 |

---

## 二、并查集的 API

```cpp
class UF {
public:
    UF(int n);                    // 初始化 n 个节点，O(n)
    void union_(int p, int q);    // 连接 p 和 q，O(1)
    bool connected(int p, int q); // p 和 q 是否连通，O(1)
    int count();                  // 连通分量数量，O(1)
};
```

注意 C++ 中 `union` 是关键字，所以一般用 `union_` 或 `merge` 代替。

---

## 三、核心原理：用森林表示连通关系

### 3.1 核心思想

> **每个连通分量用一棵树表示，树的根节点是这个连通分量的「代表」。**

```
初始状态（每个节点自成一棵树，即自己就是根）：
  0   1   2   3   4     → 5 棵树 = 5 个连通分量

union(0, 1)：把 0 的树挂到 1 的树下面
    1       2   3   4   → 4 个连通分量
    |
    0

union(2, 3)：把 2 的树挂到 3 的树下面
    1       3       4   → 3 个连通分量
    |       |
    0       2

union(0, 2)：0 的根是 1，2 的根是 3 → 把 1 的树挂到 3 下面
        3           4   → 2 个连通分量
       / \
      1   2
      |
      0
```

### 3.2 三个操作怎么做？

#### find(x)：找到 x 的根节点

顺着 parent 指针一路向上走，直到根节点（parent 指向自己的节点）。

```cpp
int find(int x) {
    while (parent[x] != x) {
        x = parent[x];    // 一路向上
    }
    return x;              // 到达根节点
}
```

#### connected(p, q)：判断是否连通

如果 p 和 q 的根节点相同 → 在同一棵树 → 连通。

```cpp
bool connected(int p, int q) {
    return find(p) == find(q);
}
```

#### union(p, q)：连接两个节点

找到 p 和 q 各自的根节点，把一棵树的根挂到另一棵树的根下面。

```cpp
void union_(int p, int q) {
    int rootP = find(p);
    int rootQ = find(q);
    if (rootP == rootQ) return;   // 已经连通，不用合并
    parent[rootP] = rootQ;        // 把 p 的树挂到 q 的树下
    cnt--;                        // 连通分量减 1
}
```

#### count()：连通分量数量

维护一个计数器，初始值为 n，每次 union 成功就减 1。

```cpp
int count() { return cnt; }
```

### 3.3 底层只需要一个数组！

不需要真的建树（TreeNode 之类的），只需要一个 `parent` 数组：

```cpp
vector<int> parent(n);

// 初始化：每个节点的父节点是自己（自己就是根）
for (int i = 0; i < n; i++) {
    parent[i] = i;
}
```

```
parent 数组：[0, 1, 2, 3, 4]   （初始：每个人的 parent 是自己）
              ↓
union(0,1)：  [1, 1, 2, 3, 4]   （0 的 parent 变成 1）
              ↓
union(2,3)：  [1, 1, 3, 3, 4]   （2 的 parent 变成 3）
              ↓
union(0,2)：  [1, 3, 3, 3, 4]   （1 的 parent 变成 3）
```

**类比**：每个人头上有个箭头指向自己的「老大」。老大头上的箭头指向自己。要判断两个人是不是一伙的，就各自顺着箭头往上找，看最终的老大是不是同一个人。

---

## 四、性能问题及优化

### 4.1 问题：树可能退化成链表

如果 union 的顺序不好，树会变得很高：

```
union(0,1), union(1,2), union(2,3), union(3,4)

      4
      |
      3
      |
      2
      |
      1
      |
      0

高度 = 5 → find 要走 4 步 → O(N) ❌
```

**类比**：就像一条很长的「食物链」，找最终老大要从链条底端一路爬上去。

### 4.2 优化一：按大小合并（Weighted Union）

**思路**：union 时，把**小树挂到大树**下面，而不是随便挂。

```cpp
vector<int> size(n, 1);   // 每棵树的节点数

void union_(int p, int q) {
    int rootP = find(p), rootQ = find(q);
    if (rootP == rootQ) return;
    
    // 小树挂到大树下面
    if (size[rootP] < size[rootQ]) {
        parent[rootP] = rootQ;
        size[rootQ] += size[rootP];
    } else {
        parent[rootQ] = rootP;
        size[rootP] += size[rootQ];
    }
    cnt--;
}
```

**效果**：树高保持 O(log N)，所以 find/union/connected 都是 O(log N)。

```
同样的操作序列，按大小合并：

union(0,1): 0 挂到 1（一样大，随便）
union(1,2): 2 挂到 1（{0,1} 更大）
union(2,3): 3 挂到 1（{0,1,2} 更大）

    1         而不是退化成链表
   /|\
  0  2  3
  
高度 = 2 ✓
```

### 4.3 优化二：路径压缩（Path Compression）

**思路**：在 find 的过程中，顺便把路径上的所有节点**直接挂到根节点下面**。

```cpp
int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);   // 递归找根，顺便把自己直接连到根
    }
    return parent[x];
}
```

**效果**：不管树原来多高，一次 find 之后整条路径都被压平为高度 2。

```
路径压缩前：              路径压缩后（find(0) 触发）：
      4                         4
      |                        /|\ \
      3                       0  1  2  3
      |
      2                   整条链被压扁了！
      |
      1
      |
      0
```

**类比**：你第一次找老大，一路问了好几个中间人。找到之后你直接记住了老大是谁，下次直接找老大，不用再经过中间人了。而且你顺手帮路上碰到的所有人也记住了老大。

### 4.4 两种优化的效果对比

| 优化 | find 复杂度 | 说明 |
|------|------------|------|
| 无优化 | O(N) | 可能退化成链表 |
| 按大小合并 | O(log N) | 树保持平衡 |
| 路径压缩 | **均摊 O(1)** | 树高被压平 |
| **两者结合** | **均摊 O(α(N)) ≈ O(1)** | α 是反阿克曼函数，增长极慢 |

> **竞赛中一般两种优化都用上**，这样所有操作的均摊复杂度就是 O(1)。

---

## 五、完整并查集模板（竞赛用）

```cpp
class UF {
    vector<int> parent;
    vector<int> size;
    int cnt;   // 连通分量数

public:
    UF(int n) : parent(n), size(n, 1), cnt(n) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    // 路径压缩的 find
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // 路径压缩
        }
        return parent[x];
    }

    // 按大小合并的 union
    void merge(int p, int q) {
        int rootP = find(p), rootQ = find(q);
        if (rootP == rootQ) return;
        // 小树挂到大树下
        if (size[rootP] < size[rootQ]) {
            parent[rootP] = rootQ;
            size[rootQ] += size[rootP];
        } else {
            parent[rootQ] = rootP;
            size[rootP] += size[rootQ];
        }
        cnt--;
    }

    bool connected(int p, int q) {
        return find(p) == find(q);
    }

    int count() { return cnt; }
};
```

**这个模板要能默写出来**，竞赛中直接拿来用。

---

## 六、关键概念速查表

| 概念 | 说明 |
|------|------|
| **并查集** | 用森林（多棵树）表示连通关系的数据结构 |
| **parent 数组** | `parent[i]` = 节点 i 的父节点，根节点的 parent 是自己 |
| **find(x)** | 找 x 的根节点，判断属于哪个连通分量 |
| **union(p,q)** | 合并 p 和 q 所在的连通分量（合并两棵树的根） |
| **connected(p,q)** | 判断 p 和 q 是否连通（根是否相同） |
| **按大小合并** | 小树挂到大树下，保持 O(log N) |
| **路径压缩** | find 时顺便把路径上的节点直接挂到根下，均摊 O(1) |
| **α(N)** | 反阿克曼函数，增长极慢（N < 10⁸⁰ 时 α(N) < 5），可以视为常数 |

---

## 七、练习题

| 题号 | 题目 | 说明 |
|------|------|------|
| **LeetCode 547** | 省份数量 | 并查集入门题，求连通分量数 |
| LeetCode 684 | 冗余连接 | 并查集判环（union 时发现已连通=有环） |
| LeetCode 200 | 岛屿数量 | 可以用并查集做（也可以 DFS） |
| **LeetCode 1584** | 连接所有点的最小费用 | Kruskal + 并查集 |
| LeetCode 399 | 除法求值 | 带权并查集（进阶） |

建议先做 547 和 684，确保并查集模板能熟练写出来。然后做 1584 把并查集和 Kruskal 结合起来。

---

*笔记整理时间：2026-03-17*
