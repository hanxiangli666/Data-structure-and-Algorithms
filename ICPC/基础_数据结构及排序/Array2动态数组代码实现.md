# 动态数组代码实现

> 前置知识：数组（顺序存储）基础原理

---

## 目录

1. [实现前的关键点](#1-实现前的关键点)
2. [完整代码实现](#2-完整代码实现)
3. [逐模块详解](#3-逐模块详解)
4. [运行示例追踪](#4-运行示例追踪)
5. [总结](#5-总结)

---

## 1. 实现前的关键点

### 关键点一：自动扩缩容策略

动态数组需要自动管理内存，我们采用以下策略：

| 触发条件                           | 操作                      |
| ---------------------------------- | ------------------------- |
| 元素个数 == 容量上限（数组满了）   | **扩容**：容量 × 2 |
| 元素个数 == 容量的 1/4（数组太空） | **缩容**：容量 ÷ 2 |

**为什么要缩容？**

假设数组开辟了能存 1000 个元素的空间，实际只存了 10 个，有 990 个位置空闲，白白浪费内存。缩容就是在元素减少时，适当释放多余的内存空间。

---

### 关键点二：索引越界检查的两种情况

代码中有两个越界检查方法，区别仅在一个 `=` 号：

```cpp
// 检查「访问/修改/删除」时的合法索引
bool isElementIndex(int index) {
    return index >= 0 && index < size;   // 严格小于 size
}

// 检查「插入」时的合法索引
bool isPositionIndex(int index) {
    return index >= 0 && index <= size;  // 可以等于 size
}
```

**为什么插入时 `index == size` 也合法？**

访问元素时，合法索引是已有元素的位置：

```
nums = [5, 6, 7, 8]
index   0  1  2  3      ← 合法范围：0 ~ size-1
```

插入元素时，新元素可以放在任意"空隙"位置，包括末尾：

```
nums = [ | 5 | 6 | 7 | 8 | ]
index    0   1   2   3   4  ← 合法范围：0 ~ size（包含末尾）
```

所以插入检查允许 `index == size`（即在末尾插入）。

---

### 关键点三：删除元素后要清空引用，防止内存泄漏

删除元素后，必须把该位置置为默认值：

```cpp
// 删除后清空，防止内存泄漏
data[size - 1] = E();   // E() 表示 E 类型的默认值（相当于 null / 0）
```

**为什么？**

如果不清空，虽然逻辑上已经"删除"了，但 `data[size-1]` 这个引用还指向那个对象，垃圾回收器认为它仍然可达，就不会释放这块内存，造成**内存泄漏**。

>
> 这里的核心点在于 **对象生命周期的管理** 。你对 `E()` 的困惑非常典型，因为它涉及到 C++ 的泛型语法。
>
> 简单来说，这一步是为了 **彻底切断联系** 。
>
> ---
>
> ### 1. 什么是 `E()`？
>
> 在 C++ 模板（泛型）中，`E` 代表某种数据类型（比如 `int`、`string` 或你定义的 `User` 类）。
>
> * **`E()` 是“值初始化”语法** ：它会生成一个该类型的“默认零值”。
> * 如果 `E` 是 `int`，`E()` 就是 `0`。
> * 如果 `E` 是 `指针`，`E()` 就是 `nullptr`。
> * 如果 `E` 是 `对象`，`E()` 就会调用它的默认构造函数，生成一个“空对象”。
>
> ### 2. 为什么要置空？（Loitering Objects 问题）
>
> 在逻辑上，我们通过 `size--` 已经认为这个元素“消失”了。但从物理内存的角度看，底层数组 `data` 在那个位置依然存放着之前的旧数据。
>
> #### 如果不执行 `data[size-1] = E()`：
>
> 1. **引用残留** ：虽然你以后不会去访问 `data[size]`，但底层数组还死死地拽着那个对象的“地址”。
> 2. **垃圾回收失效** ：在带有 GC（垃圾回收）机制的语言（如 Java, Go, 或者使用了智能指针的 C++）中，只要还有一个引用指向某个对象，该对象就永远不会被释放。
> 3. **内存泄漏** ：即使这个对象已经没用了，它依然占用着内存，直到整个数组被销毁。
>
>> **比喻** ：就像住酒店。你办理了退房（`size--`），但如果你不把房间钥匙还回去（置空引用），保洁阿姨（垃圾回收器）就认为房间里还有人，永远不会进去打扫，这个房间就一直被浪费着。
>>
>
> ### 3. C++ 里的特殊性
>
> 在原生 C++ 中，如果你存的是 **对象本身** （而不是指针），`size--` 之后虽然数据还在，但通常会被下一次 `add` 操作直接覆盖，不会导致严格意义上的内存泄漏。
>
> 但如果 `E` 是 **智能指针** （如 `std::shared_ptr`），这一步就 **至关重要** ：
>
> **C++**
>
> ```
> // 如果不置空，引用计数不会减1，对象永远不会析构
> data[size - 1] = nullptr; 
> ```
>
> ---
>
> ### 总结
>
> * **逻辑删除** ：`size--`（告诉程序以后别看这儿）。
> * **物理清理** ：`data[size - 1] = E()`（告诉内存管理器：我不再用这个对象了，你可以把它收走）。

---

## 2. 完整代码实现

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename E>
class MyArrayList {
private:
    E *data;               // 底层静态数组
    int size;              // 当前元素个数
    int cap;               // 数组容量上限
    static const int INIT_CAP = 1;  // 默认初始容量

public:
    // 默认构造函数
    MyArrayList() {
        data = new E[INIT_CAP];
        size = 0;
        cap = INIT_CAP;
    }

    // 指定初始容量的构造函数
    MyArrayList(int initCapacity) {
        data = new E[initCapacity];
        size = 0;
        cap = initCapacity;
    }



    // ==================== 增 ====================

    // 在末尾添加元素，O(1)
    void addLast(E e) {
        if (size == cap) resize(2 * cap);  // 满了就扩容
        data[size] = e;
        size++;
    }

    // 在指定索引处插入元素，O(N)
    void add(int index, E e) {
        checkPositionIndex(index);
        if (size == cap) resize(2 * cap);  // 满了就扩容

        // 从后往前搬移，给新元素腾位置
        for (int i = size - 1; i >= index; i--) {
            data[i + 1] = data[i];
        }
        data[index] = e;
        size++;
    }

    // 在头部添加元素，O(N)
    void addFirst(E e) {
        add(0, e);
    }

    // ==================== 删 ====================

    // 删除末尾元素，O(1)
    E removeLast() {
        if (size == 0) throw out_of_range("NoSuchElementException");
        if (size == cap / 4) resize(cap / 2);  // 太空就缩容

        E deletedVal = data[size - 1];
        data[size - 1] = E();  // 清空引用，防止内存泄漏
        size--;
        return deletedVal;
    }

    // 删除指定索引处的元素，O(N)
    E remove(int index) {
        checkElementIndex(index);
        if (size == cap / 4) resize(cap / 2);  // 太空就缩容

        E deletedVal = data[index];

        // 从前往后搬移，覆盖被删元素
        for (int i = index + 1; i < size; i++) {
            data[i - 1] = data[i];
        }
        data[size - 1] = E();  // 清空末尾引用
        size--;
        return deletedVal;
    }

    // 删除头部元素，O(N)
    E removeFirst() {
        return remove(0);
    }

    // ==================== 查 ====================

    // 查询指定索引的元素，O(1)
    E get(int index) {
        checkElementIndex(index);
        return data[index];
    }

    // ==================== 改 ====================

    // 修改指定索引的元素，O(1)
    E set(int index, E element) {
        checkElementIndex(index);
        E oldVal = data[index];
        data[index] = element;
        return oldVal;
    }

    // ==================== 工具方法 ====================

    int getSize() { return size; }

    bool isEmpty() { return size == 0; }

    void display() {
        cout << "size=" << size << " cap=" << cap << " | ";
        for (int i = 0; i < size; i++) cout << data[i] << " ";
        cout << endl;
    }

    // ==================== 私有方法 ====================

    // 扩缩容核心方法
    void resize(int newCap) {
        E *temp = new E[newCap];           // 申请新数组
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];             // 复制数据
        }
        delete[] data;                     // 释放旧数组
        data = temp;                       // 指向新数组
        cap = newCap;                      // 更新容量
    }

    bool isElementIndex(int index) {
        return index >= 0 && index < size;
    }

    bool isPositionIndex(int index) {
        return index >= 0 && index <= size;
    }

    void checkElementIndex(int index) {
        if (!isElementIndex(index))
            throw out_of_range("Index out of bounds");
    }

    void checkPositionIndex(int index) {
        if (!isPositionIndex(index))
            throw out_of_range("Index out of bounds");
    }

    // 析构函数：对象销毁时自动释放内存
    ~MyArrayList() {
        delete[] data;
    }
};

int main() {
    MyArrayList<int> arr(3);  // 初始容量为 3

    for (int i = 1; i <= 5; i++) arr.addLast(i);  // 添加 1~5

    arr.remove(3);       // 删除索引3的元素（值为4）
    arr.add(1, 9);       // 在索引1处插入9
    arr.addFirst(100);   // 在头部插入100
    int val = arr.removeLast();  // 删除末尾元素

    // 输出：100 1 9 2 3
    for (int i = 0; i < arr.getSize(); i++) {
        cout << arr.get(i) << endl;
    }

    return 0;
}
```

>
>
> 这两个都是 **构造函数** ，区别只是创建对象时你有没有指定初始容量。
>
> ---
>
> # 区别就一件事：初始容量从哪来
>
> **构造函数一：不传参数，用默认容量**
>
> ```cpp
> MyArrayList() {
>     data = new E[INIT_CAP];  // INIT_CAP = 1，固定的
>     size = 0;
>     cap = INIT_CAP;
> }
> ```
>
> ```cpp
> // 使用方式
> MyArrayList<int> arr;   // 不传任何参数
> // 初始容量固定是 1
> ```
>
> ---
>
> **构造函数二：传入参数，用你指定的容量**
>
> ```cpp
> MyArrayList(int initCapacity) {
>     data = new E[initCapacity];  // 容量是你传进来的
>     size = 0;
>     cap = initCapacity;
> }
> ```
>
> ```cpp
> // 使用方式
> MyArrayList<int> arr(3);   // 传入 3，初始容量就是 3
> MyArrayList<int> arr(100); // 传入 100，初始容量就是 100
> ```
>
> ---
>
> # 为什么需要两个？
>
> 如果你**提前知道**大概要存多少元素，就用第二个指定容量，可以减少扩容次数：
>
> ```cpp
> // 知道要存 1000 个元素
> MyArrayList<int> arr(1000);
> // 一开始就够用，不需要反复扩容
>
> // 不知道要存多少
> MyArrayList<int> arr;
> // 从容量1开始，按需扩容：1→2→4→8→...
> ```
>
> ---
>
> # 类比理解
>
> 就像买收纳箱：
>
> * **不传参数** ：先拿一个最小的箱子，装满了再换大的
> * **传入参数** ：提前估计好要装多少东西，直接买个合适的箱子，省得频繁换
>
> 两个构造函数其实干的是 **完全一样的事** ，只是初始容量的来源不同。


---

## 3. 逐模块详解

### 3.1 类的成员变量

```cpp
E *data;    // 底层静态数组（指针指向堆上的连续内存）
int size;   // 当前实际存储的元素个数
int cap;    // 底层数组的总容量
```

三者的关系：

```
cap（总容量）= 10
size（已用）= 4

data: [1] [2] [3] [4] [_] [_] [_] [_] [_] [_]
       ←  size=4  →    ←      空闲       →
       ←              cap=10              →
```

---

### 3.2 addLast — 末尾添加 O(1)

```cpp
void addLast(E e) {
    if (size == cap) resize(2 * cap);  // 1. 满了先扩容
    data[size] = e;                     // 2. 在末尾赋值
    size++;                             // 3. 元素个数+1
}
```

图示（不触发扩容）：

```
添加前：[1] [2] [3] [_] [_]   size=3, cap=5
                  ↑ size指向下一个空位

添加后：[1] [2] [3] [4] [_]   size=4, cap=5
```

---

### 3.3 add — 中间插入 O(N)

```cpp
void add(int index, E e) {
    checkPositionIndex(index);         // 1. 越界检查
    if (size == cap) resize(2 * cap); // 2. 满了先扩容

    // 3. 从后往前搬移（必须从后往前，否则会覆盖数据）
    for (int i = size - 1; i >= index; i--) {
        data[i + 1] = data[i];
    }

    data[index] = e;  // 4. 插入新元素
    size++;           // 5. 元素个数+1
}
```

图示（在 index=1 处插入 9）：

```
插入前：[1] [2] [3] [4] [_]
            ↑ 要在这里插入9

从后往前移：
  data[4] = data[3] → [1] [2] [3] [4] [4]
  data[3] = data[2] → [1] [2] [3] [3] [4]
  data[2] = data[1] → [1] [2] [2] [3] [4]

插入9：
  data[1] = 9       → [1] [9] [2] [3] [4]
```

> ⚠️ 必须**从后往前**搬移，从前往后会把后面的数据覆盖掉。

---

### 3.4 removeLast — 末尾删除 O(1)

```cpp
E removeLast() {
    if (size == 0) throw out_of_range("...");  // 空数组检查
    if (size == cap / 4) resize(cap / 2);      // 太空就缩容

    E deletedVal = data[size - 1];  // 保存要返回的值
    data[size - 1] = E();           // 清空引用（防内存泄漏）
    size--;                         // 元素个数-1
    return deletedVal;
}
```

---

### 3.5 remove — 中间删除 O(N)

```cpp
E remove(int index) {
    checkElementIndex(index);              // 越界检查
    if (size == cap / 4) resize(cap / 2); // 太空就缩容

    E deletedVal = data[index];

    // 从前往后搬移（必须从前往后，否则会覆盖数据）
    for (int i = index + 1; i < size; i++) {
        data[i - 1] = data[i];
    }

    data[size - 1] = E();  // 清空末尾引用（防内存泄漏）
    size--;
    return deletedVal;
}
```

图示（删除 index=1 处的元素）：

```
删除前：[1] [9] [2] [3] [4]
            ↑ 要删除这个

从前往后移：
  data[1] = data[2] → [1] [2] [2] [3] [4]
  data[2] = data[3] → [1] [2] [3] [3] [4]
  data[3] = data[4] → [1] [2] [3] [4] [4]

清空末尾：
  data[4] = E()     → [1] [2] [3] [4] [_]
```

> ⚠️ 必须**从前往后**搬移，从后往前会把前面的数据覆盖掉。

---

### 3.6 resize — 扩缩容核心 O(N)

```cpp
void resize(int newCap) {
    E *temp = new E[newCap];           // 1. 申请新内存
    for (int i = 0; i < size; i++) {
        temp[i] = data[i];             // 2. 复制数据
    }
    delete[] data;                     // 3. 释放旧内存
    data = temp;                       // 4. 指向新数组
    cap = newCap;                      // 5. 更新容量
}
```

图示（扩容：cap 从 3 扩到 6）：

```
扩容前：data → [1] [2] [3]          cap=3, size=3

申请新数组：temp → [_] [_] [_] [_] [_] [_]

复制数据：  temp → [1] [2] [3] [_] [_] [_]

释放旧数组，data 指向 temp：
           data → [1] [2] [3] [_] [_] [_]  cap=6
```

---

### 3.7 搬移方向总结

| 操作                       | 搬移方向           | 原因                       |
| -------------------------- | ------------------ | -------------------------- |
| **插入**（腾出空位） | **从后往前** | 从前往后会覆盖还没搬的数据 |
| **删除**（填补空位） | **从前往后** | 从后往前会覆盖还没搬的数据 |

>
>
> 好，我用具体例子一步一步演示给你看。
>
> ---
>
> # 插入：为什么必须从后往前？
>
> 假设要在 index=1 处插入 9，数组现在是：
>
> ```
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 3 ][ 4 ][ _ ]
> ```
>
> ## 错误示范：从前往后移
>
> ```
> 第一步：data[2] = data[1]
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 2 ][ 4 ][ _ ]
>                   ↑ 2复制过来了，但原来的3被覆盖了！
>
> 第二步：data[3] = data[2]
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 2 ][ 2 ][ _ ]
>                        ↑ 复制的是已经被污染的2，3永远丢失了！
> ```
>
> **结果：3 和 4 都丢了，数据损坏。**
>
> ---
>
> ## 正确做法：从后往前移
>
> ```
> 第一步：data[4] = data[3]
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 3 ][ 4 ][ 4 ]
>                             ↑ 先把最后的4移走
>
> 第二步：data[3] = data[2]
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 3 ][ 3 ][ 4 ]
>                        ↑ 再移3
>
> 第三步：data[2] = data[1]
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 2 ][ 3 ][ 4 ]
>                   ↑ 再移2
>
> 插入9：data[1] = 9
> index:  0    1    2    3    4
> data: [ 1 ][ 9 ][ 2 ][ 3 ][ 4 ]  ✅
> ```
>
> ---
>
> # 删除：为什么必须从前往后？
>
> 假设要删除 index=1 处的 9，数组现在是：
>
> ```
> index:  0    1    2    3    4
> data: [ 1 ][ 9 ][ 2 ][ 3 ][ 4 ]
> ```
>
> ## 错误示范：从后往前移
>
> ```
> 第一步：data[3] = data[4]
> index:  0    1    2    3    4
> data: [ 1 ][ 9 ][ 2 ][ 4 ][ 4 ]
>                        ↑ 把4移过来了，但原来的3被覆盖了！
>
> 第二步：data[2] = data[3]
> index:  0    1    2    3    4
> data: [ 1 ][ 9 ][ 4 ][ 4 ][ 4 ]
>                   ↑ 复制的是已经被污染的4，3永远丢失了！
> ```
>
> **结果：3 丢了，数据损坏。**
>
> ---
>
> ## 正确做法：从前往后移
>
> ```
> 第一步：data[1] = data[2]
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 2 ][ 3 ][ 4 ]
>             ↑ 用2覆盖掉要删除的9
>
> 第二步：data[2] = data[3]
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 3 ][ 3 ][ 4 ]
>                   ↑ 再移3
>
> 第三步：data[3] = data[4]
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 3 ][ 4 ][ 4 ]
>                        ↑ 再移4
>
> 清空末尾：data[4] = E()
> index:  0    1    2    3    4
> data: [ 1 ][ 2 ][ 3 ][ 4 ][ _ ]  ✅
> ```
>
> ---
>
> # 一句话总结
>
> ```
> 插入（从后往前）：先把最远的搬走，再逐步往前，
>                  保证每次搬移的源数据还没被动过
>
> 删除（从前往后）：从被删位置开始往前覆盖，
>                  保证每次搬移的源数据还没被动过
> ```
>
> 本质规律： **搬移方向要和"还没被动过的数据"的方向一致** ，这样才不会覆盖还没搬的数据。

---

## 4. 运行示例追踪

```cpp
MyArrayList<int> arr(3);          // size=0, cap=3
for (int i = 1; i <= 5; i++)
    arr.addLast(i);
```

| 操作       | 触发扩容？               | data 内容          | size | cap |
| ---------- | ------------------------ | ------------------ | :--: | :-: |
| addLast(1) | 否                       | [1, _, _]          |  1  |  3  |
| addLast(2) | 否                       | [1, 2, _]          |  2  |  3  |
| addLast(3) | 否                       | [1, 2, 3]          |  3  |  3  |
| addLast(4) | **是（cap×2=6）** | [1, 2, 3, 4, _, _] |  4  |  6  |
| addLast(5) | 否                       | [1, 2, 3, 4, 5, _] |  5  |  6  |

```cpp
arr.remove(3);    // 删除索引3（值为4）→ [1, 2, 3, 5, _, _]  size=4
arr.add(1, 9);    // 在索引1插入9      → [1, 9, 2, 3, 5, _]  size=5
arr.addFirst(100);// 在头部插入100     → [100, 1, 9, 2, 3, 5] size=6
arr.removeLast(); // 删除末尾（值为5） → [100, 1, 9, 2, 3, _] size=5
```

最终输出：

```
100
1
9
2
3
```

---

## 5. 总结

### 各操作时间复杂度

| 操作              | 时间复杂度 | 关键原因                            |
| ----------------- | :--------: | ----------------------------------- |
| `addLast`       | O(1) 均摊 | 直接赋值；偶尔扩容均摊后仍 O(1)     |
| `add(index)`    |    O(N)    | 需要搬移数据                        |
| `addFirst`      |    O(N)    | 等价于 `add(0)`                   |
| `removeLast`    | O(1) 均摊 | 直接操作末尾；偶尔缩容均摊后仍 O(1) |
| `remove(index)` |    O(N)    | 需要搬移数据                        |
| `removeFirst`   |    O(N)    | 等价于 `remove(0)`                |
| `get(index)`    |    O(1)    | 直接计算地址                        |
| `set(index)`    |    O(1)    | 直接计算地址                        |
| `resize`        |    O(N)    | 需要复制所有数据                    |

### 三个必须记住的细节

```
1. 插入搬移：从后往前（防覆盖）
2. 删除搬移：从前往后（防覆盖）
3. 删除后清空：data[size-1] = E()（防内存泄漏）
```

### 和 vector 的对比

这份代码实现的就是 C++ `vector` 的底层原理：

| 我们的实现        | vector 等价操作          |
| ----------------- | ------------------------ |
| `addLast(e)`    | `push_back(e)`         |
| `addFirst(e)`   | `insert(begin(), e)`   |
| `add(i, e)`     | `insert(begin()+i, e)` |
| `removeLast()`  | `pop_back()`           |
| `removeFirst()` | `erase(begin())`       |
| `remove(i)`     | `erase(begin()+i)`     |
| `get(i)`        | `arr[i]`               |
| `set(i, e)`     | `arr[i] = e`           |

> 📌 **一句话总结**
>
> 动态数组的核心就是：底层静态数组 + 自动扩缩容 + 增删时的数据搬移。
> 理解了这些，vector 的所有行为你都能解释清楚。
