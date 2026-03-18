# STL容器和算法：竞赛实战指南

为什么要学 STL 呢？因为竞赛就是拼时间。STL 容器和算法就像工具箱——你不用自己写数据结构，不用自己写排序，直接用现成的、经过优化的工具。掌握 STL，你就能把时间花在算法设计上，而不是重复造轮子。

---

# 一、vector（动态数组）← 最常用

你已经学过基础，这里补充竞赛中最常用的写法。Vector 就像一个自动扩容的数组（这也是它的名字来源），你不用担心空间不够，push_back 就行。

```cpp
vector<int> v = {3, 1, 4, 1, 5};

// 最常用操作
v.push_back(9);      // 尾部加入
v.pop_back();        // 删除尾部
v.size();            // 大小
v.empty();           // 是否为空
v.back();            // 最后一个元素
v.front();           // 第一个元素
v.clear();           // 清空

// 竞赛常用：初始化二维数组
int n = 3, m = 4;
vector<vector<int>> grid(n, vector<int>(m, 0));  // n行m列全为0
```

> 💡 **老师提醒：** `v.size()` 返回的是 unsigned int，在循环条件里对比有时会出问题。习惯上我们写 `for (int i = 0; i < (int)v.size(); i++)` 或者 `for (auto x : v)`。

**竞赛场景：存图的邻接表（非常高频）**

```cpp
int n = 5;  // 5个节点
vector<int> adj[n];  // adj[i]存所有和i相连的节点

adj[0].push_back(1);  // 0和1相连
adj[0].push_back(2);  // 0和2相连
adj[1].push_back(3);  // 1和3相连

// 遍历和节点0相连的所有节点
for (int neighbor : adj[0]) {
    cout << neighbor << " ";  // 1 2
}
```

> ✅ Vector 是竞赛中用得最频繁的容器，现在就开始习惯它吧！

---

# 二、stack（栈）

 **先进后出** ，竞赛中主要用于：括号匹配、单调栈。想象一把刀插进刀架，后插的先拔出来。

```cpp
stack<int> s;
s.push(10);    // 入栈
s.push(20);
s.top();       // 看栈顶：20（不删除）
s.pop();       // 删除栈顶
s.empty();     // 是否为空
s.size();      // 大小
```

> 💡 **老师提醒：** Stack 没有迭代器，也没有 `.back()` 或 `.front()`，只能用 `.top()` 看栈顶元素。想要取出来就必须 pop —— 这就是栈的特性。

**竞赛场景：括号匹配**

```cpp
string brackets = "({[]})";
stack<char> s;
bool valid = true;

for (char c : brackets) {
    if (c == '(' || c == '{' || c == '[') {
        s.push(c);  // 左括号入栈
    } else {
        if (s.empty()) { valid = false; break; }
        char top = s.top();
        s.pop();
        // 检查是否匹配
        if (c == ')' && top != '(') valid = false;
        if (c == '}' && top != '{') valid = false;
        if (c == ']' && top != '[') valid = false;
    }
}
if (!s.empty()) valid = false;
cout << (valid ? "valid" : "invalid") << endl;
```

> ✅ 这是最经典的栈应用题，掌握这个思路，括号题永远不会出错！

---

# 三、queue（队列）

 **先进先出** ，竞赛中主要用于：BFS（广度优先搜索）。就像排队买票，先来的先被服务。

```cpp
queue<int> q;
q.push(10);    // 入队尾
q.push(20);
q.front();     // 看队头：10（不删除）
q.back();      // 看队尾：20
q.pop();       // 删除队头
q.empty();     // 是否为空
q.size();      // 大小
```

> 💡 **老师提醒：** Queue 也没有迭代器，操作受限——只能从一头进（`push`），从另一头出（`pop`）。这种受限的设计正是队列的安全性所在。

**竞赛场景：BFS模板（后面学图论时会深入讲）**

```cpp
queue<int> q;
q.push(start);  // 起点入队

while (!q.empty()) {
    int cur = q.front();
    q.pop();
    // 处理当前节点，把邻居加入队列
    for (int neighbor : adj[cur]) {
        q.push(neighbor);
    }
}
```

> ✅ BFS 是图论的基础，Queue 用得非常多，值得反复练习！

**priority_queue（优先队列/堆）← 竞赛高频！**

普通queue是先进先出，priority_queue是 **优先级高的先出** ，就像医院挂号，危重患者会优先被看医生。

```cpp
// 默认：最大堆（大的先出）
priority_queue<int> pq;
pq.push(3);
pq.push(1);
pq.push(4);
pq.top();   // 4（最大的）
pq.pop();   // 删除4
pq.top();   // 3

// 最小堆（小的先出）
priority_queue<int, vector<int>, greater<int>> pq2;
pq2.push(3);
pq2.push(1);
pq2.push(4);
pq2.top();  // 1（最小的）
```

> 💡 **老师提醒：** 写最小堆时那个 `greater<int>` 别忘了，这是 priority_queue 里容易出错的地方。如果不加，默认就是最大堆。

**竞赛场景：找前K个最大/最小值、Dijkstra最短路**

> ✅ Priority_queue 虽然语法复杂一点，但它在竞赛中的出现频率极高——一定要掌握！

---

# 四、map 和 unordered_map

两个都是键值对，区别是：

|          | unordered_map | map          |
| -------- | ------------- | ------------ |
| 底层     | 哈希表        | 红黑树       |
| 查找速度 | O(1)          | O(log n)     |
| 是否有序 | ❌ 无序       | ✅ 按key排序 |
| 竞赛首选 | 大多数情况    | 需要有序时   |

**怎么选？** 大多数竞赛问题用 `unordered_map`，因为它更快。只有当你需要按 key 的顺序遍历时，才用 `map`。

```cpp
// unordered_map：大多数情况用这个
unordered_map<string, int> cnt;
cnt["apple"]++;
cnt["banana"]++;
cnt["apple"]++;
cout << cnt["apple"] << endl;  // 2

// 遍历
for (auto &[key, val] : cnt) {
    cout << key << " " << val << endl;
}
```

> 💡 **老师提醒：** 那个 `auto &[key, val]` 的写法叫"结构化绑定"，C++17 的特性。如果老的编译器不支持，改成 `for (auto &p : cnt) { int key = p.first; int val = p.second; }` 也可以。

```cpp
// map：需要按key有序时用
map<int, string> m;
m[3] = "three";
m[1] = "one";
m[2] = "two";

// 遍历是按key从小到大的顺序
for (auto &[key, val] : m) {
    cout << key << " " << val << endl;
}
// 输出：
// 1 one
// 2 two
// 3 three
```

> ✅ unordered_map 和 map 选择要对，否则会踩到各种坑！

**竞赛最常用场景：统计频率**

```cpp
unordered_map<int, int> freq;
vector<int> v = {1, 2, 1, 3, 2, 1};

for (int x : v) {
    freq[x]++;   // 统计每个数出现几次
}

// freq[1]=3, freq[2]=2, freq[3]=1
```

---

# 五、set 和 unordered_set

两个都只存值，不存键值对。它们的主要作用是**自动去重**和**快速判断元素存不存在**。区别和map类似：

|          | unordered_set | set         |
| -------- | ------------- | ----------- |
| 查找速度 | O(1)          | O(log n)    |
| 是否有序 | ❌            | ✅ 自动排序 |
| 是否去重 | ✅            | ✅          |

**怎么选？** 同样的，大多数情况用 `unordered_set`。需要有序输出时用 `set`。

```cpp
// unordered_set：快速判断元素存不存在
unordered_set<int> visited;
visited.insert(1);
visited.insert(2);

if (visited.count(1)) {    // count返回0或1
    cout << "1已访问" << endl;
}
// 或者用contains（C++20）
if (visited.contains(2)) {
    cout << "2已访问" << endl;
}
```

> 💡 **老师提醒：** `contains` 是 C++20 新加的，很多竞赛服务器可能还不支持。稳妥的做法是用 `count`——如果返回 1 就说明存在。

```cpp
// set：自动排序+去重
set<int> s = {3, 1, 4, 1, 5, 9, 2, 6};
// s自动变成：{1, 2, 3, 4, 5, 6, 9}（有序且去重）

for (int x : s) {
    cout << x << " ";  // 1 2 3 4 5 6 9
}
```

> ✅ 这是对 set 和 unordered_set 最基本的使用，掌握了就够用！

**竞赛场景：判断是否访问过（BFS/DFS去重）**

```cpp
unordered_set<int> visited;
visited.insert(node);
if (!visited.count(node)) {
    // 没访问过，处理这个节点
}
```

---

# 六、STL算法函数补充

你已经学过sort、reverse、find等，这里补充几个在竞赛中特别重要的。

## lower_bound 和 upper_bound（必须掌握）

```cpp
vector<int> v = {1, 2, 2, 3, 4, 5};
//               0  1  2  3  4  5

// lower_bound：第一个 >= 目标值 的位置
auto it = lower_bound(v.begin(), v.end(), 2);
cout << (it - v.begin()) << endl;  // 1（索引1，值为2）

// upper_bound：第一个 > 目标值 的位置
auto it2 = upper_bound(v.begin(), v.end(), 2);
cout << (it2 - v.begin()) << endl;  // 3（索引3，值为3）

// 两者相减 = 某个值出现的次数
int count = upper_bound(v.begin(), v.end(), 2)
          - lower_bound(v.begin(), v.end(), 2);
cout << count << endl;  // 2（值2出现了2次）
```

图示：

```
数组：  1   2   2   3   4   5
索引：  0   1   2   3   4   5
            ↑           ↑
     lower_bound(2)  upper_bound(2)
```

> 💡 **老师提醒：** 这两个函数只能用在**有序数组/容器**上！如果你的数组没排序就用它们，结果是错的，而且编译还没有警告。

---

## next_permutation（全排列）

生成下一个字典序排列，竞赛中用于枚举所有排列。什么时候用？当数据很小（通常 n <= 8），需要尝试所有排列方案时。

```cpp
vector<int> v = {1, 2, 3};

// 枚举所有排列
do {
    for (int x : v) cout << x << " ";
    cout << endl;
} while (next_permutation(v.begin(), v.end()));

// 输出：
// 1 2 3
// 1 3 2
// 2 1 3
// 2 3 1
// 3 1 2
// 3 2 1
```

⚠️ **重要** ：使用前要先排序，否则只能生成部分排列：

```cpp
sort(v.begin(), v.end());  // 先排序确保从最小排列开始
do {
    // 处理每个排列
} while (next_permutation(v.begin(), v.end()));
```

> 💡 **老师提醒：** 这个函数的时间复杂度是 O(n!)，n=8 就是 40320 次操作，n=10 就是 360 万次。所以只在数据很小时使用。

---

## nth_element（快速选第K小）

找第K小的元素，比完整排序快。就像"我不需要整个排序好的数组，只需要知道第K个元素是多少"的场景。

```cpp
vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

// 找第3小的元素（索引2）
nth_element(v.begin(), v.begin() + 2, v.end());
cout << v[2] << endl;  // 第3小的数
```

> 💡 **老师提醒：** 这个函数的时间复杂度是 O(n)，比排序快得多。但它会破坏原数组的顺序，只保证第K个位置的元素是正确的。

---

## 完整速查表

| 容器/函数            | 核心用途           | 时间复杂度   |
| -------------------- | ------------------ | ------------ |
| `vector`           | 动态数组，存数据   | 访问O(1)     |
| `stack`            | 括号匹配，单调栈   | 操作O(1)     |
| `queue`            | BFS                | 操作O(1)     |
| `priority_queue`   | 堆，找最大最小     | 操作O(log n) |
| `unordered_map`    | 统计频率，键值映射 | 操作O(1)     |
| `map`              | 有序键值映射       | 操作O(log n) |
| `unordered_set`    | 判断元素存在       | 操作O(1)     |
| `set`              | 有序去重集合       | 操作O(log n) |
| `sort`             | 排序               | O(n log n)   |
| `lower_bound`      | 二分找>=的位置     | O(log n)     |
| `upper_bound`      | 二分找>的位置      | O(log n)     |
| `next_permutation` | 枚举全排列         | O(n)         |

**关键是记住哪个函数解决什么问题，用的时候查表就行。**

---

# 竞赛中的选择原则

```
需要存一列数据          → vector
需要快速判断存不存在    → unordered_set
需要统计频率/键值映射   → unordered_map
需要有序               → set / map
需要最大最小值          → priority_queue
需要BFS                → queue
需要括号匹配/单调栈     → stack
```

**最后的建议**：不要想着一次性掌握所有细节。竞赛时就是用速查表，看题目的需求，选对容器，剩下的就是用上这一页你学的知识。多做几道题，STL 的用法自然就会了。

> ✅ STL 全部学完了！现在可以真正开始做题了——从**二分查找**开始吗？
