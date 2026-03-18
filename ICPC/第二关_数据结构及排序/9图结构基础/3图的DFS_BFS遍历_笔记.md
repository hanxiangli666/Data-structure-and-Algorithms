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

你可以把 `visited` 理解成“门上贴过的访问贴纸”。贴过一次就不再重复进门，这样你在有环的图里才不会原地打转。图和树最大的实战差异，往往就卡在这个细节。

> 💡 **老师提醒：** `visited` 的大小要和节点编号范围一致，编号从 1 开始时要注意数组开到 `n+1`。

---

## 二、DFS 遍历所有节点（最基本的场景）

### 2.1 对比：多叉树 vs 图

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
vector<bool> checked(3, false);       // 三间教室是否查过作业
int room = 1;                         // 先到 1 号教室
if (!checked[room]) checked[room] = true; // 没查过就做标记
```

**第二步：过渡说明（1-2 句话）**

> 这个例子就是“先判断、再标记”的最小版流程。下面图 DFS 的核心动作完全一样，只是把教室换成图节点。

**第三步：给原始代码加注释**

```cpp
// 多叉树 DFS
void traverse(Node* root) {
    if (root == nullptr) return;          // 空节点直接返回
    cout << root->val << endl;            // 前序位置：先处理当前节点
    for (Node* child : root->children) {
        traverse(child);                  // 递归遍历每个孩子
    }
}

// 图 DFS —— 多了一个 visited 数组
void traverse(Graph& graph, int s, vector<bool>& visited) {
    if (visited[s]) return;                // 节点已访问，直接剪枝
    visited[s] = true;                     // 进入节点就标记，防止环
    cout << s << endl;                     // 前序位置：访问当前节点
    for (auto& e : graph.neighbors(s)) {
        traverse(graph, e.to, visited);    // 沿着每条边继续 DFS
    }
}
```

**区别只有一处**：多了 `visited` 数组，在前序位置标记，之后不再撤销。

*就像景区盖章和迷宫踩点的区别*：盖过章就不需要重复排队，走图同理。

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

> ✅ 你把这个复杂度理解透了，后面分析图算法会越来越稳。

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

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
vector<int> line;                 // 记录今天走过的站点顺序
line.push_back(1);                // 进入 1 号站
line.push_back(3);                // 接着走到 3 号站
line.pop_back();                  // 回退一步，撤销刚才的选择
```

**第二步：过渡说明（1-2 句话）**

> 这就是回溯的核心手感：进栈、探索、出栈。下面代码只是把这个动作放到图路径搜索里，并加上防环判断。

**第三步：给原始代码加注释**

```cpp
vector<bool> onPath(n, false);
vector<int> path;

void traverse(Graph& graph, int src, int dest) {
    if (onPath[src]) return;              // 当前路径上重复出现，说明遇到环

    if (src == dest) {
        // 找到一条完整路径
        path.push_back(dest);             // 把终点补进路径
        printPath(path);                  // 输出这条路径
        path.pop_back();                  // 回溯，恢复现场
        return;
    }

    // 前序位置：进入节点
    onPath[src] = true;                   // 标记当前节点在路径上
    path.push_back(src);                  // 记录路径

    for (auto& e : graph.neighbors(src)) {
        traverse(graph, e.to, dest);      // 继续尝试下一个节点
    }

    // 后序位置：离开节点，撤销标记
    path.pop_back();                      // 撤销路径记录
    onPath[src] = false;                  // ⚠️ 关键：撤销 onPath，给其他路径让路
}
```

> 💡 **老师提醒：** 遍历“所有路径”时，`onPath` 才要撤销；`visited` 场景千万别照抄撤销逻辑。

### 3.4 特殊情况：有向无环图（DAG）不需要 onPath

如果题目明确说**图无环**，就不用担心死循环，onPath 数组可以省掉。

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
vector<int> plan;                  // 一条从起点到终点的路线
plan.push_back(0);                 // 到了 0 号站
plan.push_back(1);                 // 走到 1 号站
plan.pop_back();                   // 探索结束后回退
```

**第二步：过渡说明（1-2 句话）**

> DAG 没有环，所以不用担心在同一路径里兜圈。下面代码只保留“路径记录+回溯”即可。

**第三步：给原始代码加注释**

```cpp
// LeetCode 797：所有可能的路径（有向无环图）
class Solution {
    vector<vector<int>> res;
    vector<int> path;

public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        traverse(graph, 0);              // 从 0 号节点开始 DFS
        return res;                      // 返回所有路径
    }

    void traverse(vector<vector<int>>& graph, int s) {
        path.push_back(s);               // 进入当前节点，加入路径

        if (s == graph.size() - 1) {
            res.push_back(path);         // 到达终点，保存当前路径
            path.pop_back();             // 回溯前恢复
            return;
        }

        for (int v : graph[s]) {
            traverse(graph, v);          // DAG 无环，直接递归
        }

        path.pop_back();                 // 回溯：离开当前节点
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

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
queue<int> q;                       // 排队叫号队列
q.push(1001);                       // 1001 先入队
int cur = q.front(); q.pop();       // 轮到 1001 办理
```

**第二步：过渡说明（1-2 句话）**

> 这三行就是 BFS 的骨架：入队、出队、处理。下面代码是在图场景下把“处理当前节点并扩展邻居”补全。

**第三步：给原始代码加注释**

```cpp
void bfs(Graph& graph, int s) {
    vector<bool> visited(graph.size(), false); // 访问标记数组
    queue<int> q;                              // 普通队列，先进先出
    q.push(s);                                 // 起点入队
    visited[s] = true;                         // 入队即标记，避免重复入队

    while (!q.empty()) {
        int cur = q.front();                   // 取出队首节点
        q.pop();                               // 弹出队首
        cout << cur << endl;                   // 访问当前节点

        for (auto& e : graph.neighbors(cur)) {
            if (!visited[e.to]) {
                q.push(e.to);                  // 邻居首次出现，入队
                visited[e.to] = true;          // ⚠️ 入队时立刻标记
            }
        }
    }
}
```

#### 写法二：记录步数（最常用！）

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
int step = 0;                         // 第几轮点名
int sz = 3;                           // 本轮有 3 个人
for (int i = 0; i < sz; i++) step++; // 一轮一轮处理
```

**第二步：过渡说明（1-2 句话）**

> 写法二把“每一层处理完再 step++”固定下来。下面代码对应到图里，`step` 就是起点到当前层的边数。

**第三步：给原始代码加注释**

```cpp
void bfs(Graph& graph, int s) {
    vector<bool> visited(graph.size(), false); // 标记是否访问
    queue<int> q;
    q.push(s);                                 // 起点入队
    visited[s] = true;                         // 立刻标记
    int step = 0;                              // 当前层数

    while (!q.empty()) {
        int sz = q.size();                     // 当前层节点数量
        for (int i = 0; i < sz; i++) {
            int cur = q.front();
            q.pop();
            cout << "visit " << cur << " at step " << step << endl;

            for (auto& e : graph.neighbors(cur)) {
                if (!visited[e.to]) {
                    q.push(e.to);
                    visited[e.to] = true;      // 邻居入队并标记
                }
            }
        }
        step++;                                // 当前层处理完，进入下一层
    }
}
```

对于**无权图**，`step` 就是从起点到当前节点的**最短距离**（边数）。

> ✅ 你掌握了这条后，很多“最短路入门题”都能直接秒写。

#### 写法三：每个节点自带状态（最灵活）

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
struct Ticket { int id, turn; };      // 人员编号 + 到达轮次
queue<Ticket> q;                      // 队列里放“带信息的人”
q.push({7, 0});                       // 7 号在第 0 轮入队
```

**第二步：过渡说明（1-2 句话）**

> 你看到的关键变化是：队列里不再只放节点编号，而是放“节点+额外状态”。下面代码就是这个模式的图版本。

**第三步：给原始代码加注释**

```cpp
struct State {
    int node;                          // 当前节点
    int step;                          // 当前步数（也可换成其他状态）
};

void bfs(Graph& graph, int s) {
    vector<bool> visited(graph.size(), false);
    queue<State> q;
    q.push({s, 0});                    // 起点状态入队
    visited[s] = true;

    while (!q.empty()) {
        auto [cur, step] = q.front();  // ⚠️ 结构化绑定：拆出节点与步数
        q.pop();
        cout << "visit " << cur << " step=" << step << endl;

        for (auto& e : graph.neighbors(cur)) {
            if (!visited[e.to]) {
                q.push({e.to, step + 1}); // 邻居状态入队，步数 +1
                visited[e.to] = true;
            }
        }
    }
}
```

> 写法三稍加修改，就变成了 **Dijkstra 算法**（把 queue 换成 priority_queue，step+1 换成 step+weight）。后面学最短路算法时会用到。

### 5.3 visited 标记的时机：入队时标记！

注意上面代码中，`visited[e.to] = true` 是在**入队时**标记的，而不是出队时。

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
queue<int> q;                        // 排队名单
vector<bool> seen(10, false);        // 是否已经拿过号
q.push(3); seen[3] = true;           // 入队同时打标记
```

**第二步：过渡说明（1-2 句话）**

> 重点只有一句：先标记再等待出队。下面代码用“正确/错误”对照，帮你防住 BFS 最常见 bug。

**第三步：给原始代码加注释**

```cpp
// ✅ 正确：入队时标记
if (!visited[e.to]) {
    q.push(e.to);                 // 第一次发现该节点，入队
    visited[e.to] = true;         // 立刻标记，杜绝重复入队
}

// ❌ 错误：出队时标记
// 可能导致同一个节点被重复入队
int cur = q.front(); q.pop();
visited[cur] = true;              // 太晚：它可能已经进队多次
```

**为什么？** 如果出队时才标记，节点 A 可能从不同的邻居被多次加入队列，浪费时间和空间。

> 💡 **老师提醒：** BFS 一旦出现“队列莫名很长”，先检查是不是把 `visited` 放到了出队位置。

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