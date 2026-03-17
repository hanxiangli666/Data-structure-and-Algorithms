# 结构体（struct）深入讲解 —— 从基础到竞赛实战

> 前置知识：你已经学过 struct 的基本定义、结合 vector 使用、用 lambda 排序。
> 本文目标：把结构体的知识补全，让你能应对 LeetCode 和竞赛中遇到的各种结构体用法。

---

## 一、快速回顾你已经会的

```cpp
struct Student {
    string name;
    double score;
};

vector<Student> students(n);
sort(students.begin(), students.end(), [](Student &a, Student &b) {
    return a.score > b.score;
});
```

这些是基础，下面全部是新内容。

---

## 二、构造函数 —— 让创建对象更方便

### 问题：每次创建都要一个个赋值，太麻烦

```cpp
struct Point {
    int x, y;
};

Point p;
p.x = 3;
p.y = 4;
// 写三行才创建一个点，太啰嗦了
```

### 解决：给 struct 加构造函数

```cpp
struct Point {
    int x, y;

    // 构造函数：名字和struct名一样，没有返回类型
    Point(int x, int y) : x(x), y(y) {}
};

// 现在可以一行创建
Point p(3, 4);          // 方式一
Point p2 = {3, 4};      // 方式二（C++11）
Point p3{3, 4};          // 方式三（C++11）
```

### `: x(x), y(y)` 是什么？

这叫**初始化列表**，是构造函数专用的语法，意思是"用参数 x 的值初始化成员变量 x"。

等价于：

```cpp
Point(int x, int y) {
    this->x = x;   // this->x 是成员变量，x 是参数
    this->y = y;
}
```

初始化列表写法更简洁，竞赛中几乎都用这种。

### 带默认值的构造函数

```cpp
struct Point {
    int x, y;

    Point() : x(0), y(0) {}           // 无参构造：默认(0,0)
    Point(int x, int y) : x(x), y(y) {} // 有参构造
};

Point p1;        // 调用无参构造，p1 = (0, 0)
Point p2(3, 4);  // 调用有参构造，p2 = (3, 4)
```

**竞赛中为什么需要无参构造？**

因为如果你写了有参构造，C++ 就不再自动提供无参构造了。而 `vector<Point> v(10)` 需要无参构造来初始化 10 个默认元素。

```cpp
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

vector<Point> v(10);  // ❌ 编译错误！没有无参构造函数

// 解决方案一：加一个无参构造
// 解决方案二：用默认参数
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    // 这样 Point() 和 Point(3,4) 都能用
};

vector<Point> v(10);  // ✅ 10个(0,0)
```

---

## 三、运算符重载 —— 让 struct 支持比较和运算

### 问题：想用 sort 但不想每次都写 lambda

```cpp
// 每次排序都要写这么长的lambda
sort(v.begin(), v.end(), [](Student &a, Student &b) {
    return a.score > b.score;
});
```

### 解决：在 struct 内部定义 `<` 运算符

```cpp
struct Student {
    string name;
    double score;

    // 重载 < 运算符
    bool operator<(const Student &other) const {
        if (score != other.score) return score > other.score; // 分数高的排前面
        return name < other.name;  // 分数相同按名字字典序
    }
};

// 现在直接 sort，不需要 lambda！
vector<Student> students;
sort(students.begin(), students.end());  // 自动使用你定义的 <
```

### 逐字解释 `bool operator<(const Student &other) const`

```
bool              → 返回 true 或 false
operator<         → 重新定义 < 这个符号的行为
(const Student &other)  → 和谁比较
                          const 表示不会修改 other
                          & 表示传引用，不复制
const（最后的）   → 表示这个函数不会修改自身
```

**直觉理解：** 你在告诉 C++："当你需要比较两个 Student 谁小谁大的时候，就用这个规则。"

### 其他常用的运算符重载

```cpp
struct Point {
    int x, y;

    // 判断相等
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }

    // 判断不等
    bool operator!=(const Point &other) const {
        return !(*this == other);  // 复用 ==
    }

    // 小于（用于 set、map 等需要排序的容器）
    bool operator<(const Point &other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    // 加法
    Point operator+(const Point &other) const {
        return {x + other.x, y + other.y};
    }
};

Point a = {1, 2}, b = {3, 4};
Point c = a + b;        // c = {4, 6}
cout << (a == b);       // 0 (false)
```

### 为什么竞赛中需要运算符重载？

**场景一：把 struct 放进 set 或 map**

`set` 和 `map` 内部是有序的，它需要知道怎么比较你的元素。

```cpp
struct Point {
    int x, y;
    bool operator<(const Point &other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

set<Point> visited;            // ✅ 因为有 < 运算符
visited.insert({1, 2});
visited.insert({3, 4});

map<Point, int> dist;          // ✅ 用 Point 做键
dist[{1, 2}] = 5;
```

如果没有 `operator<`，放进 `set` 或 `map` 会编译报错。

**场景二：用 priority_queue**

```cpp
struct Task {
    int priority;
    string name;

    // priority_queue 默认是最大堆，用 < 决定谁排前面
    bool operator<(const Task &other) const {
        return priority < other.priority;  // 优先级高的排前面
    }
};

priority_queue<Task> pq;
pq.push({3, "吃饭"});
pq.push({1, "睡觉"});
pq.push({5, "写代码"});
cout << pq.top().name;  // "写代码"（优先级5最高）
```

---

## 四、结构体放进 unordered_set / unordered_map

`unordered_set` 和 `unordered_map` 底层是哈希表，需要两样东西：

1. **哈希函数**：把你的 struct 变成一个数字
2. **相等判断**：判断两个元素是否相同

### 方法一：写自定义哈希（标准做法）

```cpp
struct Point {
    int x, y;

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

// 自定义哈希函数
struct PointHash {
    size_t operator()(const Point &p) const {
        // 把两个int组合成一个哈希值
        return hash<int>()(p.x) ^ (hash<int>()(p.y) << 16);
    }
};

unordered_set<Point, PointHash> visited;
visited.insert({1, 2});

unordered_map<Point, int, PointHash> dist;
dist[{1, 2}] = 5;
```

### 方法二：偷懒做法（竞赛常用）

把坐标编码成一个 long long 或 string，直接用现成的哈希：

```cpp
// 方法二a：编码成 long long
long long encode(int x, int y) {
    return (long long)x * 100001 + y;  // 假设坐标范围在10万以内
}

unordered_set<long long> visited;
visited.insert(encode(1, 2));

// 方法二b：编码成 string
string encode(int x, int y) {
    return to_string(x) + "," + to_string(y);
}

unordered_set<string> visited;
visited.insert(encode(1, 2));
```

**竞赛建议：** 如果只是坐标或简单的二元组，用编码法更快更省事。需要频繁使用 struct 做键时才写自定义哈希。

---

## 五、结构体与函数 —— 竞赛中的常见模式

### 模式一：函数返回结构体

```cpp
struct Result {
    int maxVal;
    int minVal;
    double avg;
};

Result analyze(vector<int> &nums) {
    int mx = *max_element(nums.begin(), nums.end());
    int mn = *min_element(nums.begin(), nums.end());
    double avg = accumulate(nums.begin(), nums.end(), 0.0) / nums.size();
    return {mx, mn, avg};  // 直接返回
}

int main() {
    vector<int> v = {3, 1, 4, 1, 5};
    Result r = analyze(v);
    // 或者用 auto
    auto r2 = analyze(v);
    cout << r.maxVal << " " << r.minVal << " " << r.avg << endl;
}
```

当一个函数需要返回多个值时，用 struct 包装比用引用参数更清晰。

### 模式二：结构体作为函数参数（传引用！）

```cpp
struct Graph {
    int n;
    vector<vector<int>> adj;
};

// ❌ 传值：每次调用都复制整个图，太慢
void bfs(Graph g, int start) { ... }

// ✅ 传引用：不复制，直接用原来的
void bfs(Graph &g, int start) { ... }

// ✅ 如果函数不修改图，加 const 更安全
void bfs(const Graph &g, int start) { ... }
```

**规则再强调一次：struct 作为参数，永远传引用。**

### 模式三：结构体数组 + 排序（最高频）

这是 LeetCode 和竞赛中最常见的结构体使用方式：

```cpp
// LeetCode 56：合并区间
struct Interval {
    int start, end;
};

vector<Interval> merge(vector<Interval> &intervals) {
    // 按起点排序
    sort(intervals.begin(), intervals.end(), [](Interval &a, Interval &b) {
        return a.start < b.start;
    });

    vector<Interval> result;
    for (auto &itv : intervals) {
        if (result.empty() || result.back().end < itv.start) {
            result.push_back(itv);  // 不重叠，直接加入
        } else {
            result.back().end = max(result.back().end, itv.end);  // 重叠，合并
        }
    }
    return result;
}
```

---

## 六、pair 和 tuple —— 轻量级替代品

有时候你只需要临时绑定两三个值，不值得专门定义一个 struct。

### pair：两个值绑在一起

```cpp
#include <utility>  // 其实 <algorithm> 和大多数头文件已经包含了

pair<int, int> p = {3, 4};
p.first;   // 3
p.second;  // 4

// 竞赛中最常见：存坐标
pair<int, int> pos = {0, 0};

// pair 自带比较：先比 first，first 相同比 second
vector<pair<int, int>> v = {{3,1}, {1,2}, {3,0}};
sort(v.begin(), v.end());
// 排序后：{1,2}, {3,0}, {3,1}

// BFS中常见：存坐标
queue<pair<int, int>> q;
q.push({0, 0});
auto [x, y] = q.front();  // C++17 结构化绑定，直接拆开
q.pop();
```

### tuple：三个及以上的值

```cpp
#include <tuple>

tuple<int, string, double> t = {1, "Alice", 95.5};
auto [id, name, score] = t;  // C++17 结构化绑定

// 或者用 get
get<0>(t);  // 1
get<1>(t);  // "Alice"
get<2>(t);  // 95.5
```

### pair vs struct 怎么选？

| 情况 | 选择 | 原因 |
|------|------|------|
| 只有两个值，临时用一下 | `pair` | 省事，自带比较 |
| 两个值但多处使用 | `struct` | `p.x, p.y` 比 `p.first, p.second` 更好读 |
| 三个及以上的值 | `struct` | tuple 的 `get<0>` 可读性太差 |
| 需要自定义比较规则 | `struct` | pair 的默认比较可能不是你要的 |

**竞赛经验：** 坐标用 `pair<int,int>`，其他用 struct。

---

## 七、结构体在经典算法中的应用

### 应用一：图论 —— 存边

```cpp
struct Edge {
    int from, to, weight;

    // 按权重排序（Kruskal最小生成树需要）
    bool operator<(const Edge &other) const {
        return weight < other.weight;
    }
};

vector<Edge> edges;
edges.push_back({0, 1, 5});
edges.push_back({1, 2, 3});
edges.push_back({0, 2, 8});

sort(edges.begin(), edges.end());  // 按权重从小到大
```

### 应用二：Dijkstra 最短路

```cpp
struct State {
    int node, dist;

    // 注意：priority_queue 默认最大堆
    // 我们要最小的 dist 先出来，所以反着写
    bool operator<(const State &other) const {
        return dist > other.dist;  // dist 小的优先级高
    }
};

void dijkstra(vector<vector<pair<int,int>>> &adj, int start, vector<int> &dist) {
    priority_queue<State> pq;
    pq.push({start, 0});
    dist[start] = 0;

    while (!pq.empty()) {
        auto [u, d] = pq.top();  // 取出距离最小的节点
        // 注意：这里不能用 auto &[u, d]，因为 top() 返回的是 const 引用
        // 然后我们马上 pop 了，引用就悬空了
        pq.pop();

        if (d > dist[u]) continue;  // 已经有更短路径，跳过

        for (auto &[v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({v, dist[v]});
            }
        }
    }
}
```

### 应用三：贪心 —— 活动选择 / 区间调度

```cpp
struct Activity {
    int start, end;
};

int maxActivities(vector<Activity> &acts) {
    // 按结束时间排序
    sort(acts.begin(), acts.end(), [](Activity &a, Activity &b) {
        return a.end < b.end;
    });

    int count = 1;
    int lastEnd = acts[0].end;

    for (int i = 1; i < acts.size(); i++) {
        if (acts[i].start >= lastEnd) {
            count++;
            lastEnd = acts[i].end;
        }
    }
    return count;
}
```

### 应用四：BFS —— 存状态

```cpp
struct State {
    int x, y, steps;
};

int bfs(vector<vector<int>> &grid) {
    int n = grid.size(), m = grid[0].size();
    queue<State> q;
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    q.push({0, 0, 0});
    visited[0][0] = true;

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!q.empty()) {
        auto [x, y, steps] = q.front();
        q.pop();

        if (x == n-1 && y == m-1) return steps;  // 到达终点

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m
                && !visited[nx][ny] && grid[nx][ny] == 0) {
                visited[nx][ny] = true;
                q.push({nx, ny, steps + 1});
            }
        }
    }
    return -1;  // 到不了
}
```

---

## 八、链表和树 —— 结构体的自引用

这是你之前没接触过的重要用法：**结构体内部包含指向自身类型的指针。**

### 链表节点

```cpp
struct ListNode {
    int val;
    ListNode *next;  // 指向下一个节点的指针

    ListNode(int val) : val(val), next(nullptr) {}
    // nullptr 就是"空"，表示没有下一个节点
};

// 创建链表 1 -> 2 -> 3
ListNode *head = new ListNode(1);
head->next = new ListNode(2);
head->next->next = new ListNode(3);

// 遍历链表
ListNode *cur = head;
while (cur != nullptr) {
    cout << cur->val << " ";
    cur = cur->next;
}
// 输出：1 2 3
```

**为什么用指针 `ListNode *next` 而不是 `ListNode next`？**

如果直接放一个 `ListNode next`，那这个 ListNode 里面又有 next，里面又有 next... 无限嵌套，内存直接爆炸。

用指针只存一个地址（8字节），不存实际对象，所以不会无限嵌套。

**类比：** 联系人列表里，每个人的信息卡上写着"下一个人的门牌号"，而不是直接把下一个人塞进自己的卡片里。

### 二叉树节点

```cpp
struct TreeNode {
    int val;
    TreeNode *left;   // 左子节点
    TreeNode *right;  // 右子节点

    TreeNode(int val) : val(val), left(nullptr), right(nullptr) {}
};

// 创建一棵树
//     1
//    / \
//   2   3
TreeNode *root = new TreeNode(1);
root->left = new TreeNode(2);
root->right = new TreeNode(3);
```

### LeetCode 中的链表和树

LeetCode 的链表题和树题，题目都会帮你定义好节点结构：

```cpp
// LeetCode 提供的定义（你不需要自己写）
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

你只需要知道怎么使用：

```cpp
// 反转链表（LeetCode 206）
ListNode* reverseList(ListNode* head) {
    ListNode *prev = nullptr, *cur = head;
    while (cur) {
        ListNode *next = cur->next;  // 先存下一个
        cur->next = prev;             // 反转指向
        prev = cur;                    // prev前进
        cur = next;                    // cur前进
    }
    return prev;
}

// 二叉树前序遍历（LeetCode 144）
void preorder(TreeNode *root, vector<int> &result) {
    if (root == nullptr) return;  // 空节点直接返回
    result.push_back(root->val);  // 先访问根
    preorder(root->left, result);  // 再访问左子树
    preorder(root->right, result); // 最后访问右子树
}
```

---

## 九、竞赛实战模板

### 模板一：坐标点

```cpp
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Point &o) const { return x == o.x && y == o.y; }
    bool operator<(const Point &o) const {
        return x < o.x || (x == o.x && y < o.y);
    }
};
```

### 模板二：带权边

```cpp
struct Edge {
    int u, v, w;  // 起点、终点、权重
    bool operator<(const Edge &o) const { return w < o.w; }
};
```

### 模板三：Dijkstra 状态

```cpp
struct State {
    int node, dist;
    bool operator<(const State &o) const { return dist > o.dist; }
    // 注意 > 不是 <，因为 priority_queue 默认最大堆
};
```

### 模板四：多关键字排序

```cpp
struct Item {
    int a, b, c;

    bool operator<(const Item &o) const {
        if (a != o.a) return a < o.a;      // 第一关键字升序
        if (b != o.b) return b > o.b;      // 第二关键字降序
        return c < o.c;                     // 第三关键字升序
    }
};
```

---

## 十、常见错误和注意事项

### 错误一：struct 定义后忘记分号

```cpp
struct Point {
    int x, y;
}    // ❌ 漏了分号，编译错误一大堆但提示很迷惑

struct Point {
    int x, y;
};   // ✅
```

### 错误二：在 struct 内用 `<` 但方向搞反

```cpp
// priority_queue 默认最大堆
// 如果你想让小的先出来（最小堆），要反着写 <

// ❌ 这样写，大的先出来
bool operator<(const State &o) const { return dist < o.dist; }

// ✅ 这样写，小的先出来
bool operator<(const State &o) const { return dist > o.dist; }
```

记忆技巧：priority_queue 的 `<` 是"谁输谁在上面"。`return dist > o.dist` 意思是"我的dist大我就输"，所以 dist 小的赢了，排在上面先出来。

### 错误三：忘记 const

```cpp
// ❌ 少了 const，放进 set 会编译报错
bool operator<(Point &other) {
    return x < other.x;
}

// ✅ 两个 const 都要
bool operator<(const Point &other) const {
    return x < other.x;
}
```

### 错误四：浮点数比较用 !=

```cpp
// ❌ 浮点数不能直接用 != 比较
if (a.score != b.score) return a.score > b.score;

// ✅ 用差值的绝对值判断
if (abs(a.score - b.score) > 1e-9) return a.score > b.score;
```

不过竞赛中分数一般都是整数，这个问题遇到的不多。

---

## 十一、总结：你需要掌握的优先级

```
立刻掌握（LeetCode 刷题必备）：
  ✅ struct 基本定义和使用
  ✅ 构造函数（包括默认参数写法）
  ✅ 结合 vector + sort + lambda
  ✅ pair 的使用
  ✅ 链表节点和树节点的结构（-> 访问）

接下来掌握（中等题和竞赛需要）：
  ✅ operator< 重载（用于 set/map/priority_queue）
  ✅ 多关键字排序
  ✅ BFS/DFS 中的状态结构体
  ✅ 图论中的边结构体

了解即可（高级竞赛题可能用到）：
  ⏳ 自定义哈希（unordered_set/unordered_map）
  ⏳ tuple 的使用
```

---

## 附录：练习题推荐

| 题目 | 结构体用法 | 难度 |
|------|-----------|------|
| LeetCode 206 反转链表 | ListNode, 指针操作 | 简单 |
| LeetCode 21 合并两个有序链表 | ListNode, 指针操作 | 简单 |
| LeetCode 144 二叉树前序遍历 | TreeNode, 递归 | 简单 |
| LeetCode 56 合并区间 | struct + sort | 中等 |
| LeetCode 253 会议室 II | struct + priority_queue | 中等 |
| LeetCode 973 最接近原点的K个点 | struct/pair + sort | 中等 |
| LeetCode 743 网络延迟时间 | Edge struct + Dijkstra | 中等 |
| Codeforces 排序题 | 多关键字 struct 排序 | 入门 |
