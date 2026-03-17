# 链表代码实现

---

## 目录

1. [三个关键点](#1-三个关键点)
2. [双链表完整实现](#2-双链表完整实现)
3. [双链表核心方法详解](#3-双链表核心方法详解)
4. [单链表完整实现](#4-单链表完整实现)
5. [单链表核心方法详解](#5-单链表核心方法详解)
6. [双链表 vs 单链表对比](#6-双链表-vs-单链表对比)

---

## 1. 三个关键点

### 关键点一：同时持有头尾节点的引用

之前的链表只有头节点 `head`，如果要在尾部插入元素，需要每次从头遍历到尾，是 O(N)。

```cpp
// 没有 tail 引用：每次插入尾部都要遍历
Node* p = head;
while (p->next != nullptr) p = p->next;  // 遍历到尾部
p->next = newNode;                         // 才能插入
```

**同时持有 `tail` 引用后：尾部插入直接 O(1)**

```cpp
// 有 tail 引用：直接操作
tail->next = newNode;   // 直接插入，不需要遍历
tail = newNode;         // 更新尾节点
```

---

### 关键点二：虚拟头尾节点

这是最重要的技巧，能让代码简洁很多。

**没有虚拟节点时**，头部、尾部、中间的插入删除都要单独处理边界情况：

```cpp
// 插入时要判断：是头部？是尾部？是中间？
// 删除时要判断：链表是否为空？只剩一个节点？
// 非常容易出 bug
```

**有了虚拟头尾节点后**，链表永远不为空，操作统一：

```
空链表：dummyHead <-> dummyTail

加入 1, 2, 3 后：dummyHead <-> 1 <-> 2 <-> 3 <-> dummyTail
```

无论链表是否有元素，**插入操作永远是"在中间插入"**，删除操作也不用担心空指针。

> ⚠️ 虚拟节点是内部实现细节，对外不可见。`get(0)` 返回的是第一个真实节点，不是虚拟节点。

---

### 关键点三：删除节点后清空指针

删除节点后，把被删节点的指针置为 `nullptr`，是个好习惯：

```cpp
x->prev = nullptr;
x->next = nullptr;
delete x;
```

这样可以避免潜在的悬空指针问题。

---

## 2. 双链表完整实现

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename E>
class MyLinkedList {
private:
    // 节点定义（嵌套在类内部）
    struct Node {
        E val;
        Node *next;
        Node *prev;
        Node(E value) : val(value), next(nullptr), prev(nullptr) {}
    };

    Node *head;   // 虚拟头节点
    Node *tail;   // 虚拟尾节点
    int size;

public:
    // 构造函数：初始化虚拟头尾节点，互相连接
    MyLinkedList() {
        head = new Node(E());
        tail = new Node(E());
        head->next = tail;
        tail->prev = head;
        size = 0;
    }

    // 析构函数：释放所有节点内存
    ~MyLinkedList() {
        while (size > 0) removeFirst();
        delete head;
        delete tail;
    }

    // ==================== 增 ====================

    void addLast(E e) {
        Node *x = new Node(e);
        Node *temp = tail->prev;   // 找到最后一个真实节点
        // temp <-> tail  →  temp <-> x <-> tail
        temp->next = x;
        x->prev = temp;
        x->next = tail;
        tail->prev = x;
        size++;
    }

    void addFirst(E e) {
        Node *x = new Node(e);
        Node *temp = head->next;   // 找到第一个真实节点
        // head <-> temp  →  head <-> x <-> temp
        temp->prev = x;
        x->next = temp;
        head->next = x;
        x->prev = head;
        size++;
    }

    void add(int index, E element) {
        checkPositionIndex(index);
        if (index == size) {
            addLast(element);
            return;
        }
        Node *p = getNode(index);    // 找到 index 处的节点
        Node *temp = p->prev;
        Node *x = new Node(element);
        // temp <-> p  →  temp <-> x <-> p
        p->prev = x;
        temp->next = x;
        x->prev = temp;
        x->next = p;
        size++;
    }

    // ==================== 删 ====================

    E removeFirst() {
        if (size < 1) throw out_of_range("No elements to remove");
        Node *x = head->next;        // 第一个真实节点
        Node *temp = x->next;
        // head <-> x <-> temp  →  head <-> temp
        head->next = temp;
        temp->prev = head;
        E val = x->val;
        delete x;
        size--;
        return val;
    }

    E removeLast() {
        if (size < 1) throw out_of_range("No elements to remove");
        Node *x = tail->prev;        // 最后一个真实节点
        Node *temp = x->prev;
        // temp <-> x <-> tail  →  temp <-> tail
        tail->prev = temp;
        temp->next = tail;
        E val = x->val;
        x->prev = nullptr;
        x->next = nullptr;
        delete x;
        size--;
        return val;
    }

    E remove(int index) {
        checkElementIndex(index);
        Node *x = getNode(index);
        Node *prev = x->prev;
        Node *next = x->next;
        // prev <-> x <-> next  →  prev <-> next
        prev->next = next;
        next->prev = prev;
        E val = x->val;
        x->prev = nullptr;
        x->next = nullptr;
        delete x;
        size--;
        return val;
    }

    // ==================== 查 ====================

    E get(int index) {
        checkElementIndex(index);
        return getNode(index)->val;
    }

    E getFirst() {
        if (size < 1) throw out_of_range("No elements");
        return head->next->val;
    }

    E getLast() {
        if (size < 1) throw out_of_range("No elements");
        return tail->prev->val;
    }

    // ==================== 改 ====================

    E set(int index, E val) {
        checkElementIndex(index);
        Node *p = getNode(index);
        E oldVal = p->val;
        p->val = val;
        return oldVal;
    }

    // ==================== 工具 ====================

    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }

    void display() {
        cout << "size = " << size << "  |  ";
        for (Node *p = head->next; p != tail; p = p->next) {
            cout << p->val << " <-> ";
        }
        cout << "null" << endl;
    }

private:
    // 返回 index 处的真实节点
    Node* getNode(int index) {
        Node *p = head->next;
        for (int i = 0; i < index; i++) p = p->next;
        return p;
    }

    bool isElementIndex(int index) const {
        return index >= 0 && index < size;
    }

    bool isPositionIndex(int index) const {
        return index >= 0 && index <= size;
    }

    void checkElementIndex(int index) const {
        if (!isElementIndex(index))
            throw out_of_range("Index out of bounds");
    }

    void checkPositionIndex(int index) const {
        if (!isPositionIndex(index))
            throw out_of_range("Index out of bounds");
    }
};

int main() {
    MyLinkedList<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addFirst(0);
    list.add(2, 100);
    list.display();
    // size = 5  |  0 <-> 1 <-> 100 <-> 2 <-> 3 <-> null
    return 0;
}
```

---

## 3. 双链表核心方法详解

### 3.1 构造函数：初始化虚拟节点

```cpp
MyLinkedList() {
    head = new Node(E());   // 虚拟头节点，值无所谓
    tail = new Node(E());   // 虚拟尾节点，值无所谓
    head->next = tail;      // 头指向尾
    tail->prev = head;      // 尾指向头
    size = 0;
}
```

初始状态：

```
head <-> tail
（中间没有任何真实节点）
```

---

### 3.2 addLast — 在尾部插入

```cpp
void addLast(E e) {
    Node *x = new Node(e);
    Node *temp = tail->prev;   // temp 是当前最后一个真实节点
    temp->next = x;
    x->prev = temp;
    x->next = tail;
    tail->prev = x;
    size++;
}
```

图示（插入 3）：

```
插入前：head <-> 1 <-> 2 <-> tail
                        ↑
                       temp

插入后：head <-> 1 <-> 2 <-> 3 <-> tail
```

>
>
> 好，我用图一步一步画清楚。
>
> ---
>
> # 先搞清楚初始状态
>
> 链表里已经有 1, 2 两个节点，现在要在尾部插入 3：
>
> ```
> head <-> [1] <-> [2] <-> tail
> （head 和 tail 都是虚拟节点，不存真实数据）
> ```
>
> ---
>
> # temp 是什么？
>
> ```cpp
> Node *temp = tail->prev;
> ```
>
> `tail->prev` 就是 tail 的前一个节点，也就是 **当前最后一个真实节点** ，这里是节点2。
>
> ```
> head <-> [1] <-> [2] <-> tail
>                   ↑
>                  temp（节点2）
> ```
>
> **temp 不是虚拟节点，是最后一个真实节点。**
>
> 虚拟节点是 `head` 和 `tail`，`temp = tail->prev` 拿到的是 tail 前面那个真实节点。
>
> ---
>
> # 新节点 x 现在是孤立的
>
> ```cpp
> Node *x = new Node(e);   // 创建值为3的新节点
> ```
>
> ```
> head <-> [1] <-> [2] <-> tail
>                   ↑
>                  temp
>
> [3]   ← x，孤立的，prev和next都是nullptr
> ```
>
> ---
>
> # 步骤1：temp->next = x
>
> 让节点2的 `next` 指向新节点3：
>
> ```
> head <-> [1] <-> [2] ──→ [3]      tail
>                   ↑        ↑
>                  temp       x
> （此时 x 的 prev 还是 nullptr，2和3之间只有单向连接）
> ```
>
> ---
>
> # 步骤2：x->prev = temp
>
> 让新节点3的 `prev` 指向节点2，双向连通：
>
> ```
> head <-> [1] <-> [2] <-> [3]      tail
>                   ↑        ↑
>                  temp       x
> （2和3之间现在双向连通了，但3和tail还没连上）
> ```
>
> ---
>
> # 步骤3：x->next = tail
>
> 让新节点3的 `next` 指向虚拟尾节点：
>
> ```
> head <-> [1] <-> [2] <-> [3] ──→ tail
>                            ↑
>                            x
> （3指向tail了，但tail的prev还指着旧的节点2）
> ```
>
> ---
>
> # 步骤4：tail->prev = x
>
> 让虚拟尾节点的 `prev` 从指向节点2，改为指向新节点3：
>
> ```
> head <-> [1] <-> [2] <-> [3] <-> tail
>                            ↑
>                            x
> ✅ 完成！
> ```
>
>    
> ---
>
> # addFirst 同理
>
> 在头部插入0，`temp = head->next`（第一个真实节点）：
>
> ```
> 插入前：head <-> [1] <-> [2] <-> tail
>                   ↑
>                  temp（节点1）
>
> 插入后：head <-> [0] <-> [1] <-> [2] <-> tail
> ```
>
> 代码：
>
> ```cpp
> void addFirst(E e) {
>     Node *x = new Node(e);
>     Node *temp = head->next;   // temp = 节点1（第一个真实节点）
>     // head <-> temp
>     temp->prev = x;    // 节点1的prev指向新节点
>     x->next = temp;    // 新节点的next指向节点1
>     head->next = x;    // 虚拟头节点的next指向新节点
>     x->prev = head;    // 新节点的prev指向虚拟头节点
>     // head <-> x <-> temp
>     size++;
> }
> ```
>
> ---
>
> # 一句话总结
>
> ```
> addLast：
>   temp = tail->prev（最后一个真实节点）
>   在 temp 和 tail 之间插入新节点 x
>
> addFirst：
>   temp = head->next（第一个真实节点）
>   在 head 和 temp 之间插入新节点 x
>
> 两个操作本质完全一样：都是在两个已知节点之间插入新节点
> 虚拟节点的好处就在这里：head和tail永远存在，不用判断空链表
> ```
>
>>
>> 好问题，这是理解指针操作最关键的一步。
>>
>> ---
>>
>> # 等号永远从右往左读
>>
>> ```cpp
>> tail->prev = x;
>> ```
>>
>> 读法：**把 x 的值，存进 tail->prev 这个格子里。**
>>
>> ---
>>
>> # 用格子模型理解
>>
>> 每个节点就是一排格子，每个格子存一个值：
>>
>> ```
>> tail 节点长这样：
>> ┌──────┬─────┬──────┐
>> │ prev │  E  │ next │
>> └──────┴─────┴──────┘
>> ```
>>
>> `tail->prev` 就是 tail 这个节点的 `prev` 格子。
>>
>> 执行 `tail->prev = x` 就是： **把 x 的地址写进这个格子** 。
>>
>> ```
>> 执行前：
>> tail: [ 节点2的地址 | _ | nullptr ]
>>          ↑ prev格子存的是节点2的地址
>>
>> 执行后：
>> tail: [ 节点3的地址 | _ | nullptr ]
>>          ↑ prev格子现在存的是节点3的地址
>> ```
>>
>> ---
>>
>> # 反过来 x = tail->prev 是什么意思？
>>
>> ```cpp
>> x = tail->prev;   // 把 tail->prev 的值，存进 x
>> ```
>>
>> 意思是：**把 tail 的 prev 格子里存的地址，复制给 x。**
>>
>> 也就是让 x 这个指针指向 tail 的前一个节点。
>>
>> ---
>>
>> # 两者的本质区别
>>
>> ```cpp
>> tail->prev = x;
>> // 修改 tail 节点内部的 prev 格子
>> // tail 这个节点被改变了
>>
>> x = tail->prev;
>> // 把 tail->prev 的值读出来，赋给 x
>> // tail 这个节点没有任何变化，只是读了一下
>> ```
>>
>> 类比：
>>
>> ```
>> tail->prev = x    →  往格子里写东西（修改）
>> x = tail->prev    →  从格子里读东西（读取）
>> ```
>>
>> ---
>>
>> # 回到原代码
>>
>> ```cpp
>> Node *temp = tail->prev;   // 读：把 tail->prev 读出来存进 temp
>> temp->next = x;            // 写：修改 temp 节点的 next 格子
>> x->prev = temp;            // 写：修改 x 节点的 prev 格子
>> x->next = tail;            // 写：修改 x 节点的 next 格子
>> tail->prev = x;            // 写：修改 tail 节点的 prev 格子
>> ```
>>
>> 第一行是 **读** ，后四行都是 **写** 。
>>
>> ---
>>
>> # 一句话总结
>>
>> ```
>> 等号左边  →  要修改哪个格子
>> 等号右边  →  往那个格子里写什么值
>>
>> tail->prev = x    修改 tail 的 prev 格子，写入 x 的地址
>> x = tail->prev    把 tail 的 prev 格子里的值读出来给 x
>> ```
>>
>> 反过来写意思完全不同，一个是修改节点，一个是读取值。
>>

---

### 3.3 addFirst — 在头部插入

```cpp
void addFirst(E e) {
    Node *x = new Node(e);
    Node *temp = head->next;   // temp 是当前第一个真实节点
    temp->prev = x;
    x->next = temp;
    head->next = x;
    x->prev = head;
    size++;
}
```

图示（插入 0）：

```
插入前：head <-> 1 <-> 2 <-> tail
                  ↑
                 temp

插入后：head <-> 0 <-> 1 <-> 2 <-> tail
```

---

### 3.4 虚拟节点如何简化操作

没有虚拟节点时，在空链表头部插入需要特殊处理：

```cpp
// 没有虚拟节点：要判断链表是否为空
if (head == nullptr) {
    head = newNode;       // 特殊情况1
} else {
    newNode->next = head;
    head->prev = newNode;
    head = newNode;       // 普通情况
}
```

有了虚拟节点，**不管链表是否为空，操作完全一样**：

```cpp
// 有虚拟节点：永远只有一种情况
Node *temp = head->next;  // 空链表时 temp 就是 tail，不是 nullptr
temp->prev = x;
x->next = temp;
head->next = x;
x->prev = head;
```

>
>
> 好，我分别演示两种情况，你就明白区别了。
>
> ---
>
> # 场景一：没有虚拟节点，往空链表插入
>
> ## 没有虚拟节点时，空链表长这样：
>
> ```
> head = nullptr
> （什么都没有）
> ```
>
> ## 现在插入节点1：
>
> 如果直接用普通代码：
>
> ```cpp
> Node *temp = head->next;  // ❌ head是nullptr，nullptr->next 直接崩溃！
> ```
>
> 所以必须先判断：
>
> ```cpp
> if (head == nullptr) {
>     // 特殊处理：链表为空
>     head = newNode;
> } else {
>     // 普通处理：链表不为空
>     newNode->next = head;
>     head->prev = newNode;
>     head = newNode;
> }
> ```
>
> 两套代码，两种逻辑，容易出bug。
>
> ---
>
> # 场景二：有虚拟节点，往空链表插入
>
> ## 有虚拟节点时，空链表长这样：
>
> ```
> head <-> tail
> （head和tail是虚拟节点，永远存在）
> ```
>
> ## 现在插入节点1：
>
> ```cpp
> Node *temp = head->next;  // ✅ head->next 是 tail，不是nullptr，安全！
> ```
>
> ```
> 此时：
> head <-> tail
>           ↑
>          temp（temp就是tail，但不是nullptr，可以安全操作）
> ```
>
> 继续执行：
>
> ```cpp
> temp->prev = x;   // tail的prev指向新节点
> x->next = temp;   // 新节点的next指向tail
> head->next = x;   // head的next指向新节点
> x->prev = head;   // 新节点的prev指向head
> ```
>
> 一步步图示：
>
> ```
> 初始：head <-> tail
>                ↑ temp
>
> step1: temp->prev = x
> head    tail
>   ↑      ↑
>   |   x←─┘（tail的prev指向x）
>
> step2: x->next = temp
> head    [x]──→tail
>                ↑ temp
>
> step3: head->next = x
> head──→[x]──→tail
>
> step4: x->prev = head
> head←→[x]──→tail
>
> 完成：head <-> [1] <-> tail  ✅
> ```
>
> **和非空链表的操作完全一样，不需要任何特判！**
>
> ---
>
> # 对比：插入第二个节点
>
> 两种方式插入节点2（此时链表已有节点1）：
>
> **没有虚拟节点：**
>
> ```cpp
> // 链表不为空，走else分支
> newNode->next = head;    // head此时是节点1
> head->prev = newNode;
> head = newNode;
> ```
>
> **有虚拟节点：**
>
> ```cpp
> Node *temp = head->next;  // temp是节点1
> temp->prev = x;
> x->next = temp;
> head->next = x;
> x->prev = head;
> ```
>
> **代码和插入第一个节点时完全一样！**
>
> ---
>
> # 本质区别一句话
>
> ```
> 没有虚拟节点：
>   空链表时 head = nullptr
>   nullptr->next 会崩溃
>   所以必须单独处理空链表的情况
>
> 有虚拟节点：
>   空链表时 head->next = tail（不是nullptr）
>   任何时候都可以安全执行 head->next
>   永远只有一种情况，代码统一
> ```
>
> 虚拟节点就是给链表装了一个"保险"，保证链表永远不会真正为空，所有操作都能统一处理。

>
>
> 好，退回最基础的地方，重新捋清楚。
>
> ---
>
> # 第一步：-> 是什么
>
> `->` 就是 **通过指针访问节点里面的格子** 。
>
> 节点长这样，里面有三个格子：
>
> ```
> ┌──────┬─────┬──────┐
> │ prev │ val │ next │
> └──────┴─────┴──────┘
> ```
>
> 假设 `p` 是一个指向节点的指针：
>
> ```cpp
> p->val    // 访问 p 指向的节点的 val 格子
> p->next   // 访问 p 指向的节点的 next 格子
> p->prev   // 访问 p 指向的节点的 prev 格子
> ```
>
> ---
>
> # 第二步：指针变量是什么
>
> `head`、`tail`、`temp`、`x` 这些都是 **指针变量** ，存的是某个节点的地址。
>
> ```cpp
> Node *head;   // head 存的是某个节点的地址
> Node *tail;   // tail 存的是某个节点的地址
> Node *temp;   // temp 存的是某个节点的地址
> Node *x;      // x   存的是某个节点的地址
> ```
>
> 你可以把它们想象成 **便利贴** ，便利贴上写着某个节点的门牌号。
>
> ---
>
> # 第三步：具体到双链表
>
> 链表有 1, 2 两个节点时，内存里长这样：
>
> ```
> head便利贴        节点1              节点2          tail便利贴
> ┌──────┐    ┌────┬───┬────┐    ┌────┬───┬────┐    ┌──────┐
> │0x100 │    │    │ 1 │    │    │    │ 2 │    │    │0x300 │
> └──────┘    └────┴───┴────┘    └────┴───┴────┘    └──────┘
>                ↑                    ↑
>              0x100                0x200
> ```
>
> 填上指针：
>
> ```
> head → [0x100|虚拟|0x200]
>               节点1：[0x100| 1 |0x200]   prev存head地址，next存节点2地址
>               节点2：[0x100| 2 |0x300]   prev存节点1地址，next存tail地址
>               tail：[0x200|虚拟|nullptr]
> ```
>
> 所以：
>
> ```cpp
> head->next        // head节点的next格子，存的是节点1的地址(0x200)
> head->next->val   // 节点1的val格子，值是1
> tail->prev        // tail节点的prev格子，存的是节点2的地址
> ```
>
> ---
>
> # 第四步：重新看 addLast
>
> ```cpp
> void addLast(E e) {
>     Node *x = new Node(e);       // 创建新节点，x是指向它的便利贴
>     Node *temp = tail->prev;     // temp便利贴 = tail的prev格子里的地址
>                                  // 也就是最后一个真实节点（节点2）
>     temp->next = x;              // 节点2的next格子 写入 x的地址
>     x->prev = temp;              // 新节点的prev格子 写入 temp的地址（节点2）
>     x->next = tail;              // 新节点的next格子 写入 tail的地址
>     tail->prev = x;              // tail的prev格子 写入 x的地址
>     size++;
> }
> ```
>
> 用便利贴图示走一遍（插入节点3）：
>
> ```
> 初始：
> head ──→ [虚拟] ←→ [1] ←→ [2] ←→ [虚拟] ←── tail
>                                     ↑
>                               tail->prev就是节点2
>
> 第一行：Node *x = new Node(3)
> 新建节点3，x是指向它的便利贴：
> x ──→ [null | 3 | null]
>
> 第二行：Node *temp = tail->prev
> temp便利贴贴在节点2上：
> temp ──→ [节点1地址 | 2 | tail地址]
>
> 第三行：temp->next = x
> 把x的地址写进节点2的next格子：
> [节点1地址 | 2 | x地址]   [null | 3 | null]
>                 ↓________________↑
>
> 第四行：x->prev = temp
> 把temp的地址写进节点3的prev格子：
> [节点1地址 | 2 | x地址] ←→ [temp地址 | 3 | null]
>
> 第五行：x->next = tail
> 把tail的地址写进节点3的next格子：
> [2] ←→ [节点2地址 | 3 | tail地址] ──→ [虚拟tail]
>
> 第六行：tail->prev = x
> 把x的地址写进tail的prev格子：
> [2] ←→ [3] ←→ [虚拟tail]
>
> 最终：
> head ──→ [虚拟] ←→ [1] ←→ [2] ←→ [3] ←→ [虚拟] ←── tail  ✅
> ```
>
> ---
>
> # 一句话总结
>
> ```
> head、tail、temp、x 都是便利贴（指针），存的是节点的地址
>
> p->next   读写 p 指向的节点的 next 格子
> p->prev   读写 p 指向的节点的 prev 格子
> p->val    读写 p 指向的节点的 val 格子
>
> 等号左边  →  要改哪个格子
> 等号右边  →  往那个格子写什么
> ```
>
> 清楚了吗？

>
>
> 好问题，这个搞清楚了整个链表就全通了。
>
> ---
>
> # 虚拟节点长什么样？
>
> 虚拟节点和普通节点 **结构完全一样** ，都有三个格子：
>
> ```
> ┌──────┬─────┬──────┐
> │ prev │ val │ next │
> └──────┴─────┴──────┘
> ```
>
> 唯一区别是：**val 格子里存的是没意义的默认值，我们永远不会去读它。**
>
> ```cpp
> head = new Node(E());   // E() 就是默认值，int的话就是0
> tail = new Node(E());
> ```
>
> ---
>
> # 初始化时发生了什么？
>
> ```cpp
> head = new Node(E());
> tail = new Node(E());
> head->next = tail;      // head的next格子 写入 tail的地址
> tail->prev = head;      // tail的prev格子 写入 head的地址
> ```
>
> 图示：
>
> ```
> head便利贴         虚拟head节点              虚拟tail节点        tail便利贴
> ┌──────┐      ┌──────┬───┬──────┐      ┌──────┬───┬──────┐      ┌──────┐
> │地址A │─────→│nullptr│ 0 │地址B │←────→│地址A │ 0 │nullptr│←─────│地址B │
> └──────┘      └──────┴───┴──────┘      └──────┴───┴──────┘      └──────┘
>                    ↑                         ↑
>                  地址A                      地址B
> ```
>
> 用箭头简化：
>
> ```
> head ──→ [虚拟head] ←→ [虚拟tail] ←── tail
> ```
>
> ---
>
> # 加入节点1之后
>
> ```
> head ──→ [虚拟head] ←→ [1] ←→ [虚拟tail] ←── tail
> ```
>
> 此时各个格子的内容：
>
> ```
> 虚拟head：prev=nullptr，val=0，next=节点1的地址
> 节点1：   prev=虚拟head的地址，val=1，next=虚拟tail的地址
> 虚拟tail：prev=节点1的地址，val=0，next=nullptr
> ```
>
> ---
>
> # 回答你的问题：tail->prev 为什么是节点2，不是虚拟尾节点？
>
> `tail` 是便利贴，贴在**虚拟尾节点**上。
>
> `tail->prev` 是 **虚拟尾节点里面的 prev 格子** ，这个格子存的是"我前面那个节点的地址"。
>
> ```
> 链表：[虚拟head] ←→ [1] ←→ [2] ←→ [虚拟tail]
>                                          ↑
>                                         tail 便利贴贴在这里
>
> 虚拟tail节点里面：
> ┌──────────┬───┬──────────┐
> │节点2的地址│ 0 │ nullptr  │
> └──────────┴───┴──────────┘
>      ↑
>   tail->prev = 节点2的地址
> ```
>
> `tail->prev` 访问的是 **虚拟尾节点内部的 prev 格子** ，这个格子里存的是节点2的地址，所以 `tail->prev` 就是节点2，不是虚拟尾节点自己。
>
> ---
>
> # 一句话总结
>
> ```
> tail          →  便利贴，贴在虚拟尾节点上
> tail->prev    →  虚拟尾节点内部的prev格子，存的是"我前面那个节点"的地址
>
> 虚拟节点和普通节点结构完全一样
> 唯一区别是 val 没有意义，我们只用它的 prev 和 next 格子
> ```
>

---

### 3.5 remove — 删除指定位置

```cpp
E remove(int index) {
    Node *x = getNode(index);   // 找到要删除的节点
    Node *prev = x->prev;
    Node *next = x->next;
    // 让前后节点直接相连，跳过 x
    prev->next = next;
    next->prev = prev;
    // 清理 x 的指针，释放内存
    E val = x->val;
    x->prev = nullptr;
    x->next = nullptr;
    delete x;
    size--;
    return val;
}
```

图示（删除索引1的节点）：

```
删除前：head <-> 0 <-> 1 <-> 2 <-> tail
                        ↑
                        x（要删除）

删除后：head <-> 0 <-> 2 <-> tail
              （1 被彻底摘除，内存释放）
```

>
>
> 好，我换一个更直觉的方式来解释，忘掉便利贴。
>
> ---
>
> # 重新理解指针
>
> **指针就是一个变量，存的是地址，仅此而已。**
>
> ```cpp
> Node *x = getNode(1);
> ```
>
> `x` 就是一个普通变量，只不过它存的是一个地址，通过这个地址能找到那个节点。
>
> ---
>
> # 用现实类比
>
> 把每个节点想象成一栋房子，房子里有三个房间：
>
> ```
> ┌──────┬─────┬──────┐
> │ prev │ val │ next │
> └──────┴─────┴──────┘
> ```
>
> * `prev` 房间：存左边邻居家的门牌号
> * `val` 房间：存这栋房子主人的名字
> * `next` 房间：存右边邻居家的门牌号
>
> **指针变量 = 一张写着某栋房子门牌号的纸**
>
> ```cpp
> Node *x;    // x 是一张纸，上面写着某栋房子的门牌号
> x->prev     // 去 x 指向的那栋房子，进 prev 房间，看里面写的是什么
> x->next     // 去 x 指向的那栋房子，进 next 房间，看里面写的是什么
> ```
>
> ---
>
> # 现在看 remove 代码
>
> 链表：`head <-> [0] <-> [1] <-> [2] <-> tail`
>
> 要删除索引1，也就是节点1。
>
> ```
>      prev房间  val房间  next房间
> [0]: [head]    [ 0 ]   [节点1]
> [1]: [节点0]   [ 1 ]   [节点2]   ← 要删除这个
> [2]: [节点1]   [ 2 ]   [tail]
> ```
>
> ---
>
> **第2行：`Node *x = getNode(1)`**
>
> x 这张纸上写的是节点1的门牌号，通过 x 能找到节点1。
>
> ```
> x 指向节点1：
>      prev房间  val房间  next房间
> [1]: [节点0]   [ 1 ]   [节点2]
>  ↑
>  x
> ```
>
> ---
>
> **第3行：`Node *prev = x->prev`**
>
> 去节点1的 prev 房间，看里面写的是什么（节点0的门牌号），把这个门牌号抄到 prev 这张纸上。
>
> ```
> prev 指向节点0：
>      prev房间  val房间  next房间
> [0]: [head]    [ 0 ]   [节点1]
>  ↑
> prev
> ```
>
> ---
>
> **第4行：`Node *next = x->next`**
>
> 去节点1的 next 房间，看里面写的是什么（节点2的门牌号），把这个门牌号抄到 next 这张纸上。
>
> ```
> next 指向节点2：
>      prev房间  val房间  next房间
> [2]: [节点1]   [ 2 ]   [tail]
>  ↑
> next
> ```
>
> ---
>
> 现在三张纸的位置：
>
> ```
> head <-> [0] <-> [1] <-> [2] <-> tail
>           ↑       ↑       ↑
>          prev     x      next
> ```
>
> ---
>
> **第6行：`prev->next = next`**
>
> 去 prev 指向的房子（节点0），进它的 next 房间，把里面的内容从"节点1的门牌号"改成"节点2的门牌号"。
>
> ```
> 改之前，节点0的 next 房间：[节点1的门牌号]
> 改之后，节点0的 next 房间：[节点2的门牌号]
> ```
>
> 效果：节点0现在的右边邻居直接变成了节点2，跳过了节点1。
>
> ---
>
> **第7行：`next->prev = prev`**
>
> 去 next 指向的房子（节点2），进它的 prev 房间，把里面的内容从"节点1的门牌号"改成"节点0的门牌号"。
>
> ```
> 改之前，节点2的 prev 房间：[节点1的门牌号]
> 改之后，节点2的 prev 房间：[节点0的门牌号]
> ```
>
> 效果：节点2现在的左边邻居直接变成了节点0，跳过了节点1。
>
> ---
>
> **现在节点1已经被孤立了：**
>
> ```
> head <-> [0] <-> [2] <-> tail
> （节点1没有任何人指向它了，但它还在内存里）
> ```
>
> ---
>
> **第10-12行：清理节点1，释放内存**
>
> ```cpp
> x->prev = nullptr;   // 清空节点1的prev房间
> x->next = nullptr;   // 清空节点1的next房间
> delete x;            // 把节点1这栋房子彻底拆掉
> ```
>
> ---
>
> # 整个过程总结
>
> ```
> x    = 找到要删的节点1
> prev = 节点1左边的邻居（节点0）
> next = 节点1右边的邻居（节点2）
>
> prev->next = next   节点0说：我的右边邻居改成节点2
> next->prev = prev   节点2说：我的左边邻居改成节点0
>
> 节点1从此无人认识，删除释放内存
> ```
>
> 本质就是：**让节点1的左右邻居直接认识对方，把节点1从中间踢出去。**
>
> 清楚了吗？

>
>
> 可以互换，这两行顺序无所谓。
>
> ---
>
> # 为什么可以互换？
>
> 因为这两行操作的是 **不同的房子** ：
>
> ```cpp
> prev->next = next;   // 修改节点0的 next 房间
> next->prev = prev;   // 修改节点2的 prev 房间
> ```
>
> 一个改的是节点0，一个改的是节点2，互不干扰，谁先谁后都没影响。
>
> ---
>
> # 对比插入操作（顺序不能换）
>
> 你可能想到之前插入时有顺序要求：
>
> ```cpp
> // 插入时，这两行不能互换
> p->next->prev = newNode;  // 步骤3
> p->next = newNode;        // 步骤4：必须最后做
> ```
>
> 步骤4必须最后做，是因为步骤3用到了 `p->next`，如果步骤4先执行，`p->next` 就变了，步骤3就找不到原来的节点了。
>
> ---
>
> # 一句话总结
>
> ```
> 两行操作的是同一个变量  →  顺序有影响，要小心
> 两行操作的是不同变量    →  顺序无所谓，随便换
> ```
>
> 删除时 `prev->next` 和 `next->prev` 操作的是完全不同的两个节点，所以随便换。

---

### 3.6 getNode — 按索引找节点

```cpp
Node* getNode(int index) {
    Node *p = head->next;   // 从第一个真实节点开始（跳过虚拟头节点）
    for (int i = 0; i < index; i++) {
        p = p->next;
    }
    return p;
}
```

> 💡 优化方向：判断 index 靠近头部还是尾部，决定从 head 还是 tail 开始遍历，最坏情况从 O(N) 降到 O(N/2)。

---

## 4. 单链表完整实现

单链表只有 `next` 指针，没有 `prev`，所以只用了**虚拟头节点**（虚拟尾节点没有作用）。

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename E>
class MyLinkedList2 {
private:
    struct Node {
        E val;
        Node *next;
        Node(E value) : val(value), next(nullptr) {}
    };

    Node *head;   // 虚拟头节点
    Node *tail;   // 真实尾节点的引用（不是虚拟的）
    int size_;

public:
    MyLinkedList2() {
        head = new Node(E());
        tail = head;    // 初始时尾节点就是虚拟头节点
        size_ = 0;
    }

    ~MyLinkedList2() {
        Node *current = head;
        while (current != nullptr) {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }

    // ==================== 增 ====================

    void addFirst(E e) {
        Node *newNode = new Node(e);
        newNode->next = head->next;
        head->next = newNode;
        if (size_ == 0) tail = newNode;  // 第一个元素，同时更新 tail
        size_++;
    }

    void addLast(E e) {
        Node *newNode = new Node(e);
        tail->next = newNode;   // 直接接在 tail 后面，O(1)
        tail = newNode;         // 更新 tail
        size_++;
    }

    void add(int index, E element) {
        checkPositionIndex(index);
        if (index == size_) {
            addLast(element);
            return;
        }
        // 找到 index 的前驱节点（从虚拟头节点开始）
        Node *prev = head;
        for (int i = 0; i < index; i++) prev = prev->next;
        Node *newNode = new Node(element);
        newNode->next = prev->next;
        prev->next = newNode;
        size_++;
    }

    // ==================== 删 ====================

    E removeFirst() {
        if (isEmpty()) throw out_of_range("No elements to remove");
        Node *first = head->next;
        head->next = first->next;
        if (size_ == 1) tail = head;  // 删完了，tail 退回虚拟头节点
        E val = first->val;
        delete first;
        size_--;
        return val;
    }

    E removeLast() {
        if (isEmpty()) throw out_of_range("No elements to remove");
        // 找到倒数第二个节点（tail 的前驱）
        Node *prev = head;
        while (prev->next != tail) prev = prev->next;
        E val = tail->val;
        delete tail;
        prev->next = nullptr;
        tail = prev;   // 更新 tail
        size_--;
        return val;
    }

    E remove(int index) {
        checkElementIndex(index);
        // 找到 index 的前驱节点
        Node *prev = head;
        for (int i = 0; i < index; i++) prev = prev->next;
        Node *toRemove = prev->next;
        prev->next = toRemove->next;
        if (index == size_ - 1) tail = prev;  // 删的是尾节点，更新 tail
        E val = toRemove->val;
        delete toRemove;
        size_--;
        return val;
    }

    // ==================== 查 ====================

    E getFirst() {
        if (isEmpty()) throw out_of_range("No elements");
        return head->next->val;
    }

    E getLast() {
        if (isEmpty()) throw out_of_range("No elements");
        return tail->val;
    }

    E get(int index) {
        checkElementIndex(index);
        return getNode(index)->val;
    }

    // ==================== 改 ====================

    E set(int index, E element) {
        checkElementIndex(index);
        Node *p = getNode(index);
        E oldVal = p->val;
        p->val = element;
        return oldVal;
    }

    int size() { return size_; }
    bool isEmpty() { return size_ == 0; }

private:
    Node* getNode(int index) {
        Node *p = head->next;
        for (int i = 0; i < index; i++) p = p->next;
        return p;
    }

    bool isElementIndex(int index) {
        return index >= 0 && index < size_;
    }

    bool isPositionIndex(int index) {
        return index >= 0 && index <= size_;
    }

    void checkElementIndex(int index) {
        if (!isElementIndex(index))
            throw out_of_range("Index out of bounds");
    }

    void checkPositionIndex(int index) {
        if (!isPositionIndex(index))
            throw out_of_range("Index out of bounds");
    }
};

int main() {
    MyLinkedList2<int> list;
    list.addFirst(1);
    list.addFirst(2);
    list.addLast(3);
    list.addLast(4);
    list.add(2, 5);

    cout << list.removeFirst() << endl;  // 2
    cout << list.removeLast() << endl;   // 4
    cout << list.remove(1) << endl;      // 5

    cout << list.getFirst() << endl;     // 1
    cout << list.getLast() << endl;      // 3
    cout << list.get(1) << endl;         // 3
    return 0;
}
```

---

## 5. 单链表核心方法详解

### 5.1 虚拟头节点的作用

单链表只用虚拟头节点（不用虚拟尾节点）。

好处是：在头部插入/删除时，不需要特判 `head == nullptr`：

```cpp
// 没有虚拟头节点：在头部插入要特判
if (head == nullptr) {
    head = newNode;          // 链表为空的情况
} else {
    newNode->next = head;
    head = newNode;          // 链表不为空的情况
}

// 有虚拟头节点：永远只有一种情况
newNode->next = head->next;
head->next = newNode;        // 统一处理，不需要特判
```

---

### 5.2 单链表的 tail 引用如何维护

单链表持有 `tail` 引用，但每次删除尾节点后 `tail` 就失效了，需要重新找：

```cpp
E removeLast() {
    // 找倒数第二个节点（顺便就是新的 tail）
    Node *prev = head;
    while (prev->next != tail) prev = prev->next;

    E val = tail->val;
    delete tail;
    prev->next = nullptr;
    tail = prev;   // 顺便更新 tail，一举两得
    size_--;
    return val;
}
```

所以删除尾节点时，**遍历找前驱节点的同时，顺便更新 tail**，不需要额外操作。

---

### 5.3 单链表 add — 中间插入

单链表只有 `next`，插入时只需要操作**前驱节点**：

```cpp
// 找到前驱节点 prev（注意：从虚拟头节点开始！）
Node *prev = head;
for (int i = 0; i < index; i++) prev = prev->next;

Node *newNode = new Node(element);
newNode->next = prev->next;  // 步骤1：新节点接上后面
prev->next = newNode;        // 步骤2：前驱接上新节点
```

图示（在 index=1 处插入 5）：

```
插入前：head → [1] → [2] → [3] → null
               ↑
              prev（从虚拟头走1步，指向索引0）

步骤1：newNode->next = prev->next
       head → [1] → [2] → [3]
               ↑
              prev   [5] → [2]

步骤2：prev->next = newNode
       head → [1] → [5] → [2] → [3]  ✅
```

---

## 6. 双链表 vs 单链表对比

| 操作       |           双链表           |      单链表      |
| ---------- | :------------------------: | :---------------: |
| 头部插入   |            O(1)            |       O(1)       |
| 尾部插入   |            O(1)            | O(1)（持有 tail） |
| 中间插入   |            O(N)            |       O(N)       |
| 头部删除   |            O(1)            |       O(1)       |
| 尾部删除   |     O(1)（持有 tail）     | O(N)（要找前驱） |
| 中间删除   |      O(1)（已知节点）      | O(N)（要找前驱） |
| 按索引查找 |            O(N)            |       O(N)       |
| 代码复杂度 |   稍复杂（维护2个指针）   |       简单       |
| 标准库使用 | **主流**（C++ list） |       较少       |

**关键差异：**

```
双链表尾部删除是 O(1)：有 prev，直接找到前驱
单链表尾部删除是 O(N)：没有 prev，只能从头遍历

双链表已知节点时删除是 O(1)：直接操作前后指针
单链表已知节点时删除是 O(N)：还要找前驱节点
```

---

> 📌 **本章核心总结**
>
> ```
> 虚拟头尾节点  → 消除边界情况，代码统一
> 持有 tail 引用 → 尾部操作从 O(N) 变 O(1)
> 删除后清空指针 → 好习惯，避免悬空指针
>
> 双链表 = 功能更强，但代码更复杂（多维护 prev）
> 单链表 = 代码简单，但尾部删除需要遍历
> ```
