# 第八课：竞赛输入输出优化

## 为什么需要优化？

有些题目输入数据量非常大，比如读入100万个数字。默认的 `cin/cout` 比较慢，可能导致超时。

---

## 一行代码解决：加速cin/cout

```cpp
ios::sync_with_stdio(false);
cin.tie(nullptr);
```

把这两行加在 `main` 函数开头就行了，速度能提升 **3-5倍** 。

```cpp
int main() {
    ios::sync_with_stdio(false);  // 关闭cin和scanf的同步
    cin.tie(nullptr);              // 解除cin和cout的绑定

    // 后面正常用cin/cout就行
    int n;
    cin >> n;
}
```

---

## 两行代码的含义（了解即可）

**`ios::sync_with_stdio(false)`**

C++有两套输入输出：

* C语言的：`scanf / printf`
* C++的：`cin / cout`

默认情况下两套是同步的（可以混用），同步本身有开销。关掉同步后 `cin/cout` 会快很多。

代价是： **关掉之后不能混用 `scanf/printf` 和 `cin/cout`** ，竞赛中统一用 `cin/cout` 就没问题。

**`cin.tie(nullptr)`**

默认情况下每次用 `cin` 读取前，会先强制刷新 `cout` 的缓冲区（确保输出先显示出来）。解除绑定后就不会自动刷新了，速度更快。

---

## 二、用 `\n` 代替 `endl`

```cpp
cout << "hello" << endl;   // 慢：输出并强制刷新缓冲区
cout << "hello" << "\n";   // 快：只输出换行，不刷新
```

`endl` 每次都会刷新缓冲区，输出量大时很慢。竞赛中统一用 `"\n"`。

---

## 三、完整的竞赛模板

把前面所有课学的东西整合起来，这就是你以后每道题的起点：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <numeric>
#include <cmath>
using namespace std;

typedef long long ll;  // 以后写ll就是long long

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 在这里写题目逻辑

    return 0;
}
```

把这个模板背下来，以后每道题直接从这里开始写。

---

## 四、scanf/printf（了解即可）

有些老选手喜欢用C语言风格的输入输出，你可能在别人代码里看到：

```cpp
scanf("%d", &n);        // 读一个int
scanf("%lld", &n);      // 读一个long long
printf("%d\n", n);      // 输出一个int
printf("%lld\n", n);    // 输出一个long long
```

这个你不需要主动用，看懂别人代码就行。

---

# 八课全部学完了！

回顾一下你学完的东西：

```
✅ 第一课：输入输出（cin/cout）
✅ 第二课：函数
✅ 第三课：数组和字符串
✅ 第四课：STL算法函数
✅ 第五课：结构体struct
✅ 第六课：位运算
✅ 第七课：取模和快速幂
✅ 第八课：输入输出优化
```

**C++基础部分结束。**

---

# 下一阶段：开始刷算法题

从现在开始，我们边刷题边学算法。路线是这样的：

```
第一阶段：基础算法（现在开始）
  ├── 二分查找
  ├── 双指针
  ├── 前缀和
  └── 滑动窗口

第二阶段：数据结构
  ├── 栈和队列的应用
  ├── 链表
  └── 二叉树

第三阶段：进阶算法
  ├── 排序算法原理
  ├── 递归和回溯
  ├── 动态规划
  └── 图论基础
```

准备好了我们从**二分查找**开始，这是竞赛中最基础也最高频的算法之一。开始吗？
