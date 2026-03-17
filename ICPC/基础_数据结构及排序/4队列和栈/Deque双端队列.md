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

**标准队列**：只能队尾插入，队头删除（单向）

**双端队列**：头尾两端都可以插入和删除（双向）

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

```cpp
template<typename E>
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

对比标准队列，多了对**队头插入**和**队尾删除**的支持：

| 操作 | 标准队列 | 双端队列 |
|------|:-------:|:-------:|
| 队头插入 | ❌ | ✅ |
| 队尾插入 | ✅ | ✅ |
| 队头删除 | ✅ | ✅ |
| 队尾删除 | ❌ | ✅ |

---

## 2. 用链表实现双端队列

双链表天然支持头尾两端 O(1) 操作，直接调用就行：

```cpp
#include <iostream>
#include <list>
using namespace std;

template<typename E>
class MyListDeque {
private:
    list<E> lst;   // 底层用双链表

public:
    // 队头插入，O(1)
    void addFirst(const E &e) {
        lst.push_front(e);
    }

    // 队尾插入，O(1)
    void addLast(const E &e) {
        lst.push_back(e);
    }

    // 队头删除，O(1)
    E removeFirst() {
        E first = lst.front();
        lst.pop_front();
        return first;
    }

    // 队尾删除，O(1)
    E removeLast() {
        E last = lst.back();
        lst.pop_back();
        return last;
    }

    // 查看队头，O(1)
    E peekFirst() {
        return lst.front();
    }

    // 查看队尾，O(1)
    E peekLast() {
        return lst.back();
    }
};

int main() {
    MyListDeque<int> deque;
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

直接复用之前实现的 `CycleArray`，因为环形数组头尾操作都是 O(1)：

```cpp
template<typename E>
class MyArrayDeque {
private:
    CycleArray<E> arr;   // 底层用环形数组

public:
    // 队头插入，O(1)
    void addFirst(E e) {
        arr.addFirst(e);
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

```cpp
#include <deque>

deque<int> dq;
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
