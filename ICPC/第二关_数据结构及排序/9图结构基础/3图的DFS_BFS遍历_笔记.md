# 图的 DFS/BFS 遍历 学习笔记

> **前置知识**：图的存储（邻接表/邻接矩阵）、多叉树遍历  
> **核心认知**：图的遍历 = 多叉树遍历 + **visited 数组防止走回头路**（因为图可能有环）  
> **三种遍历场景**：遍历所有「节点」、所有「边」、所有「路径」——代码写法不同，要区分清楚

---

## 一、为什么图遍历需要 visited 数组？

树没有环，从根到任何节点的路径唯一，不可能走回头路。但图可能有环：

```
    1 ←→ 2
    
不加 visited：1 → 2 → 1 → 2 → 1 → ... 无限循环！
加了 visited：1 → 2 → (1 已访问，跳过) → 结束 ✓
```

> **visited 数组的作用**：记录哪些节点已经访问过，防止在环中死循环。

---

## 二、DFS 遍历所有节点（最基本的场景）

### 2.1 对比：多叉树 vs 图

```cpp
// 多叉树 DFS
void traverse(Node* root) {
    if (root == nullptr) return;
    cout << root->val << endl;           // 前序位置
    for (Node* child : root->children) {
        traverse(child);
    }
}

// 图 DFS —— 多了一个 visited 数组
void traverse(Graph& graph, int s, vector<bool>& visited) {
    if (visited[s]) return;               // 已访问，防止死循环
    visited[s] = true;                    // 标记为已访问
    cout << s << endl;                    // 前序位置
    for (auto& e : graph.neighbors(s)) {
        traverse(graph, e.to, visited);
    }
}
```

**区别只有一处**：多了 `visited` 数组，在前序位置标记，之后不再撤销。

### 2.2 图解

```
图：
    0 ──→ 1 ──→ 3
    │     ↑
    ↓     │
    2 ────┘

DFS 从 0 开始：
  访问 0, visited = {0}
    访问 1, visited = {0,1}
      访问 3, visited = {0,1,3}
      3 没有邻居，返回
    回到 1，邻居都访问过了，返回
    访问 2, visited = {0,1,2,3}
      2 的邻居是 1，但 1 已访问，跳过
  
遍历顺序：0 → 1 → 3 → 2
```

### 2.3 时间复杂度：O(V + E)

每个节点被访问**一次**（V 次），每条边被检查**一次**（E 次），总共 O(V + E)。

**为什么不是 O(V)？** 因为对每个节点，你要遍历它的所有邻居（边）来决定下一步走哪。即使邻居已经被访问过（直接跳过），检查这条边本身也消耗了时间。

---

## 三、DFS 遍历所有路径（回溯场景）

### 3.1 和遍历节点的关键区别

| 对比 | 遍历所有节点 | 遍历所有路径 |
|------|-------------|-------------|
| 用什么数组 | `visited` | `onPath` |
| 前序位置 | 标记 `true` | 标记 `true` |
| 后序位置 | **不撤销** | **撤销标记 `false`** |
| 每个节点访问次数 | 最多 1 次 | 可能多次 |
| 复杂度 | O(V + E) | 指数级或阶乘级 |

**为什么要撤销？**

因为同一个节点可能出现在**不同的路径**上。如果不撤销标记，走完一条路径后，其他经过该节点的路径就走不了了。

### 3.2 图解

```
图：
    1 ──→ 2 ──→ 4
    │     ↑
    ↓     │
    3 ────┘

找 1 到 4 的所有路径：

路径 1: 1 → 2 → 4  ✓
  （走完后，撤销 2 和 4 的标记）
路径 2: 1 → 3 → 2 → 4  ✓
  （因为 2 的标记被撤销了，所以可以再走一次）

如果不撤销：
  走完 1→2→4 后，2 和 4 被永久标记
  走 1→3→2 时发现 2 已标记，跳过 → 丢失路径 2 ❌
```

### 3.3 代码

```cpp
vector<bool> onPath(n, false);
vector<int> path;

void traverse(Graph& graph, int src, int dest) {
    if (onPath[src]) return;          // 防止在环中死循环

    if (src == dest) {
        // 找到一条路径！
        path.push_back(dest);
        printPath(path);              // 输出路径
        path.pop_back();
        return;
    }

    // 前序位置：进入节点
    onPath[src] = true;
    path.push_back(src);

    for (auto& e : graph.neighbors(src)) {
        traverse(graph, e.to, dest);
    }

    // 后序位置：离开节点，撤销标记！
    path.pop_back();
    onPath[src] = false;              // ← 关键：撤销标记
}
```

**类比回溯算法**：这就是**回溯**的模式——进入时做选择（标记），离开时撤销选择（撤销标记）。后面学回溯算法时会反复用到这个模式。

### 3.4 特殊情况：有向无环图（DAG）不需要 onPath

如果题目明确说**图无环**，就不用担心死循环，onPath 数组可以省掉。

```cpp
// LeetCode 797：所有可能的路径（有向无环图）
class Solution {
    vector<vector<int>> res;
    vector<int> path;

public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        traverse(graph, 0);
        return res;
    }

    void traverse(vector<vector<int>>& graph, int s) {
        path.push_back(s);

        if (s == graph.size() - 1) {
            res.push_back(path);       // 到达终点，记录路径
            path.pop_back();
            return;
        }

        for (int v : graph[s]) {
            traverse(graph, v);        // 无环，不需要 visited/onPath
        }

        path.pop_back();              // 回溯：撤销选择
    }
};
```

---

## 四、visited vs onPath vs 都不用：总结

| 场景 | 用什么 | 后序是否撤销 | 复杂度 |
|------|--------|-------------|--------|
| 遍历所有**节点** | `visited` | ❌ 不撤销 | O(V+E) |
| 遍历所有**边** | 二维 `visited[u][v]` | ❌ 不撤销 | O(E+V²) |
| 遍历所有**路径** | `onPath` | ✅ **要撤销** | 指数级 |
| **无环图**遍历路径 | 不需要任何数组 | — | 取决于路径数 |
| 判断是否成环 | `visited` + `onPath` **同时用** | onPath 撤销 | O(V+E) |

**口诀**：
- 遍历节点 → visited 标了不撤
- 遍历路径 → onPath 标了要撤（回溯）
- 无环图 → 啥都不用

---

## 五、BFS 遍历图

### 5.1 和多叉树 BFS 的区别

和 DFS 一样，唯一区别就是加了 visited 数组。

### 5.2 三种写法（和二叉树层序遍历完全对应）

#### 写法一：最简单，不记录步数

```cpp
void bfs(Graph& graph, int s) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;
    q.push(s);
    visited[s] = true;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        cout << cur << endl;
        
        for (auto& e : graph.neighbors(cur)) {
            if (!visited[e.to]) {
                q.push(e.to);
                visited[e.to] = true;     // 入队时就标记！
            }
        }
    }
}
```

#### 写法二：记录步数（最常用！）

```cpp
void bfs(Graph& graph, int s) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    int step = 0;

    while (!q.empty()) {
        int sz = q.size();               // 当前层的节点数
        for (int i = 0; i < sz; i++) {
            int cur = q.front();
            q.pop();
            cout << "visit " << cur << " at step " << step << endl;

            for (auto& e : graph.neighbors(cur)) {
                if (!visited[e.to]) {
                    q.push(e.to);
                    visited[e.to] = true;
                }
            }
        }
        step++;
    }
}
```

对于**无权图**，`step` 就是从起点到当前节点的**最短距离**（边数）。

#### 写法三：每个节点自带状态（最灵活）

```cpp
struct State {
    int node;
    int step;     // 也可以换成 distFromSrc 等任意信息
};

void bfs(Graph& graph, int s) {
    vector<bool> visited(graph.size(), false);
    queue<State> q;
    q.push({s, 0});
    visited[s] = true;

    while (!q.empty()) {
        auto [cur, step] = q.front();
        q.pop();
        cout << "visit " << cur << " step=" << step << endl;

        for (auto& e : graph.neighbors(cur)) {
            if (!visited[e.to]) {
                q.push({e.to, step + 1});
                visited[e.to] = true;
            }
        }
    }
}
```

> 写法三稍加修改，就变成了 **Dijkstra 算法**（把 queue 换成 priority_queue，step+1 换成 step+weight）。后面学最短路算法时会用到。

### 5.3 visited 标记的时机：入队时标记！

注意上面代码中，`visited[e.to] = true` 是在**入队时**标记的，而不是出队时。

```cpp
// ✅ 正确：入队时标记
if (!visited[e.to]) {
    q.push(e.to);
    visited[e.to] = true;    // 立刻标记
}

// ❌ 错误：出队时标记
// 可能导致同一个节点被重复入队
int cur = q.front(); q.pop();
visited[cur] = true;          // 太晚了！
```

**为什么？** 如果出队时才标记，节点 A 可能从不同的邻居被多次加入队列，浪费时间和空间。

---

## 六、关键概念速查表

| 概念 | 说明 |
|------|------|
| **visited 数组** | 标记已访问节点，前序标记，不撤销 |
| **onPath 数组** | 标记当前路径上的节点，前序标记，后序撤销 |
| **回溯** | 进入标记 + 离开撤销 = 回溯模式 |
| **DAG** | 有向无环图，不需要防环处理 |
| **BFS 入队时标记** | visited 要在入队时就设 true，不要等出队 |
| **DFS 遍历复杂度** | O(V+E)，遍历节点场景 |
| **BFS 写法二** | 记录 step，无权图中 step = 最短距离 |
| **BFS 写法三** | State 自带状态，Dijkstra 的前身 |

---

## 七、练习题

| 题号 | 题目 | 考察点 |
|------|------|--------|
| **LeetCode 797** | 所有可能的路径 | DAG 上 DFS 遍历所有路径，不需要 visited |
| LeetCode 200 | 岛屿数量 | DFS/BFS + visited，求连通分量数 |
| LeetCode 733 | 图像渲染 | BFS 经典入门 |
| LeetCode 542 | 01 矩阵 | BFS 写法二，多源 BFS |
| LeetCode 207 | 课程表 | DFS 判断有向图是否成环（visited + onPath 同时用） |
| LeetCode 210 | 课程表 II | 拓扑排序，基于 207 的扩展 |

**建议顺序**：先做 797（最简单，DAG 无环），再做 200（DFS 遍历节点），然后做 207（visited + onPath 同时用，体会区别）。

---

*笔记整理时间：2026-03-17*
