# 环形数组技巧及实现

---

## 目录

1. [为什么需要环形数组](#1-为什么需要环形数组)
2. [环形数组的核心原理](#2-环形数组的核心原理)
3. [start 和 end 指针](#3-start-和-end-指针)
4. [左闭右开区间](#4-左闭右开区间)
5. [完整代码实现](#5-完整代码实现)
6. [核心方法详解](#6-核心方法详解)
7. [为什么标准库不用环形数组](#7-为什么标准库不用环形数组)

---

## 1. 为什么需要环形数组

普通数组在头部增删元素，需要搬移所有数据，时间复杂度是 O(N)：

```
删除头部元素 1：
[1, 2, 3, 4, 5]
↓ 所有元素往前移一位
[2, 3, 4, 5, _]   搬移了4个元素，O(N)
```

**环形数组的目标：头部增删元素也能做到 O(1)，不搬移任何数据。**

---

## 2. 环形数组的核心原理

### 数组本身不是环形的

数组是一块线性连续内存，物理上不可能是环形的。但我们可以用**取模运算**让它在逻辑上变成环形：

```cpp
vector<int> arr = {1, 2, 3, 4, 5};
int i = 0;
while (true) {
    cout << arr[i] << endl;
    i = (i + 1) % arr.size();  // 到末尾自动回到开头
}
// 输出：1 2 3 4 5 1 2 3 4 5 1 2 3 ...（永远循环）
```

`%` 取模的效果：

```
i=0: (0+1) % 5 = 1
i=1: (1+1) % 5 = 2
i=2: (2+1) % 5 = 3
i=3: (3+1) % 5 = 4
i=4: (4+1) % 5 = 0  ← 自动回到开头！
```

### 用环形思路实现 O(1) 头部操作

普通数组头部删除要搬移数据，环形数组不需要：

```
初始：[1, 2, 3, _, _, _]

头部删除 1（不搬移，只标记为空）：
[_, 2, 3, _, _, _]

头部插入 4（直接放到空位）：
[4, 2, 3, _, _, _]

继续头部插入 5（前面没位置了，转一圈放到尾部）：
[4, 2, 3, _, _, 5]
        ↑逻辑上5在4前面，因为是环形的
```

关键：**不搬移数据，只移动"起始位置"的指针。**

>
> ● 你困惑的那个例子是一个简化示意，实际上它的 start/end
>   指针状态没有明确标出来，导致看起来很困惑。用文档后面更精确的 
>
> | 图示来理解：                                   |
> | ---------------------------------------------- |
> | 关键：逻辑顺序 = 从 start 出发，按环形走到 end |
>
>   以你困惑的那一步为例，还原完整状态：
>
>   初始：[1, 2, 3, _, _, _]
>          start=0, end=3
>
>   头部删除 1：清空 arr[0]，start 右移
>   [_, 2, 3, _, _, _]
>       start=1, end=3
>
>   头部插入 4：start 左移 → (1-1+6)%6=0，arr[0]=4
>   [4, 2, 3, _, _, _]
>    start=0, end=3
>
>   头部插入 5：start 左移 → (0-1+6)%6=5，arr[5]=5
>   [4, 2, 3, _, _, 5]
>                 start=5, end=3
>
>   此时逻辑顺序从 start=5 出发，绕一圈到 end=3：
>
>   索引5 → 索引0 → 索引1 → 索引2
>     5  →   4   →   2   →   3
>
>   所以5 在 4 的前面，因为 start 指向索引5，而索引5在物理上虽然
>   在最右边，但逻辑上它是队列的"头"。
>
>   用文档里的比喻：数组是一个圆，你站在 start
>   处往前走，遇到的第一个就是队头，5 就是那个队头。

---

## 3. start 和 end 指针

环形数组维护两个指针：

```
start：指向第一个有效元素的索引
end：  指向最后一个有效元素的下一个位置（开区间）
```

图示（数组长度6，存了3个元素）：

```
索引：  0    1    2    3    4    5
数组：[ 2 ][ 3 ][ _ ][ _ ][ _ ][ 1 ]
                 ↑              ↑
              end                start

逻辑顺序：1 → 2 → 3
（从start出发，绕一圈到end）
```

### 头部插入（移动 start） 

```
插入前：start=5, end=2
[ 2 ][ 3 ][ _ ][ _ ][ _ ][ 1 ]

start 左移一位：start = (5-1+6) % 6 = 4
在 arr[4] 放入新元素 0

插入后：start=4, end=2
[ 2 ][ 3 ][ _ ][ _ ][ 0 ][ 1 ]
逻辑顺序：0 → 1 → 2 → 3
```

### 尾部插入（移动 end）

```
插入前：start=4, end=2
[ 2 ][ 3 ][ _ ][ _ ][ 0 ][ 1 ]

在 arr[2] 放入新元素 4
end 右移一位：end = (2+1) % 6 = 3

插入后：start=4, end=3
[ 2 ][ 3 ][ 4 ][ _ ][ 0 ][ 1 ]
逻辑顺序：0 → 1 → 2 → 3 → 4
```

---

## 4. 左闭右开区间

代码中区间定义为 **[start, end)**，即 start 处有元素，end 处没有元素。

```
[start, end) 表示：
- start 位置：有元素（闭区间）
- end 位置：  没有元素（开区间），是下一个要填入的位置
```

**为什么选左闭右开？**

初始化时 `start = end = 0`，区间 `[0, 0)` 表示没有任何元素，最自然：

```
start=0, end=0：[0, 0) 没有元素，空数组  ✅

如果用两端都闭 [0, 0]：已经包含了索引0的元素，但数组是空的  ❌
如果用两端都开 (0, 0)：(0,0) 没有元素，但加入第一个元素后边界处理麻烦  ❌
```

**左闭右开对操作的影响：**

| 操作        | 因为 start 是闭区间      | 因为 end 是开区间      |
| ----------- | ------------------------ | ---------------------- |
| addFirst    | 先左移 start，再赋值     | —                     |
| removeFirst | 先赋值清空，再右移 start | —                     |
| addLast     | —                       | 先赋值，再右移 end     |
| removeLast  | —                       | 先左移 end，再赋值清空 |

---

## 5. 完整代码实现

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

template<typename T>
class CycleArray {
private:
    vector<T> arr;   // 底层数组
    int start;       // 第一个有效元素的索引（闭区间）
    int end;         // 最后一个有效元素的下一个位置（开区间）
    int count;       // 当前元素个数

    // 扩缩容
    void resize(int newSize) {
        vector<T> newArr(newSize);
        // 按逻辑顺序复制元素
        for (int i = 0; i < count; i++) {
            newArr[i] = arr[(start + i) % arr.size()];
        }
        arr = move(newArr);
        start = 0;
        end = count;
    }

public:
    CycleArray() : CycleArray(1) {}

    explicit CycleArray(int size)
        : arr(size), start(0), end(0), count(0) {}

    // 头部插入，O(1)
    void addFirst(const T &val) {
        if (isFull()) resize(arr.size() * 2);
        // start 是闭区间：先左移，再赋值
        start = (start - 1 + arr.size()) % arr.size();
        arr[start] = val;
        count++;
    }

    // 头部删除，O(1)
    void removeFirst() {
        if (isEmpty()) throw runtime_error("Array is empty");
        // start 是闭区间：先清空，再右移
        arr[start] = T();
        start = (start + 1) % arr.size();
        count--;
        if (count > 0 && count == arr.size() / 4) resize(arr.size() / 2);
    }

    // 尾部插入，O(1)
    void addLast(const T &val) {
        if (isFull()) resize(arr.size() * 2);
        // end 是开区间：先赋值，再右移
        arr[end] = val;
        end = (end + 1) % arr.size();
        count++;
    }

    // 尾部删除，O(1)
    void removeLast() {
        if (isEmpty()) throw runtime_error("Array is empty");
        // end 是开区间：先左移，再清空
        end = (end - 1 + arr.size()) % arr.size();
        arr[end] = T();
        count--;
        if (count > 0 && count == arr.size() / 4) resize(arr.size() / 2);
    }

    // 获取头部元素，O(1)
    T getFirst() const {
        if (isEmpty()) throw runtime_error("Array is empty");
        return arr[start];
    }

    // 获取尾部元素，O(1)
    T getLast() const {
        if (isEmpty()) throw runtime_error("Array is empty");
        // end 是开区间，要减1才是最后一个元素
        return arr[(end - 1 + arr.size()) % arr.size()];
    }

    bool isFull() const { return count == (int)arr.size(); }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};
```


>
>
> 好，这是C++类的一个特性，叫 **常量成员函数** 。
>
> ---
>
> # const 加在函数后面是什么意思？
>
> ```cpp
> bool isFull() const { ... }
> //            ↑
> //         加在这里
> ```
>
> 意思是：**这个函数承诺不会修改类的任何成员变量。**
>
> ---
>
> # 为什么要加？
>
> 类的成员函数分两种：
>
> ```cpp
> // 会修改成员变量的函数：不加 const
> void addFirst(const T &val) {
>     count++;    // 修改了 count
>     start = ...; // 修改了 start
> }
>
> // 只读、不修改成员变量的函数：加 const
> bool isFull() const {
>     return count == (int)arr.size();  // 只读了 count，没有修改
> }
> ```
>
> ---
>
> # 加了 const 有什么好处？
>
> **好处一：防止意外修改**
>
> 编译器会帮你检查，如果你在 `const` 函数里不小心写了修改操作，直接报错：
>
> ```cpp
> bool isFull() const {
>     count++;  // ❌ 编译报错！const函数不能修改成员变量
>     return count == (int)arr.size();
> }
> ```
>
> **好处二：可以被常量对象调用**
>
> ```cpp
> const CycleArray<int> arr;  // 常量对象，不允许任何修改
>
> arr.isFull();   // ✅ isFull 是 const 函数，可以调用
> arr.addFirst(1); // ❌ addFirst 不是 const 函数，不能调用
> ```
>
> ---
>
> # 竞赛中需要记住吗？
>
>  **不需要强记** ，竞赛中自己写的代码加不加 `const` 都能运行。
>
> 这是工程代码的规范写法，表示"我保证这个函数只读不写"。
>
> 看到的时候知道是什么意思就行：
>
> ```
> 函数名后面加 const = 这个函数只读数据，不修改任何成员变量
> ```
>

---

## 6. 核心方法详解

### 6.1 addFirst — 头部插入

```cpp
void addFirst(const T &val) {
    if (isFull()) resize(arr.size() * 2);
    start = (start - 1 + arr.size()) % arr.size();
    arr[start] = val;
    count++;
}
```

**为什么是 `(start - 1 + arr.size()) % arr.size()`？**

直接 `start - 1` 可能变成负数（比如 start=0 时，0-1=-1），负数不能作为数组索引。

加上 `arr.size()` 再取模，保证结果永远是正数：

```
start=0, arr.size()=6：
(0 - 1 + 6) % 6 = 5 % 6 = 5  ← 从0回到末尾5，正确！

start=3, arr.size()=6：
(3 - 1 + 6) % 6 = 8 % 6 = 2  ← 往左移一位，正确！
```

图示（start=0，插入新元素）：

```
插入前：
索引：  0    1    2    3    4    5
数组：[ 1 ][ 2 ][ 3 ][ _ ][ _ ][ _ ]
        ↑                      
       start=0

start左移：start = (0-1+6)%6 = 5
arr[5] = 新元素

插入后：
索引：  0    1    2    3    4    5
数组：[ 1 ][ 2 ][ 3 ][ _ ][ _ ][新]
                                 ↑
                               start=5
逻辑顺序：新→1→2→3  ✅
```

---

### 6.2 removeFirst — 头部删除

```cpp
void removeFirst() {
    arr[start] = T();               // 先清空 start 位置
    start = (start + 1) % arr.size(); // 再右移 start
    count--;
}
```

图示：

```
删除前：
[ _ ][ _ ][ 1 ][ 2 ][ 3 ][ _ ]
              ↑
           start=2

清空 arr[2]，start 右移：
[ _ ][ _ ][ _ ][ 2 ][ 3 ][ _ ]
                 ↑
              start=3

逻辑顺序：2→3  ✅
```

---

### 6.3 addLast — 尾部插入

```cpp
void addLast(const T &val) {
    arr[end] = val;                  // 先在 end 位置赋值
    end = (end + 1) % arr.size();   // 再右移 end
    count++;
}
```

end 是开区间（没有元素的位置），所以直接往 end 处填值，然后 end 右移。

---

### 6.4 getLast — 获取尾部元素

```cpp
T getLast() const {
    return arr[(end - 1 + arr.size()) % arr.size()];
}
```

end 指向的是**空位置**，最后一个真实元素在 `end-1` 处。

同样要加 `arr.size()` 防止负数：

```
end=0, arr.size()=6：
(0 - 1 + 6) % 6 = 5  ← 最后一个元素在索引5处，正确！
```

---

### 6.5 resize — 扩缩容

```cpp
void resize(int newSize) {
    vector<T> newArr(newSize);
    for (int i = 0; i < count; i++) {
        newArr[i] = arr[(start + i) % arr.size()];  // 按逻辑顺序复制
    }
    arr = move(newArr);
    start = 0;   // 重置 start
    end = count; // 重置 end
}
```

复制时不能直接按物理索引复制，要**按逻辑顺序**（从 start 开始）复制，否则顺序会乱。

复制完成后，重置 start=0，end=count，重新整理好。

---

## 7. 为什么标准库不用环形数组

既然环形数组能让头部操作也达到 O(1)，为什么 `vector` 没有用这个技巧？

**因为算总账不划算。**

环形数组的每次访问都需要计算真实索引：

```cpp
// 普通数组访问索引 i
arr[i]                          // 直接访问，极快

// 环形数组访问索引 i
arr[(start + i) % arr.size()]   // 多了加法和取模运算
```

`get` 操作在数组中调用频率**极高**，每次都多一步取模运算，积累下来性能损耗很大。

而在头部增删元素其实是**低频操作**，为了这个低频操作让高频操作变慢，得不偿失。

**结论：**

```
环形数组适合场景：双端队列（两端都频繁增删）
普通数组适合场景：大多数情况（随机访问频繁，头部增删少）

不是环形数组不能用，而是用在不对的场景反而更慢
```

---

> 📌 **核心总结**
>
> ```
> 环形数组 = 普通数组 + 取模运算 + start/end 双指针
>
> start 是闭区间：操作时先移动指针，再赋值（addFirst）
>                 或先赋值清空，再移动指针（removeFirst）
> end   是开区间：操作时先赋值，再移动指针（addLast）
>                 或先移动指针，再赋值清空（removeLast）
>
> 头尾增删全部 O(1)，代价是每次访问多一步取模运算
> ```
