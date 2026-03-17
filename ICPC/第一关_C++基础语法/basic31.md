# 从 C++ 基础语法到数据结构：完整衔接指南

> **你的困惑**：学完基础语法后，看数据结构代码时经常"认不出来"——变量换了个名字就不认识了，不知道某个东西有哪些操作可以用，多个概念组合在一起就混乱。
>
> **本文目标**：帮你建立一套"看到任何 C++ 代码都能读懂"的思维框架。

---

## 第一部分：为什么你会"认不出来"

你目前的状态是：每个语法点单独看都懂，但组合在一起就乱了。

根本原因是你缺少一个**统一的思维模型**。就像学英语，你认识每个单词，但看到一个长句子就读不懂——因为你还没掌握"句型结构"。

C++ 代码也有"句型结构"。掌握了它，不管变量叫什么名字、用什么类型，你都能一眼看透。

---

## 第二部分：C++ 代码的万能阅读法

### 2.1 任何一行代码，都在回答三个问题

```
1. 这个东西是什么类型？（int？vector？指针？自定义结构体？）
2. 这个类型有哪些操作？（能用什么方法？能做什么运算？）
3. 这行代码在干什么？（创建？修改？读取？调用？）
```

**练习**：看这行代码：

```cpp
vector<int> dist(n, INT_MAX);
```

三个问题的回答：

```
1. dist 是什么类型？→ vector<int>，一个存 int 的动态数组
2. vector<int> 有哪些操作？→ push_back, pop_back, size, [], 等等
3. 这行在干什么？→ 创建一个大小为 n 的数组，每个元素初始化为 INT_MAX
```

再看一行：

```cpp
pq.push({0, src});
```

```
1. pq 是什么类型？→ 往上找定义，发现是 priority_queue<...>
2. priority_queue 有哪些操作？→ push, pop, top, size, empty
3. 这行在干什么？→ 往优先队列里放入一个东西
```

**这就是你阅读任何代码的万能方法：先认类型，再查操作，最后理解意图。**

---

### 2.2 "变量换了名字就不认识"的解药

问题不在名字，在于你没有养成"看类型"的习惯。

```cpp
// 这三行做的事情完全一样，只是变量名不同
vector<int> nums(10, 0);
vector<int> dist(10, 0);
vector<int> dp(10, 0);
```

名字是给人看的（帮你理解含义），类型才决定能做什么操作。

**规则：看到任何变量，第一反应是找它的类型，而不是纠结它叫什么。**

怎么找类型？往上翻，找到它被声明的那一行：

```cpp
// 情况1：直接声明
vector<int> adj;          // 类型：vector<int>
int n;                    // 类型：int
TreeNode* root;           // 类型：TreeNode*（指向 TreeNode 的指针）

// 情况2：用 auto，看右边推断
auto it = mp.find(key);   // it 的类型？看 mp.find() 返回什么
auto [x, y] = q.front();  // x 和 y 的类型？看 q 里存的是什么

// 情况3：函数参数
void dfs(vector<vector<int>>& graph, int node, vector<bool>& visited)
//       ↑ graph 的类型              ↑ node     ↑ visited 的类型
```

---

### 2.3 类型决定了"有哪些操作"

这是最关键的一点。你不需要死记每个变量能干什么，只需要记住**每种类型**能干什么。

变量名千变万化，但类型就那么几种。下面我把你会遇到的所有类型和它们的操作整理成一张完整的表。

---

## 第三部分：你需要记住的所有类型及其操作

### 3.1 基本类型

```cpp
int a = 10;
long long b = 1e18;
double c = 3.14;
bool d = true;
char e = 'A';
```

基本类型的操作就是算术运算（+、-、*、/、%）和比较（==、!=、<、>）。没有方法可以调用，不能用 `.` 操作。

### 3.2 string（字符串）

```cpp
string s = "hello";

// 查
s.size()           // 长度：5（和 s.length() 完全一样）
s[0]               // 第一个字符：'h'
s.find("llo")      // 查找子串位置：2
s.substr(1, 3)     // 截取子串："ell"（从索引1开始，长3个）
s.empty()          // 是否为空：false

// 改
s[0] = 'H'         // 修改某个字符
s += " world"      // 拼接
s.push_back('!')   // 尾部加一个字符

// 遍历
for (char c : s) { ... }           // 遍历每个字符
for (int i = 0; i < s.size(); i++) // 用索引遍历
```

### 3.3 vector（动态数组）—— 最常用！

```cpp
vector<int> v;              // 空数组
vector<int> v(n);           // 大小为n，全为0
vector<int> v(n, val);      // 大小为n，全为val
vector<int> v = {1,2,3};   // 直接初始化

// 查
v.size()           // 元素个数
v.empty()          // 是否为空
v[i]               // 第i个元素（不检查越界）
v.front()          // 第一个元素
v.back()           // 最后一个元素

// 增
v.push_back(x)     // 尾部添加
v.insert(v.begin()+i, x)  // 在索引i处插入（慢，O(n)）

// 删
v.pop_back()       // 删除最后一个
v.erase(v.begin()+i)      // 删除索引i处（慢，O(n)）
v.clear()          // 清空

// 遍历
for (int x : v) { ... }            // 只读遍历
for (int& x : v) { ... }           // 可修改遍历
for (int i = 0; i < v.size(); i++) // 索引遍历
```

**二维 vector（矩阵/邻接表）：**

```cpp
// 创建 n×m 的矩阵，全为0
vector<vector<int>> grid(n, vector<int>(m, 0));
grid[i][j] = 5;   // 访问第i行第j列

// 邻接表（图的标准表示）
vector<vector<int>> graph(n);      // n个节点，每个节点有一个邻居列表
graph[0].push_back(1);             // 节点0 → 节点1
graph[0].push_back(2);             // 节点0 → 节点2

// 遍历节点0的所有邻居
for (int neighbor : graph[0]) {
    cout << neighbor;
}
```

### 3.4 stack（栈）

```cpp
stack<int> s;

s.push(x)     // 入栈
s.pop()        // 弹出栈顶（不返回值！）
s.top()        // 看栈顶（不弹出）
s.size()       // 大小
s.empty()      // 是否为空

// ⚠️ 没有 s[i]！不能随机访问
// ⚠️ pop() 不返回值！要先 top() 再 pop()
```

### 3.5 queue（队列）

```cpp
queue<int> q;

q.push(x)      // 入队（队尾）
q.pop()         // 出队（队头，不返回值！）
q.front()       // 看队头（不弹出）
q.back()        // 看队尾
q.size()        // 大小
q.empty()       // 是否为空

// ⚠️ 没有 q[i]！不能随机访问
// ⚠️ pop() 不返回值！要先 front() 再 pop()
```

### 3.6 priority_queue（优先队列/堆）

```cpp
// 大顶堆（默认，最大的在顶部）
priority_queue<int> maxPQ;

// 小顶堆（最小的在顶部）——这个声明比较长，背下来
priority_queue<int, vector<int>, greater<int>> minPQ;

maxPQ.push(x)    // 入堆
maxPQ.pop()       // 弹出堆顶（不返回值！）
maxPQ.top()       // 看堆顶
maxPQ.size()      // 大小
maxPQ.empty()     // 是否为空

// ⚠️ 注意：用的是 top()，不是 front()！
// ⚠️ pop() 不返回值！要先 top() 再 pop()
```

### 3.7 unordered_map（哈希表）

```cpp
unordered_map<string, int> mp;

// 增/改
mp["apple"] = 3;           // 插入或更新
mp.insert({"banana", 5});  // 插入（如果key已存在则不覆盖）

// 查
mp["apple"]                // 取值（⚠️ key不存在会自动创建！）
mp.count("apple")          // key是否存在（返回0或1）
mp.find("apple")           // 返回迭代器（找不到返回 mp.end()）
mp.size()                  // 键值对数量
mp.empty()                 // 是否为空

// 删
mp.erase("apple")          // 删除

// 遍历
for (auto& [key, val] : mp) {    // C++17 结构化绑定
    cout << key << " " << val;
}
// 或者
for (auto& p : mp) {
    cout << p.first << " " << p.second;
}
```

### 3.8 unordered_set（哈希集合）

```cpp
unordered_set<int> s;

s.insert(x)       // 插入
s.erase(x)        // 删除
s.count(x)        // 是否存在（返回0或1）
s.size()          // 大小
s.empty()         // 是否为空

// 遍历
for (int x : s) { ... }
```

### 3.9 map 和 set（有序版本）

```cpp
map<int, string> m;     // 按key自动排序
set<int> s;             // 自动排序+去重

// 操作和 unordered_map/unordered_set 基本一样，额外多了：
m.begin()->first        // 最小的key
m.rbegin()->first       // 最大的key
m.lower_bound(x)        // ≥ x 的最小key的迭代器
m.upper_bound(x)        // > x 的最小key的迭代器
```

### 3.10 pair（二元组）

```cpp
pair<int, int> p = {3, 4};
p.first    // 3
p.second   // 4

// 在 map 遍历中自动出现
for (auto& [key, val] : mp) { ... }   // key 和 val 就是 pair 的 first 和 second

// 常用于 priority_queue
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
pq.push({dist, node});
auto [d, n] = pq.top();   // d = first, n = second
```

---

## 第四部分：最容易混淆的操作对比

### 4.1 "看顶部元素"的方法名不统一

```
stack      → .top()
queue      → .front()
priority_queue → .top()
vector     → .front() 或 .back() 或 [0]
```

**记忆法：stack 和 priority_queue 用 top（因为"栈顶""堆顶"），queue 用 front（因为"队头"）。**

### 4.2 pop() 都不返回值！

C++ 的 stack、queue、priority_queue 的 pop() 都**不返回值**，只是删除。

```cpp
// 所有容器的"取出"操作都是两步：
// 第一步：看一眼（不删除）
int val = s.top();    // stack
int val = q.front();  // queue
int val = pq.top();   // priority_queue

// 第二步：删掉
s.pop();
q.pop();
pq.pop();
```

### 4.3 "判断元素是否存在"的不同写法

```cpp
// vector：没有直接的方法，要用 find 算法
if (find(v.begin(), v.end(), x) != v.end()) { ... }

// unordered_set / set：用 count
if (s.count(x)) { ... }     // 返回0或1，0就是false，1就是true

// unordered_map / map：用 count 或 find
if (mp.count(key)) { ... }
if (mp.find(key) != mp.end()) { ... }  // 等价写法
```

### 4.4 "大小"都叫 .size()

所有容器的大小都用 `.size()`，这是统一的，不会搞混。

### 4.5 "是否为空"都叫 .empty()

所有容器判断空都用 `.empty()`，返回 true/false。

---

## 第五部分：嵌套类型——你最大的困惑来源

### 5.1 为什么会嵌套？

数据结构经常需要"容器套容器"：

```cpp
vector<vector<int>> graph;           // 邻接表：vector 里面套 vector
vector<pair<int,int>> edges;         // 边列表：vector 里面套 pair
unordered_map<int, vector<int>> mp;  // map 的 value 是 vector
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
```

看起来很吓人，但拆开看很简单。

### 5.2 拆解嵌套类型的方法

**从外往里读**，一层一层拆：

```cpp
vector<vector<int>> graph;
```

```
第一层：vector<???>     → 这是一个动态数组
第二层：???  = vector<int>  → 数组里每个元素也是一个动态数组（存int的）

所以 graph 是"数组的数组"，也就是二维数组
graph[0]         → 第一个内层数组（vector<int>类型）
graph[0][1]      → 第一个内层数组的第二个元素（int类型）
graph[0].size()  → 第一个内层数组有多少个元素
graph[0].push_back(5)  → 往第一个内层数组尾部加入5
```

再来一个：

```cpp
unordered_map<string, vector<int>> mp;
```

```
第一层：unordered_map<string, ???>  → 哈希表，key是string
第二层：??? = vector<int>           → value是int数组

mp["hello"]              → 得到一个 vector<int>
mp["hello"].push_back(3) → 往那个数组里加入3
mp["hello"].size()       → 那个数组有多少个元素
mp["hello"][0]           → 那个数组的第一个元素
```

### 5.3 结构体配合容器

```cpp
struct Edge {
    int to, weight;
};

vector<vector<Edge>> graph(n);
```

```
graph              → vector<vector<Edge>>，二维的，外层是节点编号
graph[0]           → vector<Edge>，节点0的所有出边
graph[0][1]        → Edge类型，节点0的第二条出边
graph[0][1].to     → 那条边指向哪个节点
graph[0][1].weight → 那条边的权重

// 遍历节点u的所有邻居
for (auto& e : graph[u]) {
    // e 的类型是 Edge
    // e.to 是邻居节点
    // e.weight 是边权重
}

// 或者用结构化绑定（C++17）
for (auto& [to, w] : graph[u]) {
    // to 就是 e.to
    // w 就是 e.weight
}
```

### 5.4 priority_queue 放结构体

```cpp
struct State {
    int node, dist;
    bool operator<(const State& other) const {
        return dist > other.dist;  // 注意反着写 → 小顶堆
    }
};

priority_queue<State> pq;
pq.push({0, 0});           // 放入一个 State
auto cur = pq.top();       // cur 的类型是 State
cur.node                   // 节点编号
cur.dist                   // 距离
pq.pop();
```

---

## 第六部分：`auto&`、`const`、`&` 的完整指南

这些修饰符在数据结构代码中到处都是，必须搞清楚。

### 6.1 `auto` —— 让编译器帮你猜类型

```cpp
auto x = 10;                    // x 是 int
auto s = "hello"s;              // s 是 string
auto it = mp.find("key");       // it 是迭代器，类型很长，auto帮你省了
auto [key, val] = *it;          // 结构化绑定，拆开 pair
```

**什么时候用 auto？** 类型太长不想写的时候。比如迭代器类型、pair、tuple 等。

### 6.2 `&`（引用）—— 不复制，直接操作原数据

```cpp
// 不加 &：复制一份（慢，而且修改不影响原数据）
for (auto p : mp) {
    p.second = 999;    // 修改的是副本，mp 里的值不变！
}

// 加 &：直接操作原数据（快，修改会影响原数据）
for (auto& p : mp) {
    p.second = 999;    // 直接修改了 mp 里的值！
}

// 函数参数同理
void foo(vector<int> v) { ... }    // 复制整个数组进来（慢）
void foo(vector<int>& v) { ... }   // 直接操作原数组（快）
```

**竞赛规则：遍历容器和传参时，养成加 `&` 的习惯。**

### 6.3 `const` —— 承诺不修改

```cpp
// const 加在参数前：不会修改传入的数据
void foo(const vector<int>& v) {
    v[0] = 1;   // ❌ 编译报错！const 不让改
    cout << v[0]; // ✅ 只读可以
}

// const 加在函数后：不会修改类的成员变量
int size() const { return sz; }

// 遍历时 const auto& 表示"只读不改"
for (const auto& p : mp) {
    cout << p.first;    // ✅ 读可以
    p.second = 1;       // ❌ 不能改
}
```

### 6.4 什么时候用哪个？

```
要修改数据？      → auto&
只读不改？        → const auto&（更安全）
不确定？          → 先用 auto&，编译器会在你该用 const 的时候报警
竞赛中赶时间？    → 统一用 auto&，不出问题就行
```

---

## 第七部分：指针操作的完整梳理

### 7.1 什么时候会遇到指针？

数据结构代码中，指针主要出现在三个地方：

```
1. 链表节点：ListNode* head
2. 二叉树节点：TreeNode* root
3. new 出来的对象：Node* x = new Node(val)
```

### 7.2 指针的四种操作

```cpp
TreeNode* p = root;     // 声明指针，指向 root 节点

p->val                  // 访问指针指向的节点的 val（等价于 (*p).val）
p->left                 // 访问指针指向的节点的 left（也是指针）
p->next                 // 链表：访问下一个节点

p = p->left             // 让 p 指向左子节点（"往下走一步"）
p = p->next             // 链表：让 p 往后移一步

p == nullptr            // 判断指针是否为空
p != nullptr            // 判断指针是否非空
```

### 7.3 指针 vs 普通变量

```
普通变量用 .     → s.name, p.first, e.weight
指针变量用 ->    → p->val, cur->next, root->left

判断方法：看声明时有没有 *
  Student s;        → 普通变量，用 s.name
  Student* p;       → 指针变量，用 p->name
```

### 7.4 `new` 和 `delete`

```cpp
// new：在堆上创建对象，返回指针
ListNode* node = new ListNode(1);    // 创建一个节点
TreeNode* root = new TreeNode(5);    // 创建一个树节点

// delete：释放堆上的对象
delete node;

// 竞赛中通常不写 delete（程序结束自动释放，竞赛不在乎内存泄漏）
```

### 7.5 常见的指针遍历模式

```cpp
// 链表遍历
for (ListNode* p = head; p != nullptr; p = p->next) {
    cout << p->val;
}

// 二叉树遍历（递归）
void traverse(TreeNode* root) {
    if (root == nullptr) return;
    // 前序位置
    traverse(root->left);
    // 中序位置
    traverse(root->right);
    // 后序位置
}

// BFS 中队列存指针
queue<TreeNode*> q;
q.push(root);
while (!q.empty()) {
    TreeNode* cur = q.front();
    q.pop();
    if (cur->left) q.push(cur->left);
    if (cur->right) q.push(cur->right);
}
```

---

## 第八部分：模板（template）不可怕

### 8.1 模板就是"类型参数化"

```cpp
// 不用模板：要为每种类型写一份
vector<int> v1;
vector<string> v2;
vector<double> v3;

// vector 的定义大致是：
template<typename T>
class vector {
    T* data;        // T 是什么类型，data 就存什么类型
    void push_back(T val);
    T operator[](int i);
};

// 用的时候用 <> 告诉它 T 是什么
vector<int> v;      // T = int
vector<string> v;   // T = string
```

### 8.2 看到模板代码时怎么读？

**把 T/E/K/V 当成具体类型来读**。比如看到：

```cpp
template<typename K, typename V>
struct Node {
    K key;
    V val;
    Node* next;
};
```

在脑子里把 K 替换成 string，V 替换成 int：

```cpp
struct Node {
    string key;
    int val;
    Node* next;
};
```

这样就能读懂了。模板只是让同一份代码能处理不同类型，不改变任何逻辑。

---

## 第九部分：数据结构代码中的固定模式

### 9.1 模式一：初始化 + 循环处理 + 返回结果

几乎所有算法题都是这个模式：

```cpp
int solve(vector<int>& nums) {
    // 第一步：初始化数据结构
    int n = nums.size();
    vector<int> dp(n, 0);
    unordered_set<int> visited;

    // 第二步：用循环/递归处理
    for (int i = 0; i < n; i++) {
        // ...逻辑...
    }

    // 第三步：返回结果
    return dp[n-1];
}
```

### 9.2 模式二：BFS 模板（图/树通用）

```cpp
// 这个模板适用于所有 BFS 场景，只改中间的处理逻辑
queue<int> q;           // 或 queue<TreeNode*>、queue<pair<int,int>> 等
q.push(起点);
visited[起点] = true;   // 或 vector<bool>、unordered_set 等

while (!q.empty()) {
    int sz = q.size();  // 当前层的节点数
    for (int i = 0; i < sz; i++) {
        auto cur = q.front();
        q.pop();

        // 处理当前节点...

        for (auto& next : 当前节点的邻居) {
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }
    // 层数 +1（如果需要的话）
}
```

### 9.3 模式三：DFS 模板（图/树通用）

```cpp
// 遍历所有节点
void dfs(int node, vector<vector<int>>& graph, vector<bool>& visited) {
    if (visited[node]) return;
    visited[node] = true;

    // 前序位置：处理当前节点...

    for (int next : graph[node]) {
        dfs(next, graph, visited);
    }

    // 后序位置...
}
```

### 9.4 模式四：Dijkstra 模板

```cpp
vector<int> dist(n, INT_MAX);
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

dist[src] = 0;
pq.push({0, src});

while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();

    if (d > dist[u]) continue;  // 已有更短路径

    for (auto& [v, w] : graph[u]) {
        if (dist[u] + w < dist[v]) {
            dist[v] = dist[u] + w;
            pq.push({dist[v], v});
        }
    }
}
```

### 9.5 模式五：并查集模板

```cpp
class UF {
    vector<int> parent, sz;
    int cnt;
public:
    UF(int n) : parent(n), sz(n, 1), cnt(n) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void merge(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        if (sz[rx] < sz[ry]) swap(rx, ry);
        parent[ry] = rx;
        sz[rx] += sz[ry];
        cnt--;
    }
    bool connected(int x, int y) { return find(x) == find(y); }
    int count() { return cnt; }
};
```

---

## 第十部分：用一道完整的题目串起所有知识

下面我用 Dijkstra 最短路径的代码，标注每一行用到了哪些知识点：

```cpp
#include <vector>
#include <queue>
#include <climits>
using namespace std;

// 知识点：struct 定义（结构体，你学过的）
struct Edge {
    int to, weight;            // 成员变量
};

// 知识点：函数参数传引用（&），避免复制大数组
vector<int> dijkstra(vector<vector<Edge>>& graph, int src) {
    //     ↑ 返回类型：int数组
    //                 ↑ 参数类型：二维 Edge 数组（邻接表）

    int n = graph.size();      // 知识点：.size() 返回大小

    // 知识点：vector 初始化
    vector<int> dist(n, INT_MAX);   // n 个元素，全为 INT_MAX

    // 知识点：priority_queue 声明（小顶堆）
    // pair<int,int> 的 first = 距离，second = 节点
    priority_queue<
        pair<int,int>,                    // 存什么类型
        vector<pair<int,int>>,            // 底层容器（固定写法）
        greater<pair<int,int>>            // 比较器：小的优先
    > pq;

    dist[src] = 0;                        // 知识点：[] 访问
    pq.push({0, src});                    // 知识点：push 入堆，{} 构造 pair

    while (!pq.empty()) {                 // 知识点：.empty() 判断空
        // 知识点：auto + 结构化绑定，拆开 pair
        auto [d, u] = pq.top();          // d = pair.first, u = pair.second
        pq.pop();                         // 知识点：pop 不返回值

        if (d > dist[u]) continue;        // 知识点：continue 跳过本次循环

        // 知识点：范围 for 循环 + 引用 + 结构化绑定
        for (auto& [v, w] : graph[u]) {
            // v 是 Edge.to，w 是 Edge.weight
            // graph[u] 是 vector<Edge>（节点u的所有出边）

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});    // 发现更短路径，入堆
            }
        }
    }

    return dist;                          // 返回距离数组
}
```

**你看，整个 Dijkstra 算法用到的 C++ 知识：**

| 用到的知识 | 你学过了吗？ |
|-----------|------------|
| struct 定义 | ✅ basic14 |
| vector 创建和使用 | ✅ basic1 |
| 函数定义和返回值 | ✅ basic6 |
| 传引用 `&` | ✅ basic1 |
| for 范围循环 | ✅ basic1 |
| auto 和结构化绑定 | ✅ basic2 |
| pair 的 first/second | ✅ basic2 |
| priority_queue | ✅ basic28 |
| while 循环 | ✅ basic1 |
| continue | ✅ basic29 |

**你什么都学过了！只是没有把它们组合起来用过。**

---

## 第十一部分：你的行动清单

### 立即做的事

1. **把第三部分的类型操作表打印出来**，每次写代码时放在旁边查
2. **养成"看到变量先找类型"的习惯**，不要纠结变量名
3. **背下 BFS 和 DFS 的模板**（第九部分的模式二和模式三）

### 下一步做的事

1. 做 LeetCode 200（岛屿数量）—— 练习 DFS + visited
2. 做 LeetCode 102（二叉树层序遍历）—— 练习 BFS 模板
3. 做 LeetCode 215（第K大元素）—— 练习 priority_queue

### 遇到看不懂的代码时

```
第一步：找到所有变量的类型（往上翻找声明）
第二步：查这个类型有哪些操作（对照本文第三部分）
第三步：一行一行翻译成中文（"把 x 放进数组末尾"、"从堆顶取出最小的"等）
```

不要试图一眼看懂整段代码，像翻译外语一样一句一句来，速度会越来越快。

---

## 附录：所有容器操作速查表

| 操作 | vector | stack | queue | priority_queue | unordered_map | unordered_set | map/set |
|------|--------|-------|-------|----------------|---------------|---------------|---------|
| 加入 | push_back | push | push | push | mp[k]=v | insert | insert |
| 删除 | pop_back | pop | pop | pop | erase(k) | erase(x) | erase |
| 看头/顶 | front() | top() | front() | top() | — | — | begin() |
| 看尾 | back() | — | back() | — | — | — | rbegin() |
| 大小 | size() | size() | size() | size() | size() | size() | size() |
| 判空 | empty() | empty() | empty() | empty() | empty() | empty() | empty() |
| 随机访问 | v[i] ✅ | ❌ | ❌ | ❌ | mp[k] | ❌ | — |
| 判断存在 | find算法 | ❌ | ❌ | ❌ | count(k) | count(x) | count |
| 清空 | clear() | — | — | — | clear() | clear() | clear() |
