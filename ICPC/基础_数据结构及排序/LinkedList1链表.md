# 链表（链式存储）基本原理

---

## 目录

1. [为什么需要链表](#1-为什么需要链表)
2. [链表节点的结构](#2-链表节点的结构)
3. [单链表的基本操作](#3-单链表的基本操作)
4. [双链表的基本操作](#4-双链表的基本操作)
5. [时间复杂度总结](#5-时间复杂度总结)
6. [数组 vs 链表对比](#6-数组-vs-链表对比)

---

## 1. 为什么需要链表

数组的本质是**一块连续的内存空间**，这带来了随机访问的超能力，但也带来了两个缺陷：

- 中间插入/删除需要搬移数据，O(N)
- 扩容需要重新申请内存并复制数据，O(N)

链表解决了这两个问题：

**链表的元素可以分散在内存的任意位置**，通过每个节点上的 `next`、`prev` 指针把零散的内存块串联成链式结构。

```
数组（连续内存）：
[1][2][3][4][5]   ← 必须挨在一起

链表（分散内存）：
[1] → [2] → [3] → [4] → [5]
 ↑     ↑     ↑     ↑     ↑
各节点可以在内存的任意位置
```

**链表的优势：**

- 节点随用随 new，不需要一整块连续内存
- 插入/删除只需改指针，不需要搬移数据
- 理论上没有容量限制，不需要扩缩容

**链表的劣势：**

- 不支持随机访问，查找第 i 个元素只能从头遍历，O(N)

---

## 2. 链表节点的结构

### 单链表节点

```cpp
class ListNode {
public:
    int val;          // 存储的值
    ListNode *next;   // 指向下一个节点的指针

    ListNode(int x) : val(x), next(NULL) {}
};
```

>
>
> 好，从头讲清楚。
>
> ---
>
> # 1. class 的语法回顾
>
> ```cpp
> class ListNode {
> public:
>     int val;           // 成员变量：存储节点的值
>     ListNode *next;    // 成员变量：指向下一个节点的指针
>
>     ListNode(int x) : val(x), next(NULL) {}
>     // ↑ 构造函数
> };
> // ↑ 别忘了分号！
> ```
>
> 这里有个新语法：**构造函数初始化列表**
>
> ```cpp
> ListNode(int x) : val(x), next(NULL) {}
> ```
>
> 这是C++特有的写法，等价于：
>
> ```cpp
> ListNode(int x) {
>     val = x;
>     next = NULL;
> }
> ```
>
> 冒号后面的 `val(x), next(NULL)` 就是"在构造时直接初始化成员变量"，两种写法效果完全一样，只是第一种更简洁，竞赛中常见。
>
> ---
>
> # 2. 怎么创建节点
>
> ```cpp
> // 创建一个值为 1 的节点
> ListNode *node = new ListNode(1);
> //         ↑              ↑
> //     指针变量        调用构造函数，传入 x=1
>
> // 访问成员
> cout << node->val << endl;   // 1
> cout << node->next << endl;  // NULL（还没连接任何节点）
> ```
>
> ---
>
> # 3. 手动串成一条链表
>
> ```cpp
> // 创建三个节点
> ListNode *a = new ListNode(1);
> ListNode *b = new ListNode(2);
> ListNode *c = new ListNode(3);
>
> // 把它们串起来
> a->next = b;
> b->next = c;
> // c->next 默认是 NULL，链表结束
>
> // 现在：a -> b -> c -> NULL
> //       1    2    3
> ```
>
> ---
>
> # 4. 遍历这条链表
>
> ```cpp
> // 从头节点 a 开始，顺着 next 走
> for (ListNode *p = a; p != nullptr; p = p->next) {
>     cout << p->val << " ";
> }
> // 输出：1 2 3
> ```
>
> ---
>
> # 5. 完整可运行的代码
>
> ```cpp
> #include <iostream>
> using namespace std;
>
> class ListNode {
> public:
>     int val;
>     ListNode *next;
>     ListNode(int x) : val(x), next(NULL) {}
> };
>
> int main() {
>     // 创建节点
>     ListNode *a = new ListNode(1);
>     ListNode *b = new ListNode(2);
>     ListNode *c = new ListNode(3);
>
>     // 串成链表
>     a->next = b;
>     b->next = c;
>
>     // 遍历打印
>     for (ListNode *p = a; p != nullptr; p = p->next) {
>         cout << p->val << " ";
>     }
>     // 输出：1 2 3
>
>     return 0;
> }
> ```
>
> ---
>
> # 一句话总结
>
> ```
> new ListNode(x)  → 创建一个值为 x 的节点，返回它的地址
> node->val        → 访问节点的值
> node->next       → 访问节点的下一个节点
> node->next = xx  → 让节点指向 xx
> ```
>

结构图示：

```
  ┌─────┬──────┐     ┌─────┬──────┐     ┌─────┬──────┐
  │  1  │  ──────→   │  2  │  ──────→   │  3  │ NULL │
  └─────┴──────┘     └─────┴──────┘     └─────┴──────┘
   val   next          val   next          val   next
```

### 双链表节点

```cpp
class DoublyListNode {
public:
    int val;
    DoublyListNode *next;   // 指向下一个节点
    DoublyListNode *prev;   // 指向前一个节点

    DoublyListNode(int x) : val(x), next(NULL), prev(NULL) {}
};
```

结构图示：

```
       ┌──────┬─────┬──────┐
NULL ← │ prev │  1  │ next │ ←→ [2] ←→ [3]
       └──────┴─────┴──────┘
```

### 单链表 vs 双链表

|            |     单链表     |                   双链表                   |
| ---------- | :------------: | :-----------------------------------------: |
| 指针数量   |  1个（next）  |             2个（next + prev）             |
| 遍历方向   |    只能向后    |               可以向前和向后               |
| 删除节点   | 需要找前驱节点 |                直接操作自身                |
| 代码复杂度 |      简单      |                   稍复杂                   |
| 标准库使用 |      较少      | **主流**（Java LinkedList、C++ list） |

### 创建链表的工具函数

```cpp
// 单链表
ListNode* createLinkedList(vector<int> arr) {
    if (arr.empty()) return nullptr;
    ListNode* head = new ListNode(arr[0]);
    ListNode* cur = head;
    for (int i = 1; i < arr.size(); i++) {
        cur->next = new ListNode(arr[i]);
        cur = cur->next;
    }
    return head;
}

// 双链表
DoublyListNode* createDoublyLinkedList(const vector<int>& arr) {
    if (arr.empty()) return NULL;
    DoublyListNode* head = new DoublyListNode(arr[0]);
    DoublyListNode* cur = head;
    for (int i = 1; i < arr.size(); i++) {
        DoublyListNode* newNode = new DoublyListNode(arr[i]);
        cur->next = newNode;
        newNode->prev = cur;
        cur = cur->next;
    }
    return head;
}
```

>
>
> 好，我带你一行一行读懂。
>
> ---
>
> # 工具函数是什么意思？
>
> 就是 **把重复用到的逻辑封装成一个函数** ，方便反复调用。
>
> 这两个函数的作用就一件事： **给你一个数组，帮你转成链表，返回头节点** 。
>
> ```cpp
> // 不用工具函数：每次都要手写
> ListNode *a = new ListNode(1);
> ListNode *b = new ListNode(2);
> ListNode *c = new ListNode(3);
> a->next = b;
> b->next = c;
>
> // 用工具函数：一行搞定
> ListNode *head = createLinkedList({1, 2, 3});
> ```
>
> ---
>
> # 单链表工具函数逐行解释
>
> ```cpp
> ListNode* createLinkedList(vector<int> arr) {
> ```
>
> 接收一个整数数组，返回链表的头节点指针。
>
> ---
>
> ```cpp
>     if (arr.empty()) return nullptr;
> ```
>
> 如果数组是空的，直接返回空指针，链表为空。
>
> ---
>
> ```cpp
>     ListNode* head = new ListNode(arr[0]);
> ```
>
> 用数组第一个元素创建头节点。
>
> ```
> arr = [1, 2, 3]
> head → [1] → NULL
> ```
>
> ---
>
> ```cpp
>     ListNode* cur = head;
> ```
>
> `cur` 是一个"当前位置"的指针，初始指向头节点。
>
> 可以把 `cur` 想象成你的手指，永远指着 **链表最后一个节点** ，方便接上新节点。
>
> ```
> head → [1] → NULL
>         ↑
>        cur
> ```
>
> ---
>
> ```cpp
>     for (int i = 1; i < arr.size(); i++) {
>         cur->next = new ListNode(arr[i]);
>         cur = cur->next;
>     }
> ```
>
> 每次循环做两件事：
>
> **第一步：** `cur->next = new ListNode(arr[i])` — 在当前节点后面接一个新节点
>
> **第二步：** `cur = cur->next` — 手指往后移，指向刚接上的新节点
>
> 我们用 `[1, 2, 3]` 模拟走一遍：
>
> ```
> 初始：
> head → [1] → NULL
>         ↑
>        cur
>
> i=1，arr[1]=2：
>   cur->next = new ListNode(2)
>   head → [1] → [2] → NULL
>           ↑
>          cur
>
>   cur = cur->next
>   head → [1] → [2] → NULL
>                 ↑
>                cur
>
> i=2，arr[2]=3：
>   cur->next = new ListNode(3)
>   head → [1] → [2] → [3] → NULL
>                 ↑
>                cur
>
>   cur = cur->next
>   head → [1] → [2] → [3] → NULL
>                        ↑
>                       cur
>
> 循环结束
> ```
>
> ---
>
> ```cpp
>     return head;
> ```
>
> 返回头节点，这样外面就能通过头节点访问整条链表。
>
> ---
>
> # 双链表工具函数的区别
>
> 双链表比单链表多一个 `prev` 指针，所以每次创建新节点后， **要额外设置 prev** ：
>
> ```cpp
> DoublyListNode* newNode = new DoublyListNode(arr[i]);
> cur->next = newNode;    // 当前节点的 next 指向新节点（和单链表一样）
> newNode->prev = cur;    // 新节点的 prev 指向当前节点（多了这一步！）
> cur = cur->next;        // 手指后移
> ```
>
> 走一遍 `[1, 2, 3]`：
>
> ```
> 初始：
> head → [NULL ← 1 → NULL]
>                ↑
>               cur
>
> i=1，创建节点2：
>   cur->next = newNode
>   newNode->prev = cur
>
>   head → [NULL ← 1 ←→ 2 → NULL]
>                        ↑
>                    cur 后移到这里
>
> i=2，创建节点3：
>   head → [NULL ← 1 ←→ 2 ←→ 3 → NULL]
>                             ↑
>                         cur 后移到这里
> ```
>
> ---
>
> # 对比两个函数的差异
>
> ```cpp
> // 单链表：只设置 next
> cur->next = new ListNode(arr[i]);
> cur = cur->next;
>
> // 双链表：多设置一个 prev
> DoublyListNode* newNode = new DoublyListNode(arr[i]);
> cur->next = newNode;      // 同样设置 next
> newNode->prev = cur;      // 多了这一步：设置 prev
> cur = cur->next;
> ```
>
> ---
>
> # 一句话总结
>
> ```
> 工具函数 = 把"数组转链表"这个重复操作封装起来
>
> 核心逻辑：
> 1. 用第一个元素创建头节点
> 2. cur 指针跟踪当前最后一个节点
> 3. 循环：在 cur 后面接新节点，cur 往后移
> 4. 返回头节点
>
> 双链表比单链表多一步：每次接新节点时，设置 newNode->prev = cur
> ```
>
> 清楚了吗？

---

## 3. 单链表的基本操作

### 3.1 查/改 — O(N)

单链表没有索引，只能从头节点开始顺着 `next` 一个一个往后找：

```cpp
ListNode* head = createLinkedList({1, 2, 3, 4, 5});

// 遍历打印每个节点
for (ListNode* p = head; p != nullptr; p = p->next) {
    cout << p->val << endl;
}
```

> ⚠️ 单链表查找某个索引的元素，最坏情况要走到最后，时间复杂度 O(N)。

---

### 3.2 增

#### 头部插入 — O(1)

```cpp
// 新节点的 next 指向原头节点
// 新节点成为新的头节点
ListNode* newNode = new ListNode(0);
newNode->next = head;
head = newNode;
// 1->2->3->4->5  变成  0->1->2->3->4->5
```

图示：

```
插入前：head → [1] → [2] → [3]

步骤1：newNode->next = head
        [0] → [1] → [2] → [3]

步骤2：head = newNode
head → [0] → [1] → [2] → [3]  ✅
```

---

#### 尾部插入 — O(N)

需要先遍历到最后一个节点，再接上新节点：

```cpp
ListNode* p = head;
while (p->next != nullptr) {  // 走到最后一个节点
    p = p->next;
}
p->next = new ListNode(6);    // 接上新节点
// 1->2->3->4->5  变成  1->2->3->4->5->6
```

> 💡 如果持有尾节点的引用，尾部插入就是 O(1)。这也是双链表的优化点之一。

---

#### 中间插入 — O(N)

先找到插入位置的**前驱节点**，再操作指针：

```cpp
// 在第 3 个节点后面插入 66
ListNode* p = head;
for (int i = 0; i < 2; i++) {  // 走到第 3 个节点（索引2）
    p = p->next;
}

ListNode* newNode = new ListNode(66);
newNode->next = p->next;  // 步骤1：新节点先接后面
p->next = newNode;        // 步骤2：前驱节点再接新节点
// 变成 1->2->3->66->4->5
```

图示：

```
插入前：... → [3] → [4] → ...
                p

步骤1：newNode->next = p->next
       ... → [3] → [4] → ...
               ↑
           [66] ──→ [4]

步骤2：p->next = newNode
       ... → [3] → [66] → [4] → ...  ✅
```

> ⚠️ 两步顺序不能反！必须先让新节点接上后面，再断开前驱节点，否则会丢失后面的链表。

---

### 3.3 删

#### 头部删除 — O(1)

```cpp
head = head->next;  // head 直接移到下一个节点
// 1->2->3->4->5  变成  2->3->4->5
```

---

#### 尾部删除 — O(N)

需要找到**倒数第二个节点**，把它的 `next` 置为 null：

```cpp
ListNode* p = head;
while (p->next->next != nullptr) {  // 走到倒数第二个节点
    p = p->next;
}
p->next = nullptr;  // 断开尾节点
// 1->2->3->4->5  变成  1->2->3->4
```

---

#### 中间删除 — O(N)

找到被删节点的**前驱节点**，让前驱节点的 `next` 跳过被删节点：

```cpp
// 删除第 4 个节点
ListNode* p = head;
for (int i = 0; i < 2; i++) {  // 走到第 3 个节点（前驱）
    p = p->next;
}
p->next = p->next->next;  // 跳过第 4 个节点
// 1->2->3->4->5  变成  1->2->3->5
```

图示：

```
删除前：... → [3] → [4] → [5] → ...
                p   要删

p->next = p->next->next

删除后：... → [3] → [5] → ...  ✅
              ([4] 被跳过，无人持有引用，自动回收)
```

---

## 4. 双链表的基本操作

双链表每个节点同时持有 `prev` 和 `next`，操作时需要同时维护两个方向的指针。

### 4.1 查/改 — O(N)

双链表支持**双向遍历**，可以根据目标位置靠近头部还是尾部，选择合适方向遍历：

```cpp
DoublyListNode* tail = nullptr;

// 从头向后遍历
for (DoublyListNode* p = head; p != nullptr; p = p->next) {
    cout << p->val << endl;
    tail = p;
}

// 从尾向前遍历
for (DoublyListNode* p = tail; p != nullptr; p = p->prev) {
    cout << p->val << endl;
}
```

---

### 4.2 增

#### 头部插入 — O(1)

```cpp
DoublyListNode* newHead = new DoublyListNode(0);
newHead->next = head;   // 新节点的 next 指向原头节点
head->prev = newHead;   // 原头节点的 prev 指向新节点
head = newHead;         // 更新头节点
// 变成 0 <-> 1 <-> 2 <-> 3 <-> 4 <-> 5
```

---

#### 尾部插入 — O(N)，持有尾节点则 O(1)

```cpp
// 先走到尾节点
DoublyListNode* tail = head;
while (tail->next != nullptr) tail = tail->next;

// 插入新节点
DoublyListNode* newNode = new DoublyListNode(6);
tail->next = newNode;   // 原尾节点的 next 指向新节点
newNode->prev = tail;   // 新节点的 prev 指向原尾节点
tail = newNode;         // 更新尾节点引用
```

---

#### 中间插入 — O(N)

需要同时维护新节点与前后两个节点的指针，共 4 条指针需要更新：

```cpp
// 在索引 2（第 3 个节点）后面插入 66
DoublyListNode* p = head;
for (int i = 0; i < 2; i++) p = p->next;

DoublyListNode* newNode = new DoublyListNode(66);

// 先组装新节点的前后指针
newNode->next = p->next;  // 步骤1
newNode->prev = p;        // 步骤2

// 再修改相邻节点的指针
p->next->prev = newNode;  // 步骤3：后继节点的 prev 指向新节点
p->next = newNode;        // 步骤4：前驱节点的 next 指向新节点
// 变成 1 <-> 2 <-> 3 <-> 66 <-> 4 <-> 5
```

图示：

```
插入前：... ←→ [3] ←→ [4] ←→ ...
                p

插入后：... ←→ [3] ←→ [66] ←→ [4] ←→ ...
```

>
>
> 好，我用图一步一步画给你看。
>
> ---
>
> # 初始状态
>
> 链表是 `1 <-> 2 <-> 3 <-> 4 <-> 5`，要在索引2（节点3）后面插入66。
>
> 走完 for 循环后，p 指向节点3：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [4] ←→ [5]
>                 ↑
>                 p
> ```
>
> 创建新节点，此时 newNode 还没有连接任何东西：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [4] ←→ [5]
>                 ↑
>                 p
>
> [66]   ← newNode，孤立的，prev和next都是NULL
> ```
>
> ---
>
> # 步骤1：newNode->next = p->next
>
> 让新节点的 `next` 指向节点4（p的后继）：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [4] ←→ [5]
>                 ↑       ↑
>                 p       |
>                         |
>                [66] ────┘
>             newNode->next 指向4
> ```
>
> ---
>
> # 步骤2：newNode->prev = p
>
> 让新节点的 `prev` 指向节点3（p本身）：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [4] ←→ [5]
>                 ↑       ↑
>                 p       |
>                 |       |
>                 └──[66]─┘
>              newNode 两端都连好了
>              但节点3和节点4还不知道66的存在！
> ```
>
> ---
>
> # 步骤3：p->next->prev = newNode
>
> 让节点4的 `prev` 从指向节点3，改为指向新节点66：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3]    [4] ←→ [5]
>                 ↑      ↑
>                 p      |
>                 |      |（4的prev改指向66了）
>                 └─[66]─┘
> ```
>
> ---
>
> # 步骤4：p->next = newNode
>
> 让节点3的 `next` 从指向节点4，改为指向新节点66：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [66] ←→ [4] ←→ [5]
>                 ↑               ↑
>                 p（3的next改指向66了）
> ```
>
> **完成！**
>
> ---
>
> # 为什么顺序不能乱？
>
> 关键在于 **步骤4必须最后做** 。
>
> 步骤3需要用到 `p->next`（即节点4）：
>
> ```cpp
> p->next->prev = newNode;  // p->next 此时还是节点4
> ```
>
> 如果步骤4先执行，`p->next` 就变成了 newNode（66），步骤3就找不到节点4了：
>
> ```cpp
> // 错误顺序：
> p->next = newNode;        // 步骤4先做，p->next 变成了66
> p->next->prev = newNode;  // 步骤3：p->next 现在是66，66->prev = 66，自己指自己！❌
> ```
>
> ---
>
> # 四步总结
>
> ```
> 步骤1：newNode->next = p->next   先把新节点右边接好
> 步骤2：newNode->prev = p         再把新节点左边接好
> 步骤3：p->next->prev = newNode   让右边邻居认识新节点
> 步骤4：p->next = newNode         让左边邻居认识新节点（最后做！）
> ```
>
> 记忆口诀： **先把新节点两端接好，再让邻居改指针，最后改前驱的next** 。

---

### 4.3 删

#### 头部删除 — O(1)

```cpp
DoublyListNode* toDelete = head;
head = head->next;      // head 移到下一个节点
head->prev = nullptr;   // 新头节点的 prev 清空
toDelete->next = nullptr; // 清理被删节点（好习惯）
// 1<->2<->3<->4<->5  变成  2<->3<->4<->5
```

---

#### 尾部删除 — O(N)，持有尾节点则 O(1)

双链表的优势：有了 `prev` 指针，不需要找倒数第二个节点，可以直接操作尾节点自己：

```cpp
// 找到尾节点
DoublyListNode* p = head;
while (p->next != nullptr) p = p->next;

// 直接通过 prev 指针摘除自己
p->prev->next = nullptr;  // 前驱节点的 next 断开
p->prev = nullptr;        // 清理被删节点（好习惯）
// 1<->2<->3<->4<->5  变成  1<->2<->3<->4
```

> 对比单链表：单链表删除尾节点必须找倒数第二个节点；双链表直接通过 `prev` 操作自己，代码更简洁。
>
>
> 好，同样一步一步画图。
>
> ---
>
> # 初始状态
>
> 走完 while 循环后，p 指向尾节点5：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [4] ←→ [5] → NULL
>                                ↑
>                                p（尾节点）
> ```
>
> ---
>
> # 步骤1：p->prev->next = nullptr
>
> `p->prev` 是节点4，让节点4的 `next` 断开，不再指向节点5：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [4] → NULL     [5] → NULL
>                         ↑              ↑
>                    节点4的next         p
>                    断开了              但5的prev还指着4
> ```
>
> 节点4现在是新的尾节点了，链表在这里结束。
>
> ---
>
> # 步骤2：p->prev = nullptr
>
> 把被删节点5的 `prev` 也断开，彻底和链表脱离关系：
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [4] → NULL     [5]
>                                     prev=NULL, next=NULL
>                                     完全孤立，等待回收
> ```
>
> ---
>
> # 完成
>
> ```
> head
>  ↓
> [1] ←→ [2] ←→ [3] ←→ [4] → NULL
> ```
>
> ---
>
> # 为什么双链表比单链表方便？
>
> 单链表删除尾节点，必须找到**倒数第二个节点**才能操作：
>
> ```cpp
> // 单链表：要找倒数第二个节点
> while (p->next->next != nullptr) p = p->next;
> p->next = nullptr;  // 操作倒数第二个节点
> ```
>
> 双链表因为有 `prev`，尾节点可以 **直接找到自己的前驱** ，不需要从头再走一遍：
>
> ```cpp
> // 双链表：尾节点直接通过 prev 找到前驱
> p->prev->next = nullptr;  // 直接操作，不需要额外遍历
> ```
>
> ---
>
> # 一句话总结
>
> ```
> p->prev->next = nullptr   →  让前驱节点的 next 断开（链表在这里结束）
> p->prev = nullptr         →  让被删节点彻底孤立（清理好习惯）
> ```
>
> 本质就是：**通过 prev 找到前驱，让前驱把 next 断掉，节点5就从链表里消失了。**

---

#### 中间删除 — O(N)，已知节点引用则 O(1)

```cpp
// 删除第 4 个节点
DoublyListNode* p = head;
for (int i = 0; i < 2; i++) p = p->next;

DoublyListNode* toDelete = p->next;    // 要删除的节点

// 前后节点互相连接，跳过 toDelete
p->next = toDelete->next;              // 前驱的 next 跳过
toDelete->next->prev = p;             // 后继的 prev 跳过

// 清理被删节点的指针（好习惯）
toDelete->next = nullptr;
toDelete->prev = nullptr;
// 1<->2<->3<->4<->5  变成  1<->2<->3<->5
```

图示：

```
删除前：... ←→ [3] ←→ [4] ←→ [5] ←→ ...
                p    toDelete

删除后：... ←→ [3] ←→ [5] ←→ ...
              ([4] 无人持有引用，自动回收)
```

>
>
> 好，这是个很关键的问题，彻底搞清楚这个，链表就全通了。
>
> ---
>
> # 先回到最基础的问题：变量存的是什么？
>
> ```cpp
> int a = 10;
> ```
>
> `a` 这个变量，存的是**值** `10`。
>
> ```cpp
> int *p = &a;
> ```
>
> `p` 这个变量，存的是 **地址** ，比如 `0x1000`。
>
> ---
>
> # 节点指针存的也是地址
>
> ```cpp
> ListNode *node = new ListNode(1);
> ```
>
> `node` 存的不是节点本身，而是节点在内存里的 **地址** ，比如 `0x2000`。
>
> ```
> node 变量：[ 0x2000 ]   ← 存的是地址
>
> 内存 0x2000 处：
> ┌─────┬──────┐
> │  1  │ NULL │
> └─────┴──────┘
>  val   next
> ```
>
> ---
>
> # p->next 到底是什么？
>
> ```cpp
> ListNode *a = new ListNode(1);   // a 存地址 0x2000
> ListNode *b = new ListNode(2);   // b 存地址 0x3000
> a->next = b;                     // a->next 存的是 b 的地址 0x3000
> ```
>
> 内存里长这样：
>
> ```
> a 变量：[ 0x2000 ]
>
> 内存 0x2000（节点1）：
> ┌─────┬──────────┐
> │  1  │  0x3000  │   ← next 存的是节点2的地址
> └─────┴──────────┘
>
> 内存 0x3000（节点2）：
> ┌─────┬──────┐
> │  2  │ NULL │
> └─────┴──────┘
> ```
>
> 所以 `a->next` 就是 `0x3000`，它是一个 **地址** ，指向节点2所在的内存。
>
> ---
>
> # 关键：地址就等于节点本身
>
> 这是最重要的一点：
>
> ```cpp
> ListNode *p = a->next;
> ```
>
> `p` 现在存的是 `0x3000`，也就是节点2的地址。
>
>  **你拿到了地址，就等于拿到了那个节点** ，因为你可以通过地址访问那块内存的所有内容：
>
> ```cpp
> p->val;   // 去 0x3000 取 val，得到 2
> p->next;  // 去 0x3000 取 next，得到 NULL
> ```
>
> ---
>
> # 所以 p->next 为什么能代表一个节点？
>
> 因为 `p->next` 存的是下一个节点的 **地址** ，而地址就是那个节点的"门牌号"，有了门牌号就能访问那个节点的一切。
>
> ```
> p->next          是一个地址（门牌号）
> *(p->next)       去那个地址取出节点本身
> (p->next)->val   去那个地址取出节点的 val
> (p->next)->next  去那个地址取出节点的 next
> ```
>
> 用图来说：
>
> ```
> p → [1 | 0x3000]
>          ↓
>        0x3000 就是节点2的地址
>          ↓
>     [2 | NULL]   ← 这就是节点2本身
> ```
>
> `p->next` 就是 `0x3000`，你可以把这个地址再赋给另一个指针，然后继续操作：
>
> ```cpp
> ListNode *q = p->next;  // q 也指向节点2
> q->val;                 // 2
> ```
>
> ---
>
> # 再看链表遍历就完全清楚了
>
> ```cpp
> for (ListNode *p = head; p != nullptr; p = p->next) {
>     cout << p->val << endl;
> }
> ```
>
> 逐步拆解：
>
> ```
> 初始：p = head = 0x2000（节点1的地址）
>
> 第1轮：
>   p = 0x2000，不是nullptr
>   p->val = 1，打印1
>   p = p->next = 0x3000（节点2的地址）
>
> 第2轮：
>   p = 0x3000，不是nullptr
>   p->val = 2，打印2
>   p = p->next = 0x4000（节点3的地址）
>
> 第3轮：
>   p = 0x4000，不是nullptr
>   p->val = 3，打印3
>   p = p->next = NULL
>
> p == nullptr，循环结束
> ```
>
> ---
>
> # 一句话总结
>
> ```
> 指针存的是地址
> 地址就是那个节点的"门牌号"
> 有了门牌号就能访问那个节点的所有内容
>
> 所以：
> p          → 节点本身（通过地址访问）
> p->val     → 节点的值
> p->next    → 下一个节点的地址（也就是下一个节点）
> p->next->val → 下一个节点的值
> ```
>
> **p->next 能代表节点，是因为它存的是那个节点的地址，地址就是通往那个节点的钥匙。**
>
> 清楚了吗？

---

## 5. 时间复杂度总结

### 单链表

| 操作        |   时间复杂度   | 备注                 |
| ----------- | :------------: | -------------------- |
| 头部插入    | **O(1)** | 直接操作 head        |
| 尾部插入    |      O(N)      | 需要遍历到尾部       |
| 中间插入    |      O(N)      | 需要找前驱节点       |
| 头部删除    | **O(1)** | 直接操作 head        |
| 尾部删除    |      O(N)      | 需要找倒数第二个节点 |
| 中间删除    |      O(N)      | 需要找前驱节点       |
| 按索引查/改 |      O(N)      | 只能从头遍历         |

### 双链表

| 操作        |      时间复杂度      | 备注                         |
| ----------- | :------------------: | ---------------------------- |
| 头部插入    |    **O(1)**    | 直接操作 head                |
| 尾部插入    | O(N) /**O(1)** | 持有尾节点引用则 O(1)        |
| 中间插入    |         O(N)         | 需要找到插入位置             |
| 头部删除    |    **O(1)**    | 直接操作 head                |
| 尾部删除    | O(N) /**O(1)** | 持有尾节点引用则 O(1)        |
| 中间删除    | O(N) /**O(1)** | 已知节点引用则 O(1)          |
| 按索引查/改 |         O(N)         | 可双向遍历，效率略优于单链表 |

---

## 6. 数组 vs 链表对比

|                    |  数组（vector）  |           链表（list）           |
| ------------------ | :---------------: | :------------------------------: |
| 内存结构           |       连续       |               分散               |
| 随机访问（按索引） |  **O(1)**  |               O(N)               |
| 头部插入/删除      |       O(N)       |          **O(1)**          |
| 尾部插入/删除      |  **O(1)**  |        O(1)（持有尾节点）        |
| 中间插入/删除      |       O(N)       | O(N)（找位置）/ O(1)（已知节点） |
| 内存占用           |       紧凑       |        每个节点额外存指针        |
| 扩容               | 需要重新申请+复制 |              不需要              |

**选择原则：**

```
频繁按索引访问         → 用数组（vector）
频繁在头部增删         → 用链表（list）
频繁在已知位置增删     → 用链表（list）
其他大多数情况         → 用数组（vector），缓存友好，实际更快
```

---

> 📌 **下一章预告**
>
> 用单链表和双链表实现 `MyLinkedList`，并介绍**虚拟头节点技巧**，把头、尾、中部的操作统一起来，避免处理空指针边界情况。
