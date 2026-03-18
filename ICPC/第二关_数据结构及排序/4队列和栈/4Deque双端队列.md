# 双端队列（Deque）原理及实现

> 前置知识：队列/栈基本原理、环形数组技巧

---

## 目录

1. [什么是双端队列](#1-什么是双端队列)
2. [用链表实现双端队列](#2-用链表实现双端队列)
3. [用数组实现双端队列](#3-用数组实现双端队列)
4. [总结](#4-总结)

---

## 1. 什么是双端队列

**标准队列**：只能队尾插入，队头删除（单向）。
它的规则非常明确，所以行为可预测。
但也因此，它在“需要两端都灵活处理”的题目里不够用。
这时就需要升级到双端队列。

**双端队列**：头尾两端都可以插入和删除（双向）。
你可以把它看成“队列能力的扩展版”。
保留了 FIFO 可用性，又增加了两端操作自由度。
所以很多窗口类题目会优先想到它。

```
标准队列（单向）：
  只能这端进 →  [1, 2, 3, 4]  → 只能这端出

双端队列（双向）：
  可以这端进出 ← [1, 2, 3, 4] → 可以这端进出
```

类比：

```
标准队列 = 排队买票，只能从队尾加入，从队头离开
双端队列 = 过街天桥，两端都可以随意进出
```

双端队列的 API：

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
deque<int> road;                    // 一条两头都能上下的“通道”
road.push_front(1);                 // 从前门进
road.push_back(2);                  // 从后门进
road.pop_front();                   // 从前门出
```

> 上面这个最小例子展示了“两端都能操作”，下面的接口代码就是把这些动作系统化命名。

```cpp
template<typename E>                // ⚠️ 模板：E 是元素类型占位符
class MyDeque {
public:
    void addFirst(E e);    // 队头插入，O(1)
    void addLast(E e);     // 队尾插入，O(1)
    E removeFirst();       // 队头删除，O(1)
    E removeLast();        // 队尾删除，O(1)
    E peekFirst();         // 查看队头，O(1)
    E peekLast();          // 查看队尾，O(1)
};
```

对比标准队列，多了对**队头插入**和**队尾删除**的支持。
这两项能力看起来小，实际非常关键。
在维护单调性或双端收缩窗口时，它们直接决定你能不能 O(1) 更新状态。
会用 deque，很多题会变得顺手。

| 操作 | 标准队列 | 双端队列 |
|------|:-------:|:-------:|
| 队头插入 | ❌ | ✅ |
| 队尾插入 | ✅ | ✅ |
| 队头删除 | ✅ | ✅ |
| 队尾删除 | ❌ | ✅ |

*就像单向地铁闸机和双向旋转门的区别：一个固定流向，一个两边都通。*

> 💡 **老师提醒：** 别把“双端队列”理解成“没有规则”。它依然有规则，只是允许你在两端做合法操作。

> ✅ 能区分“更灵活”与“更混乱”，你的概念已经很稳了。

---

## 2. 用链表实现双端队列

双链表天然支持头尾两端 O(1) 操作，直接调用就行。
这是非常典型的“底层能力直接映射上层接口”。
你几乎不需要额外技巧。
把接口名字对应好就完成了大半。

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
list<int> gate;                      // 两端都能进出的“通道”
gate.push_front(9);                  // 前门进
int x = gate.back();                 // 看后门那一端
gate.pop_back();                     // 后门出
```

> 上面的例子展示了双链表两端操作的便利，下面真实代码就是把这些操作封装成 `MyListDeque`。

```cpp
#include <iostream>   // 输出
#include <list>       // 双链表
using namespace std;  // 标准命名空间

template<typename E>  // ⚠️ 模板类：E 决定元素类型
class MyListDeque {
private:
    list<E> lst;   // 底层用双链表

public:
    // 队头插入，O(1)
    void addFirst(const E &e) {      // ⚠️ const E&：按引用传参，避免拷贝
        lst.push_front(e);
    }

    // 队尾插入，O(1)
    void addLast(const E &e) {
        lst.push_back(e);
    }

    // 队头删除，O(1)
    E removeFirst() {
        E first = lst.front();  // 先取值
        lst.pop_front();        // 再删除
        return first;           // 返回被删元素
    }

    // 队尾删除，O(1)
    E removeLast() {
        E last = lst.back();    // 先取尾部值
        lst.pop_back();         // 再删除尾部
        return last;
    }

    // 查看队头，O(1)
    E peekFirst() {
        return lst.front();     // 只读查看头部
    }

    // 查看队尾，O(1)
    E peekLast() {
        return lst.back();      // 只读查看尾部
    }
};

int main() {
    MyListDeque<int> deque;     // 创建双端队列对象
    deque.addFirst(1);   // [1]
    deque.addFirst(2);   // [2, 1]
    deque.addLast(3);    // [2, 1, 3]
    deque.addLast(4);    // [2, 1, 3, 4]

    cout << deque.removeFirst() << endl;  // 2，队列变成 [1, 3, 4]
    cout << deque.removeLast()  << endl;  // 4，队列变成 [1, 3]
    cout << deque.peekFirst()   << endl;  // 1（不删除）
    cout << deque.peekLast()    << endl;  // 3（不删除）
}
```

### 运行过程追踪

```
addFirst(1)：[1]
addFirst(2)：[2, 1]        ← 从头部插入，2排到最前面
addLast(3)： [2, 1, 3]     ← 从尾部插入
addLast(4)： [2, 1, 3, 4]  ← 从尾部插入

removeFirst()：返回2，链表 = [1, 3, 4]
removeLast()： 返回4，链表 = [1, 3]
peekFirst()：  返回1，链表不变
peekLast()：   返回3，链表不变
```

---

## 3. 用数组实现双端队列

直接复用之前实现的 `CycleArray`，因为环形数组头尾操作都是 O(1)。
你可以理解成“底层已经具备双端能力”。
所以这一层主要是接口命名转换。
逻辑简单，但思想很实用。

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
int head = 0;                        // 队头位置
int tail = 0;                        // 队尾后一个位置
head = (head - 1 + 8) % 8;           // 头部插入时，头指针向前绕一格
tail = (tail + 1) % 8;               // 尾部插入时，尾指针向后绕一格
```

> 这个小例子只演示“环形两端移动”的本质，下面真实代码把这些动作封装在 `CycleArray` 方法里。

```cpp
template<typename E>                 // ⚠️ 模板：E 为元素类型
class MyArrayDeque {
private:
    CycleArray<E> arr;   // 底层用环形数组

public:
    // 队头插入，O(1)
    void addFirst(E e) {
        arr.addFirst(e);   // 委托给底层环形数组
    }

    // 队尾插入，O(1)
    void addLast(E e) {
        arr.addLast(e);
    }

    // 队头删除，O(1)
    E removeFirst() {
        return arr.removeFirst();
    }

    // 队尾删除，O(1)
    E removeLast() {
        return arr.removeLast();
    }

    // 查看队头，O(1)
    E peekFirst() {
        return arr.getFirst();
    }

    // 查看队尾，O(1)
    E peekLast() {
        return arr.getLast();
    }
};
```

代码非常简单，就是把 `CycleArray` 的方法名换了个"马甲"对外暴露。
这样做的好处是上层语义更清楚。
题目说 deque，你就看 `addFirst/removeLast` 这些名字，不必关心底层细节。
抽象层次分清楚，写题会更快。

*就像电器插头和墙内电路：你只操作插头接口，内部走线不用每次都想一遍。*

> 💡 **老师提醒：** 如果你自己实现 `CycleArray`，请优先保证边界正确，再考虑美化代码。双端结构最怕 off-by-one。

> ✅ 你已经在练“复用底层能力”，这正是工程和竞赛都看重的能力。

---

## 4. 总结

```
双端队列 = 标准队列 + 头部也能插入/删除

实现方式：
  链表实现：双链表天然支持头尾 O(1) 操作，直接调用
  数组实现：普通数组头部 O(N)，必须用环形数组

两种实现的所有操作都是 O(1)
```

**C++ STL 中的双端队列：**

**第一步：最简单的例子（用你能想到的最日常的比喻）**

```cpp
#include <deque>                      // 引入双端队列
deque<int> d;                         // 建一个双端容器
d.push_front(1);                      // 前面进
int y = d.back();                     // 看后面元素
```

> 上面是最小动作组合，下面原代码给出 STL 的完整常用接口清单，便于你直接套题。

```cpp
#include <deque>   // 双端队列头文件

deque<int> dq;     // 定义 int 类型双端队列
dq.push_front(1);   // 队头插入
dq.push_back(2);    // 队尾插入
dq.pop_front();     // 队头删除
dq.pop_back();      // 队尾删除
dq.front();         // 查看队头
dq.back();          // 查看队尾
```

> 📌 **竞赛中的使用场景**
>
> 双端队列在算法题里用到最多的场景是**单调队列**，用来解决滑动窗口最大值等问题，后面学到会专门讲解。

---

**三种数据结构对比：**

| | 标准队列 | 栈 | 双端队列 |
|--|:-------:|:--:|:-------:|
| 队头插入 | ❌ | ❌ | ✅ |
| 队尾插入 | ✅ | ✅（栈顶） | ✅ |
| 队头删除 | ✅ | ❌ | ✅ |
| 队尾删除 | ❌ | ✅（栈顶） | ✅ |
| 规律 | 先进先出 | 先进后出 | 灵活，无固定规律 |

*就像自行车、摩托车、汽车：功能层层增强，但每一种都有最适合它的场景。*
