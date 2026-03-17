# 位图（BitMap）原理及实现

---

## 目录

1. [为什么需要位图](#1-为什么需要位图)
2. [位图的核心原理](#2-位图的核心原理)
3. [索引计算规则](#3-索引计算规则)
4. [三个核心操作](#4-三个核心操作)
5. [完整代码实现](#5-完整代码实现)
6. [位运算优化](#6-位运算优化)
7. [总结](#7-总结)

---

## 1. 为什么需要位图

写算法时，经常用布尔数组记录哪些元素被访问过：

```cpp
vector<bool> visited(1000, false);
visited[10] = true;
visited[100] = true;
```

**问题：布尔数组太浪费内存。**

```
bool 只有 true / false 两种状态
理论上 1 个 bit（0或1）就够了

但实际上大多数语言里一个 bool 占 1 字节 = 8 bit
也就是说浪费了 7/8 的内存空间！
```

存 1000 个布尔值：

```
布尔数组：1000 × 8 bit = 8000 bit
位  图：  1000 × 1 bit = 1000 bit

节省了 7/8 的空间
```

> ⚠️ 日常刷题用普通布尔数组就够了，只有处理**超大规模数据**（比如布隆过滤器）才需要位图。

---

## 2. 位图的核心原理

位图的思路：**用一个 long 类型的每个 bit 位来存一个布尔值。**

一个 `unsigned long` 是 64 bit，可以同时存 64 个布尔值：

```
一个 unsigned long（64位）：
bit: 63 62 61 ... 3  2  1  0
值:   0  0  0 ...  0  0  1  1
                          ↑  ↑
                      表示索引1和0对应的元素存在
```

用一个 `vector<unsigned long>` 作为底层存储，相当于一个二维 bit 数组：

```
words[0]: bit0  bit1  bit2  ... bit63    （存索引 0~63）
words[1]: bit0  bit1  bit2  ... bit63    （存索引 64~127）
words[2]: bit0  bit1  bit2  ... bit63    （存索引 128~191）
...
```

---

## 3. 索引计算规则

给定一个 bitIndex，需要找到它在哪个 `words[i]` 的哪个 bit 位：

```cpp
int wordIndex = bitIndex / 64;   // 在第几个 words 元素
int bitOffset = bitIndex % 64;   // 在该元素的第几个 bit
```

**举例：找第 135 个 bit：**

```
wordIndex = 135 / 64 = 2    → 在 words[2]
bitOffset = 135 % 64 = 7    → 在 words[2] 的第7个bit位
```

用二维数组类比：

```
把 words 想象成一个二维数组：
每行有 64 列（64个bit位）

bitIndex=135 → 第 135/64=2 行，第 135%64=7 列
```

---

## 4. 三个核心操作

位图只需要三个操作，全部用位运算实现。

### get — 判断某个 bit 是否为 1

思路：把目标 bit 位**移到最低位**，和 1 做与运算：

```cpp
(words[wordIndex] >> bitOffset) & 1
```

等价写法（代码里用的）：

```cpp
(words[wordIndex] & (1ULL << bitOffset)) != 0
```

图示（判断 bitOffset=3 是否为1）：

```
words[i]:     1 0 1 1 0 1 0 0
1ULL << 3:    0 0 0 0 1 0 0 0
                        ↑ 只有第3位是1

& 运算：       0 0 0 0 0 0 0 0  → 结果为0，说明第3位是0
              或
              0 0 0 0 1 0 0 0  → 结果不为0，说明第3位是1
```

---

### set — 把某个 bit 设置为 1

思路：用**按位或**操作，只把目标位变成1，其他位不变：

```cpp
words[wordIndex] |= (1ULL << bitOffset);
```

图示（把 bitOffset=2 设置为1）：

```
words[i]:     1 0 1 1 0 0 0 0
1ULL << 2:    0 0 0 0 0 1 0 0

| 运算：       1 0 1 1 0 1 0 0  → 第2位变成1，其他位不变 ✅
```

---

### clear — 把某个 bit 设置为 0

思路：先取反得到一个"只有目标位是0其他位是1"的掩码，再用**按位与**：

```cpp
words[wordIndex] &= ~(1ULL << bitOffset);
```

图示（把 bitOffset=2 设置为0）：

```
1ULL << 2:        0 0 0 0 0 1 0 0
~(1ULL << 2):     1 1 1 1 1 0 1 1   ← 取反，只有第2位是0

words[i]:         1 0 1 1 0 1 0 0
& 运算：           1 0 1 1 0 0 0 0   → 第2位变成0，其他位不变 ✅
```

---

## 5. 完整代码实现

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

class MyBitSet {
private:
    vector<unsigned long> words;  // 底层存储
    int size;                      // 能存储的最大元素值+1

public:
    MyBitSet(int size) : size(size) {
        // 计算需要多少个 unsigned long
        // 比如 size=1000，需要 1000/64+1=16 个
        int arraySize = size / 64 + 1;
        words.resize(arraySize, 0);  // 全部初始化为0
    }

    // 判断第 bitIndex 个 bit 是否为 1
    bool get(int bitIndex) {
        if (bitIndex < 0 || bitIndex >= size)
            throw out_of_range("Index out of range");
        int wordIndex = bitIndex / 64;
        int bitOffset = bitIndex % 64;
        return (words[wordIndex] & (1ULL << bitOffset)) != 0;
    }

    // 将第 bitIndex 个 bit 设置为 1
    void set(int bitIndex) {
        if (bitIndex < 0 || bitIndex >= size)
            throw out_of_range("Index out of range");
        int wordIndex = bitIndex / 64;
        int bitOffset = bitIndex % 64;
        words[wordIndex] |= (1ULL << bitOffset);
    }

    // 将第 bitIndex 个 bit 设置为 0
    void clear(int bitIndex) {
        if (bitIndex < 0 || bitIndex >= size)
            throw out_of_range("Index out of range");
        int wordIndex = bitIndex / 64;
        int bitOffset = bitIndex % 64;
        words[wordIndex] &= ~(1ULL << bitOffset);
    }
};

int main() {
    MyBitSet bitSet(1000);

    bitSet.set(10);
    bitSet.set(100);
    bitSet.set(500);

    cout << bitSet.get(10)  << endl;   // 1（true）
    cout << bitSet.get(100) << endl;   // 1（true）
    cout << bitSet.get(200) << endl;   // 0（false）

    bitSet.clear(100);
    cout << bitSet.get(100) << endl;   // 0（false）

    return 0;
}
```

---

## 6. 位运算优化

代码中的 `/64` 和 `%64` 可以用位运算替代，速度更快：

| 原写法            | 优化写法          | 原理                                  |
| ----------------- | ----------------- | ------------------------------------- |
| `bitIndex / 64` | `bitIndex >> 6` | 64 = 2^6，除以64等于右移6位           |
| `bitIndex % 64` | `bitIndex & 63` | 除数是2的幂时，取模等于按位与(除数-1) |

```cpp
// 优化前
int wordIndex = bitIndex / 64;
int bitOffset = bitIndex % 64;

// 优化后
int wordIndex = bitIndex >> 6;
int bitOffset = bitIndex & 63;
```

这两种写法结果完全相同，位运算版本更快。

---

## 7. 总结

```
位图的本质：用每个 bit 位存一个布尔值，节省 7/8 的内存

底层结构：vector<unsigned long>
          每个 unsigned long 存 64 个布尔值

索引计算：
  wordIndex = bitIndex / 64   找到第几个 words 元素
  bitOffset = bitIndex % 64   找到该元素的第几个 bit

三个操作：
  get：用 &  判断某位是否为1
  set：用 |= 把某位设为1（不影响其他位）
  clear：用 &= ~ 把某位设为0（不影响其他位）
```

>
> 好，我退回最基础的地方，一个一个讲清楚。
>
> ---
>
> # 先复习：位运算操作的是什么
>
> 每个 `unsigned long` 有 64 个 bit，每个 bit 只能是 0 或 1：
>
> ```
> 索引：  7  6  5  4  3  2  1  0
> bits：  0  0  1  0  1  1  0  0
> ```
>
> 我们要操作的就是这些 bit 位。
>
> ---
>
> # 先理解 `1ULL << bitOffset` 是什么
>
> 这是三个操作都用到的核心工具，先把它搞懂。
>
> `1ULL << n` 就是把数字1左移n位，效果是 **只有第n位是1，其他全是0** ：
>
> ```
> 1ULL << 0:  0 0 0 0 0 0 0 1   只有第0位是1
> 1ULL << 1:  0 0 0 0 0 0 1 0   只有第1位是1
> 1ULL << 2:  0 0 0 0 0 1 0 0   只有第2位是1
> 1ULL << 3:  0 0 0 0 1 0 0 0   只有第3位是1
> ```
>
> 可以把 `1ULL << n` 理解为一把 **手电筒** ，只照亮第n位，其他位全黑。
>
> ---
>
> # get — 判断某位是否为1
>
> ```cpp
> (words[wordIndex] & (1ULL << bitOffset)) != 0
> ```
>
> **场景：** 想知道第3位是不是1。
>
> `1ULL << 3` = `0 0 0 0 1 0 0 0`，只有第3位是1。
>
> 用它和 words 做 `&` 运算：
>
> ```
> & 运算规则：两个都是1才是1，否则是0
>
> 情况一：第3位是1
> words:        1 0 1 1 1 0 1 0
> 1ULL << 3:    0 0 0 0 1 0 0 0
> & 结果：       0 0 0 0 1 0 0 0  ← 不是0，说明第3位是1 ✅
>
> 情况二：第3位是0
> words:        1 0 1 1 0 0 1 0
> 1ULL << 3:    0 0 0 0 1 0 0 0
> & 结果：       0 0 0 0 0 0 0 0  ← 是0，说明第3位是0 ✅
> ```
>
> **结论：** `& (1ULL << n)` 就像拿手电筒照第n位，结果不为0就是1，为0就是0。
>
> ---
>
> # set — 把某位设置为1
>
> ```cpp
> words[wordIndex] |= (1ULL << bitOffset);
> ```
>
> **场景：** 想把第2位设置为1，其他位不变。
>
> `|` 运算规则：有一个是1就是1。
>
> ```
> words:        1 0 1 1 0 0 1 0   （第2位现在是0）
> 1ULL << 2:    0 0 0 0 0 1 0 0   （只有第2位是1）
> | 结果：       1 0 1 1 0 1 1 0   （第2位变成1，其他位没变）✅
> ```
>
> **为什么其他位不变？**
>
> 因为 `1ULL << 2` 除了第2位，其他全是0。
>
> 任何数 `| 0` 还是它自己，所以其他位完全不受影响。
>
> ---
>
> # clear — 把某位设置为0
>
> ```cpp
> words[wordIndex] &= ~(1ULL << bitOffset);
> ```
>
> 这里多了一个 `~`，先解释 `~` 是什么。
>
> **`~` 是按位取反，0变1，1变0：**
>
> ```
> 1ULL << 2:     0 0 0 0 0 1 0 0   只有第2位是1
> ~(1ULL << 2):  1 1 1 1 1 0 1 1   只有第2位是0，其他全是1
> ```
>
> 然后用这个结果做 `&` 运算：
>
> ```
> & 运算规则：两个都是1才是1，否则是0
>
> words:           1 0 1 1 0 1 1 0   （第2位现在是1，想把它设为0）
> ~(1ULL << 2):    1 1 1 1 1 0 1 1   （只有第2位是0）
> & 结果：          1 0 1 1 0 0 1 0   （第2位变成0，其他位没变）✅
> ```
>
> **为什么其他位不变？**
>
> 因为 `~(1ULL << 2)` 除了第2位，其他全是1。
>
> 任何数 `& 1` 还是它自己，所以其他位完全不受影响。
>
> ---
>
> # 三个操作的规律总结
>
> ```
> 核心工具：1ULL << n
> 作用：制造一个只有第n位是1的数字（手电筒）
>
> get：  用 &        照一下第n位，看是0还是1
> set：  用 |=       强行把第n位变成1，其他位不动
> clear：用 &= ~     先取反变成只有第n位是0，再& 强行把第n位变成0，其他位不动
> ```
>
> ---
>
> # 一句话记忆
>
> ```
> 想让某位变1  → 用 |=（或运算，有1出1）
> 想让某位变0  → 用 &= ~（取反后与运算）
> 想读某位的值 → 用 &（与运算，结果非0就是1）
> ```
>

**内存对比：**

```
存 N 个布尔值：
  bool 数组：N 字节  = 8N bit
  位   图：  N/8 字节 = N bit

节省 87.5% 的内存
```

> 📌 **使用场景**
>
> 日常刷题：直接用 `vector<bool>` 就够了
> 超大规模数据（亿级别）：才需要位图节省内存，比如布隆过滤器
