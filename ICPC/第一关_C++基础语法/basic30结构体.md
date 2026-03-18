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

想象一下，在竞赛中要创建 1000 个点，每个都这样赋值一遍——手都要酸了。更重要的是，这样容易忘赋某个成员，导致数据未初始化。

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

构造函数的作用就是——在对象创建的时候自动帮你初始化成员变量。三种创建方式都是一样的效果，只是写法不同。在竞赛中，你会发现 `Point p(3, 4)` 是最常用的。

> ✅ 构造函数是让你代码更简洁、更安全的利器！

### `: x(x), y(y)` 是什么？

这叫**初始化列表**，是构造函数专用的语法，意思是"用参数 x 的值初始化成员变量 x"。为什么不直接在函数体里赋值呢？因为初始化列表在成员变量创建的同时就给了它们值，更高效；而在函数体里赋值是先创建后修改，多了一步。

等价于：

```cpp
Point(int x, int y) {
    this->x = x;   // this->x 是成员变量，x 是参数
    this->y = y;
}
```

初始化列表写法更简洁，竞赛中几乎都用这种。

> 💡 **老师提醒：** 如果参数名和成员变量名一样（都叫 `x`），会有覆盖问题。初始化列表里就没问题（`: x(x)` 明确了前一个是成员，后一个是参数）。如果在函数体里，`x = x` 就变成自己给自己赋值，没有效果。

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

因为如果你写了有参构造，C++ 就不再自动提供无参构造了。而 `vector<Point> v(10)` 需要无参构造来初始化 10 个默认元素。所以只要你自己写了构造函数，就记得也写个无参构造，否则会卡壳。

```cpp
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

vector<Point> v(10);  // ❌ 编译错误！没有无参构造函数
```

> 💡 **老师提醒：** 很多初学者写构造函数时忘了加无参版本，导致 `vector<struct>` 报错。解决办法有两个——要么加无参构造，要么给参数加默认值：

```cpp
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    // 这样 Point()、Point(3,4) 都能用，最方便
};

vector<Point> v(10);  // ✅ 10个(0,0)
```

这个"默认参数"的做法是竞赛中最常用的，记住这一招！

> ✅ 构造函数搞定——你就能轻松创建对象了！

---

## 三、运算符重载 —— 让 struct 支持比较和运算

### 问题：想用 sort 但不想每次都写 lambda

```cpp
// 每次排序都要写这么长的lambda
sort(v.begin(), v.end(), [](Student &a, Student &b) {
    return a.score > b.score;
});
```

虽然 lambda 能解决问题，但如果同一个 struct 多个地方都用 sort，就得重复写 lambda——又啰嗦又容易出错（万一两处的排序规则不一样呢？）。

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

**直觉理解：** 你在告诉 C++："当你需要比较两个 Student 谁小谁大的时候，就用这个规则。" 一旦你定义了 `operator<`，`sort` 就会自动调用它，不用你每次都写判断逻辑。

> 💡 **老师提醒：** 最后那个 `const` 千万别忘——如果少了，放进 `set` 或 `map` 时编译会报错，提示信息很迷惑。建议养成习惯，记住模板：`bool operator<(const T &other) const`

> ✅ 运算符重载不复杂——就是告诉编译器怎么排序！

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

`set` 和 `map` 内部是有序的，它需要知道怎么比较你的元素。没有比较规则，编译器就不知道该怎么排序，所以会报错。就像你要把书排成一列，但不知道按字数、按厚度还是按颜色排，就很困惑。给了 `operator<`，编译器就有规则了。

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

> 💡 **老师提醒：** priority_queue 的排序有点反直觉——如果你写 `return priority < other.priority`，反而是优先级高的先出来。这是因为 priority_queue 默认是最大堆，`a < b` 则 a 优先级更低。想清楚这点，别写反了。

> ✅ 学会运算符重载，你就能轻松处理各种有序场景！

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
    return {mx, mn, avg};  // 直接返回，会调用构造函数初始化
}

int main() {
    vector<int> v = {3, 1, 4, 1, 5};
    Result r = analyze(v);
    // 或者用 auto
    auto r2 = analyze(v);
    cout << r.maxVal << " " << r.minVal << " " << r.avg << endl;
}
```

当一个函数需要返回多个值时，用 struct 包装比用引用参数更清晰。*就像一个快递员一次送多个包裹，用一个大箱子打包比分多趟送更高效*。这样调用方也能清楚地看出来"这个函数返回一个结果"。

**这段代码逐行在做什么（按执行顺序看）：**

1. `struct Result` 定义了一个返回结果的"盒子"，里面有三个成员：最大值 `maxVal`、最小值 `minVal`、平均值 `avg`。
2. `Result analyze(vector<int> &nums)` 表示函数接收一个整型数组（引用传参，不拷贝原数组），并且返回一个 `Result`。
3. `max_element(nums.begin(), nums.end())` 找到最大元素的位置（迭代器），前面的 `*` 把位置解引用成真正的数值，所以得到 `mx`。
4. `min_element(...)` 同理，得到最小值 `mn`。
5. `accumulate(nums.begin(), nums.end(), 0.0)` 计算总和。这里写 `0.0` 很关键，表示按 `double` 参与运算，避免平均值被整数除法截断。
6. `... / nums.size()` 用总和除以元素个数，得到平均值 `avg`。
7. `return {mx, mn, avg};` 把三个值一次性打包成 `Result` 返回。
8. 在 `main` 里，`Result r = analyze(v);` 和 `auto r2 = analyze(v);` 都能接收返回值，后者只是让编译器自动推导类型。

**代入样例 `v = {3, 1, 4, 1, 5}`：**

- 最大值：`5`
- 最小值：`1`
- 总和：`14`
- 平均值：`14.0 / 5 = 2.8`

所以输出就是：`5 1 2.8`。

> 💡 **老师提醒：** 这段代码默认 `nums` 非空。若数组为空，`max_element` / `min_element` 解引用会出错，`nums.size()` 还会出现除以 0 的风险。竞赛里如果输入可能为空，要先 `if (nums.empty())` 做保护。

> ✅ 你现在看到的其实是一个非常经典的模式："扫描一次数组，统计多个指标，然后用 struct 一次返回"。后面做题会经常遇到。

> 💡 **老师提醒：** 返回 struct 时，C++ 会优化（RVO / Copy Elision），不会真的复制整个对象，所以不必担心性能。

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

**为什么要传引用？** 因为 struct 可能很大（比如包含一个邻接表）。如果不传引用，每次函数调用都得复制整个 struct，就像每次寄快递都要复印一份合同——浪费时间和空间。传引用相当于"只把地址告诉函数"，函数直接操作原数据，快得多。

> 💡 **老师提醒：** struct 作为参数永远传引用，这是新手最容易忘的规则。即使编译过了，性能也会很差。

**规则再强调一次：struct 作为参数，永远传引用。**

> ✅ 参数传引用，今后竞赛代码提速 10 倍！

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

有时候你只需要临时绑定两三个值，不值得专门定义一个 struct。这时候 pair 和 tuple 就很方便——它们是预定义好的"快速结构体"。

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

**难道 pair 自己就能比较？** 是的。pair 本身就重载了 `operator<`——按 first 从小到大排，first 相同就按 second 排。所以坐标用 pair 时根本不用再写 lambda，直接 `sort` 就行。这是 pair 的一大优势。

> ✅ 因为 pair 帮你省了 operator< 的代码——很方便！

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

tuple 能装任意数量的变量，比 pair 更灵活。但 `get<0>`、`get<1>` 这种写法可读性差（数字多了就容易搞混）。通常你还是用结构化绑定 `auto [a,b,c]` 更清楚。

### pair vs struct 怎么选？

| 情况 | 选择 | 原因 |
|------|------|------|
| 只有两个值，临时用一下 | `pair` | 省事，自带比较，不用写 operator< |
| 两个值但多处使用 | `struct` | `p.x, p.y` 比 `p.first, p.second` 更好读 |
| 三个及以上的值 | `struct` | tuple 的 `get<0>` 可读性太差，struct 更清楚 |
| 需要自定义比较规则 | `struct` | pair 的默认比较可能不是你要的 |

**竞赛经验：** 坐标用 `pair<int,int>`，其他用 struct。如果两个值起了有意义的名字（比如起点和终点、距离和节点号），最好定义 struct 来体现这个含义。

> 💡 **老师提醒：** tuple 虽然强大，但竞赛中用得特别少。掌握 pair 和 struct 就够用了。

> ✅ 轻量级结构体的选择一目了然——用对了能省好多代码！

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

这是你之前没接触过的重要用法：**结构体内部包含指向自身类型的指针。** 这听起来很奇怪——一个东西里面还有它自己？但这正是构建链表和树的方式。

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

如果直接放一个 `ListNode next`，那这个 ListNode 里面又有 next，里面又有 next... 无限嵌套，内存直接爆炸。用指针只存一个地址（8字节），不存实际对象，所以不会无限嵌套。

**类比：** *就像一个房间里不能再放大小一样的房间，但可以贴一张"下一个房间地址"的纸条*——地址很小（1 张纸），实际的房间在别的地方。

> 💡 **老师提醒：** 很多初学者这里会卡壳，觉得"自己包含自己"无法理解。关键是用了指针——指针只是个地址，不是实际对象，所以不会无限递归。

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

> ✅ 链表和树虽然看起来复杂，但核心就是指针链接——掌握这个思想就不怕它们了！

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

这个错误很常见，因为在函数里漏分号编译器会直接说"缺少分号"。但在 struct 定义里漏分号，编译器的错误提示往往出现在下一行，让你找半天。**记住：struct 定义的 `}` 后面一定要加分号。**

> 💡 **老师提醒：** 作为防守手段，可以在写完 struct 后立即打分号，再写内容。这样就不会忘。

### 错误二：在 struct 内用 `<` 但方向搞反

```cpp
// priority_queue 默认最大堆
// 如果你想让小的先出来（最小堆），要反着写 <

// ❌ 这样写，大的先出来
bool operator<(const State &o) const { return dist < o.dist; }

// ✅ 这样写，小的先出来
bool operator<(const State &o) const { return dist > o.dist; }
```

记忆技巧：priority_queue 的 `<` 是"谁输谁在上面"。`return dist > o.dist` 意思是"我的dist大我就输"，所以 dist 小的赢了，排在上面先出来。如果你记不清，可以这样想：**大的想抢位置，结果比较时它大了所以输了，被压在下面**。

> 💡 **老师提醒：** 这个坑踩过一次基本就记住了。竞赛时如果 priority_queue 出现异常，99% 就是这里搞反了。

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

这个错误特别坑——你的代码逻辑没问题，但编译器会说"没有匹配的比较函数"。为什么呢？因为 `set` 的比较要求是 `const` 的（保证不修改元素）。如果你的 `operator<` 没有 `const`，就不符合要求。

> 💡 **老师提醒：** 这是坑中坑。编译器的提示信息特别长和复杂，容易让初学者以为是别的问题。记住模板就行：`bool operator<(const T &other) const { ... }`

### 错误四：浮点数比较用 !=

```cpp
// ❌ 浮点数不能直接用 != 比较（精度问题）
if (a.score != b.score) return a.score > b.score;

// ✅ 用差值的绝对值判断
if (abs(a.score - b.score) > 1e-9) return a.score > b.score;
```

浮点数在计算过程中会有微小的精度误差，两个"应该相等"的浮点数比较时可能因为精度问题判断错误。不过竞赛中分数一般都是整数，这个问题遇到的不多，了解一下即可。

> ✅ struct 定义时注意这几个坑，就能少掉很多莫名其妙的编译错误！

---

## 十一、总结：你需要掌握的优先级

学 struct 也是有"优先级"的——先掌握高优先级的，才能高效地刷题。要知道的是：你不需要一开始就学会所有的 struct 用法，只需要按照优先级来学——马上能用上的先学，罕见的用法可以以后遇到再查。

```
立刻掌握（LeetCode 刷题必备）：
  ✅ struct 基本定义和使用
     → 这是基础中的基础，一切都从这儿开始
  
  ✅ 构造函数（包括默认参数写法）
     → 节省代码，让初始化更安全，竞赛中很常用
  
  ✅ 结合 vector + sort + lambda
     → 日常排序的绝对必备，掌握这套组合技能，90% 的排序问题都能解决
  
  ✅ pair 的使用
     → 两个值的临时绑定，自带排序，极其方便
  
  ✅ 链表节点和树节点的结构（-> 访问）
     → LeetCode 经典题型，指针操作虽然一开始看着虚，但很快就会习惯

接下来掌握（中等题和竞赛需要）：
  ✅ operator< 重载（用于 set/map/priority_queue）
     → 这个是"进阶必修课"，搞懂它你就能处理有序数据结构
  
  ✅ 多关键字排序
     → 实战中很常见——按优先级排序、按距离排序等
  
  ✅ BFS/DFS 中的状态结构体
     → 图论题的灵魂，每个状态都需要清楚的结构定义
  
  ✅ 图论中的边结构体
     → Dijkstra、Kruskal 等算法都需要这个

了解即可（高级竞赛题可能用到）：
  ⏳ 自定义哈希（unordered_set/unordered_map）
     → 理解原理即可，实战中偷懒法（编码成数字）用得更多
  
  ⏳ tuple 的使用
     → 强大但不常用，了解存在即可，遇到再用
```

> 💡 **老师提醒：** 不要想一次全部掌握，那样反而会学得很乱。按优先级来，每个阶段专注 3-4 个要素，等真正用熟了再往下学。这样学得快也学得牢。

> ✅ Struct 核心知识掌握了——你现在已经有了处理复杂数据结构的足够底气！

---

## 附录：精选练习题

下面这些题目按难度排列，建议按这个顺序来做——从简单题开始找到感觉，再逐步增加难度。每一题都体现了 struct 的某个重要用法。

| 题目 | 结构体用法 | 学到什么 | 难度 |
|------|-----------|--------|------|
| LeetCode 206 反转链表 | ListNode, 指针操作 | 指针遍历、节点链接 | 简单 |
| LeetCode 21 合并两个有序链表 | ListNode, 指针比较 | 双指针合并 | 简单 |
| LeetCode 144 二叉树前序遍历 | TreeNode, 递归 | 树的递归访问 | 简单 |
| LeetCode 56 合并区间 | struct + sort | 多关键字排序的应用 | 中等 |
| LeetCode 253 会议室 II | struct + priority_queue | priority_queue 的实战用法 | 中等 |
| LeetCode 973 最接近原点的 K 个点 | struct/pair + sort | 自定义排序规则 | 中等 |
| LeetCode 743 网络延迟时间 | Edge struct + Dijkstra | 图论结构体 | 中等 |

**推荐做题策略：**

1. **第一阶段（简单题）**: 完成 206、21、144，熟悉基本的 struct 定义和指针操作。这三题都是"热身"，目的是让 ListNode 和 TreeNode 不再陌生。
   
2. **第二阶段（中等题）**: 做 56、253、973，学会自定义排序和优先队列的结合——这是从"能用"到"善用"的转变。
   
3. **第三阶段（高级应用）**: 做 743 和一些 Codeforces 初级排序题，在实战中综合应用你学到的所有知识。

> 💡 **老师提醒：** 不要看完资料就直接跳到中等题——很可能会被卡住。循序渐进很重要。每做一题，就把你学到的新用法整理成"自己的模板"，积累模板库真的很关键。

> ✅ 现在就去 LeetCode 上练练手吧——很快你就会发现这些知识有多实用！
