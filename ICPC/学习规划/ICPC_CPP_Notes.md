# C++ 竞赛编程入门笔记

> 目标：从零开始备战 ICPC，冲击区域赛银/铜奖

---

## 目录

1. [基础语法](#1-基础语法)
2. [函数](#2-函数)
3. [数组与字符串](#3-数组与字符串)
4. [STL 容器](#4-stl-容器)
5. [STL 算法函数](#5-stl-算法函数)
6. [结构体 struct](#6-结构体-struct)
7. [位运算](#7-位运算)
8. [常用数学](#8-常用数学)
9. [输入输出优化](#9-输入输出优化)
10. [指针、引用、class 详解](#10-指针引用class-详解)
11. [LeetCode 刷题须知](#11-leetcode-刷题须知)
12. [ACM 模式说明与模板](#12-acm-模式说明与模板)
13. [常见错误总结](#13-常见错误总结)
14. [竞赛代码模板](#14-竞赛代码模板)

---

## 1. 基础语法

### 程序基本结构

```cpp
#include <iostream>
using namespace std;

int main() {
    // 你的代码写在这里
    return 0;
}
```

- `#include <iostream>`：引入输入输出工具包
- `using namespace std;`：使用标准库，省去每次写 `std::`
- `int main()`：程序入口
- `return 0;`：程序正常结束

### 输出 cout

```cpp
int a = 10;
cout << a << endl;               // 输出：10
cout << "Hello, World!" << endl; // 输出：Hello, World!
cout << "a = " << a << endl;     // 串联输出
```

- `endl` 表示换行，竞赛中推荐用 `"\n"` 代替（更快）

### 基本类型

```cpp
int a = 10;         // 整数
double b = 3.14;    // 浮点数
bool c = true;      // 布尔值（true/false）
string s = "abc";   // 字符串（需要 #include <string>）
long long x = 1e18; // 大整数，最大约 9.2×10^18
```

- C++ 必须**提前声明类型**，且声明后不能改变

### 条件判断

```cpp
int a = 10;

if (a > 5) {
    cout << "a > 5" << endl;
} else if (a == 5) {
    cout << "a == 5" << endl;
} else {
    cout << "a < 5" << endl;
}
```

- 条件要加括号 `(a > 5)`
- 代码块用花括号 `{}` 而不是缩进

### 循环

**for 循环：**

```cpp
// 格式：for (初始化; 条件; 每次循环后做什么) {}
for (int i = 0; i < 5; i++) {
    cout << i << " ";
}
// 输出：0 1 2 3 4
```

**while 循环：**

```cpp
int num = 100;
while (num > 0) {
    cout << num << " ";
    num /= 2;
}
// 输出：100 50 25 12 6 3 1
```

### break 和 continue

```cpp
// break：直接跳出整个循环
for (int i = 0; i < 5; i++) {
    if (i == 3) break;
    cout << i << " ";
}
// 输出：0 1 2

// continue：跳过本次，继续下一次
for (int i = 0; i < 5; i++) {
    if (i == 3) continue;
    cout << i << " ";
}
// 输出：0 1 2 4
```

> ⚠️ break 和 continue 只影响**最近的那一层循环**

---

## 2. 函数

### 基本写法

```cpp
// 格式：返回类型 函数名(参数列表) { ... }
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(3, 5);
    cout << result << endl;  // 8
    return 0;
}
```

### 无返回值用 void

```cpp
void printSum(int a, int b) {
    cout << a + b << endl;
    // 不需要 return
}
```

### 传值 vs 传引用

```cpp
// 传值：复制副本，不影响原变量
void modifyValue(int x) {
    x = 10;
}

// 传引用：直接操作原变量（& 表示引用）
void modifyReference(int &x) {
    x = 10;
}

int main() {
    int a = 5;
    modifyValue(a);
    cout << a << endl;      // 5，没变

    modifyReference(a);
    cout << a << endl;      // 10，变了
}
```

**竞赛中的规则：**

| 情况 | 推荐方式 |
|------|---------|
| `int`、`bool` 等基本类型 | 传值 |
| `vector`、`map` 等容器 | **传引用**（避免复制） |
| 递归函数里有容器 | **必须传引用**，否则超时 |

---

## 3. 数组与字符串

### vector（动态数组）← 替代原始数组

```cpp
#include <vector>

vector<int> nums(10);          // 大小为10，全为0
vector<int> nums2{1, 3, 5};   // 直接初始化
vector<int> nums3(5, 2);       // 大小为5，全为2

// 二维数组
vector<vector<int>> grid(3, vector<int>(4, 0));  // 3行4列全为0
```

常用操作：

```cpp
vector<int> v{1, 2, 3};
v.push_back(4);   // 尾部加入 → {1,2,3,4}
v.pop_back();     // 删除尾部 → {1,2,3}
v[0] = 99;        // 直接修改
v.size();         // 长度
v.empty();        // 是否为空
v.back();         // 最后一个元素
v.front();        // 第一个元素
v.clear();        // 清空

// 遍历
for (int x : v) {
    cout << x << " ";
}
```

### string（字符串）

```cpp
#include <string>

string s = "hello";
s.size();                      // 长度：5
s[0];                          // 取字符：'h'
string t = s + " world";       // 拼接
s == t;                        // 比较（支持 ==, !=, <, >）

// 截取子串
s.substr(0, 3);    // "hel"（从0开始，长度3）
s.substr(2);       // "llo"（从2到结尾）

// 查找
s.find("ll");      // 返回位置：2
s.find("xyz");     // 找不到返回 string::npos

// 遍历
for (char c : s) {
    cout << c << " ";
}
```

### 字符操作

```cpp
char c = 'A';
(int)c;          // 取ASCII码：65
tolower(c);      // 转小写：'a'
toupper('a');    // 转大写：'A'
isalpha(c);      // 是否是字母
isdigit(c);      // 是否是数字
islower(c);      // 是否是小写字母
isupper(c);      // 是否是大写字母
```

### 字符串与数字互转

```cpp
string s = to_string(123);   // 数字 → 字符串："123"
int n = stoi("123");         // 字符串 → 整数：123
double d = stod("3.14");     // 字符串 → 浮点：3.14
```

### 输入字符串

```cpp
string word;
cin >> word;              // 读一个单词（遇空格停止）

string line;
getline(cin, line);       // 读整行（包括空格）

// ⚠️ cin >> n 后紧跟 getline 要先清除残留回车
int n;
cin >> n;
cin.ignore();             // 清除残留回车
getline(cin, line);       // 现在才正确
```

---

## 4. STL 容器

### stack（栈）— 先进后出

```cpp
#include <stack>

stack<int> s;
s.push(10);    // 入栈
s.top();       // 看栈顶（不删除）
s.pop();       // 删除栈顶
s.empty();     // 是否为空
s.size();      // 大小
```

竞赛用途：括号匹配、单调栈

### queue（队列）— 先进先出

```cpp
#include <queue>

queue<int> q;
q.push(10);    // 入队尾
q.front();     // 看队头（不删除）
q.back();      // 看队尾
q.pop();       // 删除队头
q.empty();     // 是否为空
```

竞赛用途：BFS（广度优先搜索）

### priority_queue（优先队列/堆）← 高频！

```cpp
// 最大堆（默认，大的先出）
priority_queue<int> pq;
pq.push(3); pq.push(1); pq.push(4);
pq.top();    // 4

// 最小堆（小的先出）
priority_queue<int, vector<int>, greater<int>> pq2;
pq2.top();   // 最小值
```

竞赛用途：找前K大/小值、Dijkstra 最短路

### unordered_map（哈希表）← Python 的 dict

```cpp
#include <unordered_map>

unordered_map<string, int> cnt;
cnt["apple"]++;
cnt["apple"]++;
cnt["apple"];    // 2

// ⚠️ 访问不存在的键会自动创建！先判断再访问
if (cnt.count("banana")) {
    cout << cnt["banana"] << endl;
}

// 删除
cnt.erase("apple");

// 遍历
for (auto &[key, val] : cnt) {
    cout << key << " " << val << endl;
}
```

### map（有序哈希表）

```cpp
#include <map>

map<int, string> m;
m[3] = "three";
m[1] = "one";
// 遍历按 key 从小到大有序
for (auto &[key, val] : m) {
    cout << key << " " << val << endl;
}
```

| | unordered_map | map |
|--|:--:|:--:|
| 查找速度 | O(1) | O(log n) |
| 是否有序 | ❌ | ✅ |
| 竞赛首选 | 大多数情况 | 需要有序时 |

### unordered_set（哈希集合）← Python 的 set

```cpp
#include <unordered_set>

unordered_set<int> visited;
visited.insert(1);
visited.count(1);     // 存在返回1，不存在返回0
visited.contains(1);  // C++20，更直观
visited.erase(1);
```

### set（有序集合）

```cpp
#include <set>

set<int> s = {3, 1, 4, 1, 5};
// 自动变成：{1, 3, 4, 5}（有序且去重）
```

**容器选择原则：**

| 需求 | 选择 |
|------|------|
| 存一列数据 | `vector` |
| 快速判断存不存在 | `unordered_set` |
| 统计频率/键值映射 | `unordered_map` |
| 需要有序 | `set` / `map` |
| 最大最小值 | `priority_queue` |
| BFS | `queue` |
| 括号匹配/单调栈 | `stack` |

---

## 5. STL 算法函数

```cpp
#include <algorithm>
#include <numeric>
```

### sort（排序）

```cpp
vector<int> v = {5, 3, 1, 4, 2};

sort(v.begin(), v.end());                          // 从小到大
sort(v.begin(), v.end(), greater<int>());          // 从大到小

// 自定义排序（lambda）
sort(v.begin(), v.end(), [](int a, int b) {
    return abs(a) < abs(b);    // 按绝对值从小到大
});

// 字符串也可以排序
string s = "dcba";
sort(s.begin(), s.end());    // "abcd"
```

### reverse（反转）

```cpp
reverse(v.begin(), v.end());
reverse(s.begin(), s.end());
```

### max / min

```cpp
max(3, 7);    // 7
min(3, 7);    // 3

// 数组中最大最小值
int maxVal = *max_element(v.begin(), v.end());
int minVal = *min_element(v.begin(), v.end());
```

### lower_bound / upper_bound（二分查找）← 高频！

> ⚠️ 使用前数组必须已排序

```cpp
vector<int> v = {1, 2, 2, 3, 4, 5};

// lower_bound：第一个 >= 目标值 的位置
auto it = lower_bound(v.begin(), v.end(), 2);
cout << (it - v.begin()) << endl;   // 1

// upper_bound：第一个 > 目标值 的位置
auto it2 = upper_bound(v.begin(), v.end(), 2);
cout << (it2 - v.begin()) << endl;  // 3

// 统计某值出现次数
int count = upper_bound(v.begin(), v.end(), 2)
          - lower_bound(v.begin(), v.end(), 2);  // 2
```

图示：
```
数组：  1   2   2   3   4   5
索引：  0   1   2   3   4   5
            ↑           ↑
     lower_bound(2)  upper_bound(2)
```

### accumulate（求和）

```cpp
#include <numeric>

vector<int> v = {1, 2, 3, 4, 5};
int sum = accumulate(v.begin(), v.end(), 0);  // 15
```

### unique（去重）

```cpp
// 先排序，再去重
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());
```

### next_permutation（全排列）

```cpp
vector<int> v = {1, 2, 3};
sort(v.begin(), v.end());  // 先排序

do {
    for (int x : v) cout << x << " ";
    cout << endl;
} while (next_permutation(v.begin(), v.end()));
// 输出所有排列：1 2 3 / 1 3 2 / 2 1 3 / ...
```

### 算法速查表

| 函数 | 作用 | 头文件 |
|------|------|--------|
| `sort` | 排序 | algorithm |
| `reverse` | 反转 | algorithm |
| `max / min` | 两数比较 | algorithm |
| `max_element / min_element` | 数组最大最小值 | algorithm |
| `lower_bound` | 第一个 >= 目标值的位置 | algorithm |
| `upper_bound` | 第一个 > 目标值的位置 | algorithm |
| `binary_search` | 判断值是否存在 | algorithm |
| `accumulate` | 求和 | numeric |
| `unique` + `erase` | 去重 | algorithm |
| `next_permutation` | 枚举全排列 | algorithm |

---

## 6. 结构体 struct

### 基本写法

```cpp
struct Student {
    string name;
    int age;
    double score;
};  // ⚠️ 注意分号！

int main() {
    Student s;
    s.name = "Alice";
    s.age = 20;
    s.score = 95.5;

    // 直接初始化
    Student s2 = {"Bob", 21, 88.0};

    cout << s.name << " " << s.age << endl;
}
```

### 结合 vector

```cpp
vector<Student> students;
students.push_back({"Alice", 20, 95.5});
students.push_back({"Bob", 21, 88.0});

for (auto &s : students) {
    cout << s.name << " " << s.score << endl;
}
```

### 对 struct 排序（竞赛高频！）

```cpp
// 按成绩从高到低，成绩相同按名字字典序
sort(students.begin(), students.end(), [](Student &a, Student &b) {
    if (a.score != b.score) return a.score > b.score;
    return a.name < b.name;
});
```

### 竞赛常用场景

```cpp
// 存坐标
struct Point { int x, y; };

// 存图的边（图论高频）
struct Edge { int from, to, weight; };
vector<Edge> edges;
edges.push_back({1, 2, 5});
```

---

## 7. 位运算

计算机中所有数字用二进制存储，位运算直接操作二进制位，速度极快。

```
十进制  二进制
  5  →  0101
  3  →  0011
```

### 六种位运算

| 运算 | 符号 | 规则 | 例子 |
|------|------|------|------|
| 按位与 | `&` | 都是1才是1 | `5 & 3 = 1` |
| 按位或 | `\|` | 有一个1就是1 | `5 \| 3 = 7` |
| 按位异或 | `^` | 不同为1，相同为0 | `5 ^ 3 = 6` |
| 按位取反 | `~` | 0变1，1变0 | `~5 = -6` |
| 左移 | `<<` | 相当于乘2的n次方 | `1 << 3 = 8` |
| 右移 | `>>` | 相当于除2的n次方 | `8 >> 1 = 4` |

### 竞赛常用技巧

```cpp
n & 1          // 判断奇偶（1为奇，0为偶）
n << 1         // n * 2
n >> 1         // n / 2
1 << n         // 2^n
(n >> k) & 1   // 判断第k位是否为1
n | (1 << k)   // 把第k位设为1
n & ~(1 << k)  // 把第k位设为0
a ^ a          // 等于0（相同数异或为0）
```

### 经典应用：找只出现一次的数字

```cpp
// 其余数都出现两次，找出只出现一次的
vector<int> v = {1, 2, 3, 2, 1};
int result = 0;
for (int x : v) result ^= x;
cout << result << endl;  // 3
```

### 枚举子集（状态压缩）

```cpp
int n = 3;
for (int mask = 0; mask < (1 << n); mask++) {
    for (int i = 0; i < n; i++) {
        if ((mask >> i) & 1) {
            cout << i << " ";  // 第i个元素被选中
        }
    }
    cout << endl;
}
```

---

## 8. 常用数学

### 取模运算

竞赛题常要求"答案对 1e9+7 取模"，防止数字溢出。

```cpp
const int MOD = 1e9 + 7;

(a + b) % MOD           // 加法取模
(a * b) % MOD           // 乘法取模
(a - b + MOD) % MOD     // 减法取模（加MOD防止负数）
```

**核心性质：每步运算后取模，结果不变**

### long long

当数字可能超过21亿时，用 `long long` 代替 `int`：

```cpp
long long a = 1000000000LL;
// 竞赛中常用 typedef 简写
typedef long long ll;
ll a, b;
cin >> a >> b;
```

**数据范围参考：**

| 数据范围 | 类型 |
|---------|------|
| n ≤ 1e9 | `int` |
| n > 1e9 | `long long` |

### 快速幂

计算 $a^b \mod m$，时间复杂度 $O(\log b)$：

```cpp
long long power(long long a, long long b, long long mod) {
    long long result = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) result = result * a % mod;  // b是奇数
        a = a * a % mod;                        // 底数平方
        b >>= 1;                                // 指数折半
    }
    return result;
}

// 使用
cout << power(2, 10, MOD) << endl;  // 1024
```

### GCD（最大公约数）

```cpp
#include <numeric>

gcd(12, 8);          // 4（C++17）
__gcd(12, 8);        // 4（通用）

// 最小公倍数
int a = 12, b = 8;
a / gcd(a, b) * b;   // 24
```

### 常用数学函数

```cpp
#include <cmath>

abs(-5);      // 绝对值：5（int）
fabs(-3.14);  // 绝对值：3.14（double）
sqrt(16.0);   // 平方根：4.0
ceil(3.2);    // 向上取整：4.0
floor(3.8);   // 向下取整：3.0
```

> ⚠️ 竞赛中能用整数就不用浮点数，求幂次用快速幂不用 `pow()`

---

## 9. 输入输出优化

### 标准输入 cin

```cpp
int a;
cin >> a;          // 读一个整数

int a, b;
cin >> a >> b;     // 读两个整数（空格或回车分隔）

string s;
cin >> s;          // 读一个单词
```

### 竞赛常见输入模式

```cpp
// 模式一：读固定数量
int n;
cin >> n;
vector<int> a(n);
for (int i = 0; i < n; i++) cin >> a[i];

// 模式二：读到 EOF
int a, b;
while (cin >> a >> b) {
    cout << a + b << "\n";
}
```

### 加速 cin/cout（每题必加！）

```cpp
int main() {
    ios::sync_with_stdio(false);  // 关闭同步，速度提升3-5倍
    cin.tie(nullptr);              // 解除 cin 和 cout 的绑定

    // 后面正常用 cin/cout
}
```

### 用 `"\n"` 代替 `endl`

```cpp
cout << "hello" << endl;   // 慢：每次刷新缓冲区
cout << "hello" << "\n";   // 快：只换行
```

---

## 10. 指针、引用、class 详解

### & 的两种含义

```cpp
int a = 10;

int &ref = a;   // 左边有类型名：声明引用（别名）
&a;             // 左边没有类型名：取地址
```

### * 的两种含义

```cpp
int *p = &a;    // 左边有类型名：声明指针
*p = 20;        // 左边没有类型名：解引用（取值/赋值）
```

### 引用 vs 指针

```cpp
int a = 10;

int &ref = a;   // 引用：ref 就是 a 的别名
ref = 20;       // 直接修改，a 变成 20

int *p = &a;    // 指针：p 存的是 a 的地址
*p = 30;        // 解引用后修改，a 变成 30
```

### . 和 -> 的区别

```cpp
Student s;
s.name = "Alice";    // 普通变量用 .

Student *p = &s;
p->name = "Alice";   // 指针用 ->
// p->name 等价于 (*p).name
```

### new 和 delete

```cpp
int *p = new int;        // 申请单个对象
int *arr = new int[10];  // 申请数组

delete p;                // 释放单个对象
delete[] arr;            // 释放数组
```

> 竞赛中直接用 vector，基本不需要手动 new/delete

### class 的组成

```cpp
class Student {
private:               // 私有：外部不能直接访问
    string name;       // 成员变量
    int age;

public:                // 公开：外部可以访问
    // 构造函数：创建对象时自动执行，名字和类名相同
    Student(string n, int a) {
        name = n;
        age = a;
    }

    // 成员函数：对象能做的事
    void introduce() {
        cout << "我是" << name << "，" << age << "岁" << endl;
    }

    // 析构函数：对象消亡时自动执行，名字是 ~ 加类名
    ~Student() {
        // 释放 new 申请的内存（竞赛中基本不写）
    }
};

int main() {
    Student s("Alice", 20);  // 构造函数自动执行
    s.introduce();
}
```

### template（模板）

```cpp
// T 是类型占位符，用时再确定具体类型
template<typename T>
T maxVal(T a, T b) {
    return a > b ? a : b;
}

maxVal(3, 5);          // T=int
maxVal(3.14, 2.71);    // T=double
```

**竞赛中的优先级：**

```
必须掌握：引用(&)、vector、STL
了解原理：指针(*)、new/delete
看懂即可：class、template（竞赛直接用STL，不手写）
```

---

## 11. LeetCode 刷题须知

### 核心代码模式

LeetCode 给你一个函数框架，只需实现算法逻辑：

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // 在这里写算法
    }
};
```

### 常见错误类型

| 错误 | 含义 |
|------|------|
| Compile Error | 语法错误，无法编译 |
| Runtime Error | 运行时错误，如数组越界 |
| Wrong Answer | 结果错误，算法有问题 |
| Time Limit Exceeded (TLE) | 超时，算法复杂度太高 |
| Memory Limit Exceeded (MLE) | 内存超限，空间复杂度太高 |

### 重要注意事项

- **不要用文件级别的全局变量**，多个测试用例会互相影响，应使用类级别变量
- **提交前删除所有打印语句**，`cout` 是 IO 操作，严重影响性能
- 容器作为函数参数时，用**传引用**，否则每次递归都会复制数据

---

## 12. ACM 模式说明与模板

### 核心代码模式 vs ACM 模式

| | 核心代码模式（LeetCode）| ACM 模式（笔试/ICPC）|
|--|:--:|:--:|
| 输入处理 | 平台自动处理 | 自己读取解析 |
| 输出 | 通过返回值 | 自己打印到标准输出 |
| 导包 | 不需要 | 自己写 |
| 难度 | 简单 | 稍麻烦 |

### ACM 模式 C++ 通用模板

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <numeric>
#include <cmath>
using namespace std;

typedef long long ll;

// 把算法逻辑封装在 Solution 类中（和 LeetCode 核心代码模式保持一致）
class Solution {
public:
    int add(int a, int b) {
        return a + b;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b;
    // 读取到 EOF（不知道有多少组输入时用这个）
    while (cin >> a >> b) {
        int result = Solution().add(a, b);
        cout << result << "\n";
    }

    return 0;
}
```

**关键点：一定要读取到 EOF 才停，因为不知道有多少组测试用例。**

---

## 13. 常见错误总结

### 错误一：代码写在 main 外面 ⭐最高频

```cpp
// ❌ 执行语句跑到 main 外面
int n;
cin >> n;

int main() { ... }

// ✅ 所有执行语句在 main 里
int main() {
    int n;
    cin >> n;
}
```

> 原因：Python 没有 main 的概念，C++ 全局区域只能放**定义**，不能放**执行**

### 错误二：变量没有声明类型

```cpp
// ❌
int maxOfThree(a, b, c) { ... }

// ✅
int maxOfThree(int a, int b, int c) { ... }
```

### 错误三：花括号没有配对

```cpp
// ❌
for (char c : line) {
if (isupper(c)){
count++;}}

// ✅ 写 { 就立刻写对应的 }
for (char c : line) {
    if (isupper(c)) {
        count++;
    }
}
```

### 错误四：计数器定义在循环里

```cpp
// ❌ 每次循环都重置为0
for (char c : line) {
    int count = 0;
    if (isupper(c)) count++;
}

// ✅ 定义在循环外
int count = 0;
for (char c : line) {
    if (isupper(c)) count++;
}
```

### 错误五：去重/修改后还用旧的 n

```cpp
// ❌ 去重后 a.size() 变了，n 还是旧值
a.erase(unique(a.begin(), a.end()), a.end());
for (int i = 0; i < n; i++) { ... }

// ✅
for (int x : a) { ... }
```

### 错误六：赋值和计算混淆

```cpp
// ❌ 只计算，没有存回去（死循环！）
n >> 1;

// ✅
n >>= 1;   // 等价于 n = n >> 1
```

---

## 14. 竞赛代码模板

每次写题先把这个框架打出来，再往里填逻辑：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <numeric>
#include <cmath>
using namespace std;

typedef long long ll;
const int MOD = 1e9 + 7;

// 快速幂（需要时使用）
ll power(ll a, ll b, ll mod) {
    ll result = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) result = result * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 在这里写题目逻辑

    return 0;
}
```

---

> 📌 **学习路线提醒**
>
> C++ 基础 → 二分查找 → 双指针 → 前缀和 → 滑动窗口 → 栈队列应用 → 递归回溯 → 动态规划 → 图论基础
