# 用链表实现队列和栈

> 前置知识：队列/栈基本原理、链表基础

---

## 目录

1. [核心思路](#1-核心思路)
2. [用链表实现栈](#2-用链表实现栈)
3. [用链表实现队列](#3-用链表实现队列)
4. [对比总结](#4-对比总结)

---

## 1. 核心思路

栈和队列的所有操作都是 O(1)，而双链表的**头尾增删**也都是 O(1)，天然匹配。

所以实现非常简单：**直接调用双链表的 API 就行了。**

```
双链表的能力：
  头部插入/删除：O(1)
  尾部插入/删除：O(1)

栈需要的能力：
  同一端插入/删除：O(1)  → 用链表的尾部即可

队列需要的能力：
  一端插入、另一端删除：O(1)  → 用链表的尾部插入、头部删除即可
```

---

## 2. 用链表实现栈

**设计：把链表尾部作为栈顶。**

```
链表：head <-> [1] <-> [2] <-> [3] <-> tail
                                 ↑
                               栈顶（push/pop 都在这里）

push(4)：在尾部加入4  → [1] <-> [2] <-> [3] <-> [4]
pop()：  从尾部取出4  → [1] <-> [2] <-> [3]
peek()： 看尾部的3，不删除
```

### 完整代码

```cpp
#include <iostream>
#include <list>
using namespace std;

template<typename E>
class MyLinkedStack {
private:
    list<E> lst;   // 底层用双链表

public:
    // 向栈顶插入，O(1)
    void push(const E &e) {
        lst.push_back(e);   // 链表尾部 = 栈顶
    }

    // 从栈顶删除并返回，O(1)
    E pop() {
        E value = lst.back();  // 先取出栈顶的值
        lst.pop_back();        // 再删除
        return value;
    }

    // 查看栈顶元素（不删除），O(1)
    E peek() const {
        return lst.back();
    }

    // 返回元素个数，O(1)
    int size() const {
        return lst.size();
    }
};

int main() {
    MyLinkedStack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);

    while (stack.size() > 0) {
        cout << stack.pop() << endl;
    }
    // 输出：4 3 2 1（后进先出）
}
```

### 运行过程追踪

```
push(1)：链表 = [1]
push(2)：链表 = [1, 2]
push(3)：链表 = [1, 2, 3]
push(4)：链表 = [1, 2, 3, 4]

pop()：返回4，链表 = [1, 2, 3]
pop()：返回3，链表 = [1, 2]
pop()：返回2，链表 = [1]
pop()：返回1，链表 = []
```

### 也可以用头部作为栈顶

把尾部换成头部，效果完全一样，因为双链表头部操作也是 O(1)：

```cpp
void push(const E &e) { lst.push_front(e); }  // 改成头部插入
E pop()               { E v = lst.front(); lst.pop_front(); return v; }
E peek() const        { return lst.front(); }
```

---

## 3. 用链表实现队列

**设计：链表尾部作为队尾（插入），链表头部作为队头（删除）。**

```
链表：head <-> [1] <-> [2] <-> [3] <-> tail
                ↑                         ↑
              队头（出队）               队尾（入队）

push(4)：在尾部加入4  → [1] <-> [2] <-> [3] <-> [4]
pop()：  从头部取出1  → [2] <-> [3] <-> [4]
peek()： 看头部的2，不删除
```

### 完整代码

```cpp
#include <iostream>
#include <list>
using namespace std;

template<typename E>
class MyLinkedQueue {
private:
    list<E> lst;   // 底层用双链表

public:
    // 向队尾插入，O(1)
    void push(const E &e) {
        lst.push_back(e);   // 链表尾部 = 队尾
    }

    // 从队头删除并返回，O(1)
    E pop() {
        E front = lst.front();  // 先取出队头的值
        lst.pop_front();        // 再删除
        return front;
    }

    // 查看队头元素（不删除），O(1)
    E peek() const {
        return lst.front();
    }

    // 返回元素个数，O(1)
    int size() const {
        return lst.size();
    }
};

int main() {
    MyLinkedQueue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);

    cout << queue.peek() << endl;  // 1（看队头，不删除）
    cout << queue.pop()  << endl;  // 1（先进先出）
    cout << queue.pop()  << endl;  // 2
    cout << queue.peek() << endl;  // 3
}
```

### 运行过程追踪

```
push(1)：链表 = [1]
push(2)：链表 = [1, 2]
push(3)：链表 = [1, 2, 3]

peek()：看队头，返回1，链表不变 = [1, 2, 3]
pop()： 删队头，返回1，链表 = [2, 3]
pop()： 删队头，返回2，链表 = [3]
peek()：看队头，返回3，链表不变 = [3]
```

---

## 4. 对比总结

| | 栈 | 队列 |
|--|:--:|:----:|
| 规则 | 先进后出 | 先进先出 |
| push | 链表尾部插入 | 链表尾部插入 |
| pop | 链表尾部删除 | 链表**头部**删除 |
| peek | 看链表尾部 | 看链表**头部** |

**关键区别只有一点：**

```
栈：push 和 pop 都在同一端（尾部）
队列：push 在尾部，pop 在头部
```

两种实现用到的链表 API：

```cpp
lst.push_back()   // 尾部插入
lst.push_front()  // 头部插入
lst.pop_back()    // 尾部删除
lst.pop_front()   // 头部删除
lst.back()        // 查看尾部
lst.front()       // 查看头部
lst.size()        // 元素个数
```

> 📌 **一句话总结**
>
> 栈和队列本质都是双链表，只是限制了能用哪一端。
> 栈只用一端，队列用两端，全部操作都是 O(1)。
