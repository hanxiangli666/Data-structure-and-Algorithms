好，我来从头带你过一遍。你上传的这些文件覆盖了C++最核心的基础知识，我们按顺序一块一块来讲，每个概念都配例子，确保你真正理解再往下走。

---

# 第一块：基础语法（ctrl.cpp + std.cpp）

## 1. 程序的基本结构

```cpp
#include <iostream>
using namespace std;

int main() {
    // 你的代码写在这里
    return 0;
}
```

逐行解释：

* `#include <iostream>` — 引入"输入输出"工具包，这样才能用 `cout`（打印）和 `cin`（读取输入）
* `using namespace std;` — 告诉编译器我们用标准库里的东西，否则每次都要写 `std::cout`，很麻烦
* `int main()` — 程序的入口，C++程序从这里开始执行
* `return 0;` — 告诉系统"程序正常结束"

---

## 2. 输出：cout

```cpp
int a = 10;
cout << a << endl;          // 输出：10
cout << "Hello, World!" << endl;  // 输出：Hello, World!
```

* `cout` 就是"打印到屏幕"
* `<<` 是"往输出流里塞东西"，可以串联多个
* `endl` 是换行（相当于Python的 `\n`）

---

## 3. 变量和基本类型

```cpp
int a = 10;       // 整数
double b = 3.14;  // 浮点数
bool c = true;    // 布尔值（true/false）
string s = "abc"; // 字符串（需要 #include <string>）
```

跟Python相比，C++必须 **提前声明类型** ，之后不能改变。

---

## 4. 条件判断（if/else if/else）

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

和Python几乎一样，区别是：

* 条件要加括号 `(a > 5)`
* 代码块用花括号 `{}` 而不是缩进

---

## 5. 循环

**for循环：**

```cpp
for (int i = 0; i < 5; i++) {
    cout << i << " ";
}
// 输出：0 1 2 3 4
```

for循环括号里分三段，用分号隔开：

```
for (初始化 ; 条件 ; 每次循环后做什么) {}
```

* `int i = 0` — 从0开始
* `i < 5` — 只要i小于5就继续
* `i++` — 每次循环i加1（等价于Python的 `i += 1`）

**while循环：**

```cpp
int num = 100;
while (num > 0) {
    cout << num << " ";
    num /= 2;
}
// 输出：100 50 25 12 6 3 1
```

只要括号里条件为真就一直循环，和Python完全一样。

---

# 第二块：函数传值 vs 传引用

这是C++和Python差异最大的地方之一，非常重要。

## 先理解"传值"

```cpp
void modifyValue(int x) {
    x = 10;  // 只是修改了副本
}

int main() {
    int num = 5;
    modifyValue(num);
    cout << num << endl;  // 输出：5，原来的num没变！
}
```

 **直觉理解** ：传值就像你把一张纸上的数字**抄了一份**给函数。函数改的是那份抄本，你原来那张纸不变。

---

## 再理解"传引用"

```cpp
void modifyReference(int &x) {  // 注意：int后面有个 &
    x = 10;  // 修改的是原始数据
}

int main() {
    int num = 5;
    modifyReference(num);
    cout << num << endl;  // 输出：10，原来的num变了！
}
```

 **直觉理解** ：传引用就像你把那张纸本身交给了函数（或者给它一把能直接操作你那张纸的钥匙）。函数改的就是原始数据。

`&` 符号的意思就是"引用"，即"不要复制，直接操作原来那个变量"。

**竞赛中的重要规则（你的文件里也提到了）：**

| 情况                                | 推荐方式                                                |
| ----------------------------------- | ------------------------------------------------------- |
| `int`、`bool`等基本类型         | 传值（开销小）                                          |
| `vector`、`unordered_map`等容器 | **传引用** （避免复制大量数据）                   |
| 递归函数里有容器                    | **必须传引用** ，否则每层递归都复制一份，直接超时 |

---

# 第三块：STL容器

STL（Standard Template Library）是C++自带的工具箱，装了各种常用数据结构。你的文件里涉及了5种，我逐个讲。

---

## 1. vector（动态数组）← 最常用，优先掌握

 **可以把它理解为Python里的 `list`** ，但功能更多、更快。

```cpp
#include <vector>
using namespace std;

vector<int> nums(10);     // 大小为10，全为0
vector<int> nums2{1,3,5}; // 直接初始化
vector<int> nums3(5, 2);  // 大小为5，全为2
```

**常用操作：**

```cpp
vector<int> v{1, 2, 3};

v.push_back(4);    // 尾部加入4 → {1,2,3,4}
v.pop_back();      // 删除尾部  → {1,2,3}
v[0] = 99;         // 直接修改  → {99,2,3}
v.size();          // 长度：3
v.empty();         // 是否为空：false

// 遍历（类似Python的 for x in list）
for (int x : v) {
    cout << x << " ";
}
```

**二维数组（矩阵）：**

```cpp
// 创建 3行 4列，全为0的二维数组
vector<vector<int>> grid(3, vector<int>(4, 0));
grid[1][2] = 5;  // 第1行第2列设为5
```

---

## 2. stack（栈）

 **先进后出** ，就像一叠盘子，只能从顶部取放。

```cpp
#include <stack>

stack<int> s;
s.push(10);   // 放入：{10}
s.push(20);   // 放入：{10, 20}
s.push(30);   // 放入：{10, 20, 30}

s.top();      // 看顶部：30（不删除）
s.pop();      // 删除顶部 → {10, 20}
s.empty();    // 是否为空：false
s.size();     // 大小：2
```

---

## 3. queue（队列）

 **先进先出** ，就像排队买奶茶，先来的先服务。

```cpp
#include <queue>

queue<int> q;
q.push(10);  // 入队尾
q.push(20);
q.push(30);

q.front();   // 看队头：10
q.back();    // 看队尾：30
q.pop();     // 删除队头 → {20, 30}
q.size();    // 大小：2
```

---

## 4. unordered_map（哈希表）

 **可以理解为Python里的 `dict`** ，存键值对，查找是O(1)。

```cpp
#include <unordered_map>

unordered_map<int, string> mp;
mp[1] = "one";   // 插入
mp[2] = "two";

// 查找前先判断键是否存在！
if (mp.contains(1)) {     // C++20写法
    cout << mp[1] << endl; // 输出：one
}

mp.erase(2);  // 删除键2

// 遍历
for (auto &pair : mp) {
    cout << pair.first << " -> " << pair.second << endl;
}
```

⚠️  **C++特有的坑** （和Python不同）：

```cpp
unordered_map<int, string> mp;
cout << mp[99] << endl;  // 键99不存在，但不会报错！
                          // 而是自动创建了键99，值为""
cout << mp.size() << endl; // 输出：1 （被悄悄插入了）
```

所以 **访问之前一定要用 `.contains()` 先判断** 。

---

## 5. unordered_set（哈希集合）

 **可以理解为Python里的 `set`** ，只存值，不存键值对，用于快速判断"某个元素在不在"。

```cpp
#include <unordered_set>

unordered_set<int> s{1, 2, 3, 4};

s.insert(5);    // 插入
s.erase(2);     // 删除

if (s.contains(3)) {
    cout << "3在集合里" << endl;
}

s.size();   // 大小
s.empty();  // 是否为空
```

---

## 6. list（双向链表）

 **用得相对少** ，只在需要频繁在头部插入/删除时才用。你文件里的 `list` 大部分场景 `vector` 能替代。

暂时了解即可，等真正用到时再细学。

---

# 总结：你现在需要掌握的优先级

```
第一优先级（马上要会）：
  ✅ 基础语法（变量、if/else、for、while）
  ✅ cout / cin
  ✅ vector 的所有操作
  ✅ 传值 vs 传引用 的区别

第二优先级（接下来学）：
  ✅ stack、queue
  ✅ unordered_map、unordered_set

第三优先级（暂时了解）：
  ⏳ list（实际竞赛用得不多）
```

---

有哪个部分还不清楚，或者想直接动手练一道对应的题，告诉我！
