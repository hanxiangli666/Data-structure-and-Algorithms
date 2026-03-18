# 哈希链表（LinkedHashMap）学习笔记

> **前置知识**：哈希表的基本原理（数组 + 哈希函数 + 解决冲突）
> **核心问题**：普通哈希表的 key 是无序的，我们能不能让 key **按插入顺序**排列，同时不影响 O(1) 的增删查改？
> **答案**：可以！用一个**双链表**把所有键值对串起来，就得到了「哈希链表」。

---

## 一、为什么需要哈希链表？

### 1.1 普通哈希表的问题

普通哈希表（`unordered_map`）内部是一个数组，key 通过哈希函数被分散存储到不同位置。

**类比**：想象一个大型停车场，每辆车（key）根据车牌号被分配到某个车位。你没法通过走一遍停车场来还原「这些车是按什么顺序开进来的」——因为车位的分配跟到达顺序无关。

所以普通哈希表：

- ✅ 查找、插入、删除都是 O(1)
- ❌ 无法按插入顺序遍历所有 key
- ❌ 一旦扩容，key 的位置还会变

### 1.2 我们想要什么？

> 一种数据结构，**既有哈希表 O(1) 的增删查改能力**，**又能按照插入顺序遍历所有 key**。

这就是 **LinkedHashMap（哈希链表）**。

![1773781821130](image/2链表加强哈希表_LinkedHashMap_笔记/1773781821130.png)

![1773781875913](image/2链表加强哈希表_LinkedHashMap_笔记/1773781875913.png)

---

## 二、核心思路：哈希表 + 双链表

### 2.1 直觉理解

**类比**：还是停车场的例子。这次我们在停车场门口放了一本「签到簿」（双链表）。每辆车进来时，除了停到指定车位（哈希表插入），还要在签到簿末尾签上名字（链表尾部追加）。

这样：

- 想找某辆车？→ 通过车牌号直接去车位（哈希表 O(1) 查找）
- 想知道到达顺序？→ 翻签到簿从头看到尾（链表顺序遍历）
- 某辆车要走？→ 从车位移走（哈希表删除），同时把签到簿上对应的名字划掉（链表删除）

### 2.2 结构设计

把普通哈希表的「值」包装成一个**双链表节点**：

```
普通哈希表:    key  →  value
哈希链表:      key  →  Node(key, value, prev, next)
```

所有 Node 通过 `prev` 和 `next` 指针串成一条双链表，并持有 `head`（头哨兵）和 `tail`（尾哨兵）。

**示意图**：

```
哈希表部分（快速查找）：
  map["k2"] ──→ Node(k2)
  map["k4"] ──→ Node(k4)
  map["k5"] ──→ Node(k5)

双链表部分（维护插入顺序）：
  head ↔ Node(k2) ↔ Node(k4) ↔ Node(k5) ↔ tail
  
  插入顺序就是: k2 → k4 → k5
```

### 2.3 为什么必须用「双链表」而不是「单链表」？

删除一个节点时，需要修改它**前面那个节点**的 `next` 指针。

- **双链表**：节点自己就有 `prev` 指针，直接找到前驱，O(1) 删除
- **单链表**：没有 `prev` 指针，必须从头遍历才能找到前驱，O(n) 删除

所以为了保证删除操作是 O(1)，必须用双链表。

---

## 三、各操作的复杂度分析

| 操作                             | 哈希表部分       | 链表部分               | 总复杂度       |
| -------------------------------- | ---------------- | ---------------------- | -------------- |
| **查找** `get(key)`      | O(1) 找到 Node   | 直接读 Node.val        | **O(1)** |
| **插入** `put(key, val)` | O(1) 放入 map    | O(1) 追加到链表尾部    | **O(1)** |
| **删除** `remove(key)`   | O(1) 从 map 删除 | O(1) 双链表指针操作    | **O(1)** |
| **按序遍历** `keys()`    | —               | O(n) 从 head 走到 tail | **O(n)** |

关键点：所有操作的复杂度都和普通哈希表一样，只是额外加了一个链表来维护顺序。

---

## 四、C++ 代码实现 + 逐行解读

### 4.1 双链表节点定义

**第一步：最简单的例子**

想象你要把一群学生连成一队。每个学生需要知道自己前面是谁、后面是谁，这就是双链表节点的作用：

```cpp
struct Student {
    string name;        // 学生名字
    int score;          // 学生成绩
    Student* prev;      // 指向前一个学生
    Student* next;      // 指向后一个学生
    Student(string n, int s) : name(n), score(s), prev(nullptr), next(nullptr) {}
};
```

> 下面的代码完全相同，只是把「学生」换成了通用的「数据对」(key, value)。

```cpp
template <typename K, typename V>
struct Node {
    K key;                              // ⚠️ 存 key
    V val;                              // ⚠️ 存 value
    Node* next;                         // ⚠️ 指向链表中的下一个节点
    Node* prev;                         // ⚠️ 指向链表中的上一个节点（这就是"双"的含义）
    Node(K key, V val) : key(key), val(val), next(nullptr), prev(nullptr) {}
};
```

> ✅ 双链表最大的好处就是：有了 prev，删除任意节点都能 O(1) 完成。

### 4.2 哈希链表类整体结构

**第一步：最简单的例子**

一个哈希链表需要两个部分：一个哈希表（快速查找）和一个双链表（保持顺序）。就像学校登记系统：有一个索引簿（哈希表）快速查学生，也有一个签到册（链表）记录学生的到达顺序：

```cpp
unordered_map<string, Student*> studentIndex;  // 快速查找
Student* firstStudent = nullptr;               // 链表头
Student* lastStudent = nullptr;                // 链表尾
```

> 下面的代码就是这个思路，使用哈希表映射 key → 节点指针，用 head/tail 哨兵维护链表。

```cpp
template <typename K, typename V>
class MyLinkedHashMap {
private:
    Node<K, V>* head;                           // ⚠️ 头哨兵（不存数据，只做边界标记）
    Node<K, V>* tail;                           // ⚠️ 尾哨兵（不存数据，只做边界标记）
    unordered_map<K, Node<K, V>*> map;          // ⚠️ key → 节点指针的映射

    // ... 私有方法（链表操作）
public:
    // ... 公有方法（对外接口）
};
```

> 💡 **老师提醒：** head 和 tail 是哨兵节点，就像书的封面和封底——不是内容，但让遍历逻辑简洁。

### 4.3 构造函数和析构函数

**第一步：最简单的例子**

初始化一个「签到册」：先建立一个封面（头哨兵）和封底（尾哨兵），让它们互相连接。销毁时逐页撕掉所有页面：

```cpp
void initSignInBook() {
    head = new Page();     // 创建封面
    tail = new Page();     // 创建封底
    head->next = tail;     // 封面 → 封底
    tail->prev = head;     // 封底 ← 封面
}

void destroySignInBook() {
    Page* current = head->next;
    while (current != tail) {
        Page* next = current->next;
        delete current;    // 撕掉这页  
        current = next;
    }
    delete head;
    delete tail;
}
```

> 下面的代码完全相同，只是改用面向对象的构造函数和析构函数写法。

```cpp
MyLinkedHashMap() {
    head = new Node<K, V>(K(), V());   // 创建头哨兵节点
    tail = new Node<K, V>(K(), V());   // 创建尾哨兵节点
    head->next = tail;                  // head 和 tail 互相连接
    tail->prev = head;                  // 初始状态：head ↔ tail（中间没有数据）
}
```

初始状态：`head ↔ tail`（中间没有数据节点）

```cpp
~MyLinkedHashMap() {
    // 遍历并释放所有数据节点
    Node<K, V>* current = head->next;      // ⚠️ 从 head 下一个开始（跳过头哨兵）
    while (current != tail) {              // 循环到 tail（跳过尾哨兵）
        Node<K, V>* next = current->next;  // 先保存下一个节点
        delete current;                     // 释放当前节点的内存
        current = next;                     // 移动到下一个
    }
    delete head;
    delete tail;
}
```

> 💡 **老师提醒：** 一定要先保存 `next`，再 `delete`，否则 delete 后访问 `current->next` 会出错（访问已释放的内存）。

### 4.4 核心私有方法：链表操作

#### 尾部插入

**第一步：最简单的例子**

在队伍末尾加一个新人。需要连接三条关系：新人的前驱指向队尾前的那个人，新人的后驱指向队尾，队尾前的人指向新人，队尾指向新人：

```cpp
void addStudentAtEnd(Student* newStudent) {
    Student* lastStudent = tail->prev;  // 队尾前的那个学生
    newStudent->next = tail;            // 新人 → 队尾
    newStudent->prev = lastStudent;     // 新人 ← 队尾前
    lastStudent->next = newStudent;     // 队尾前 → 新人
    tail->prev = newStudent;            // 队尾 ← 新人
}
```

> 下面的代码逻辑完全相同。

```cpp
void addLastNode(Node<K, V>* x) {
    Node<K, V>* temp = tail->prev;  // temp 是当前最后一个数据节点
    // 原状态: temp ↔ tail
    // 目标状态: temp ↔ x ↔ tail
  
    x->next = tail;                 // x 的后驱指向 tail
    x->prev = temp;                 // x 的前驱指向 temp
    temp->next = x;                 // temp 的后驱指向 x
    tail->prev = x;                 // tail 的前驱指向 x（更新队尾）
}
```

**图解**（插入节点 x）：

```
插入前:  ... ↔ temp ↔ tail
插入后:  ... ↔ temp ↔ x ↔ tail
```

> ✅ 四行指针操作，O(1) 完成插入。

#### 删除任意节点

**第一步：最简单的例子**

从队伍中间踢出一个学生。只需要让他前一个和后一个学生直接相连，绕过被删的这个人：

```cpp
void removeStudentFromQueue(Student* toDelete) {
    Student* prevStudent = toDelete->prev;      // 前一个学生
    Student* nextStudent = toDelete->next;      // 后一个学生
    
    prevStudent->next = nextStudent;            // 前 → 后（跳过 toDelete）
    nextStudent->prev = prevStudent;            // 后 ← 前（跳过 toDelete）
    toDelete->next = toDelete->prev = nullptr;  // 清空 toDelete 的指针（安全）
}
```

> 下面的代码完全相同。

```cpp
void removeNode(Node<K, V>* x) {
    Node<K, V>* prev = x->prev;     // x 前面的节点
    Node<K, V>* next = x->next;     // x 后面的节点
    // 原状态: prev ↔ x ↔ next
    // 目标状态: prev ↔ next（x 被跳过）
  
    prev->next = next;              // prev 直接指向 next
    next->prev = prev;              // next 直接指向 prev
    x->next = x->prev = nullptr;    // ⚠️ 清空 x 的指针（防止野指针）
}
```

**图解**（删除节点 x）：

```
删除前:  ... ↔ prev ↔ x ↔ next ↔ ...
删除后:  ... ↔ prev ↔ next ↔ ...   (x 被跳开了)
```

> 💡 **老师提醒：** 这就是为什么删除是 O(1)——我们已经有了 x 的引用和它的 prev，所以不用遍历就能麻利地摘出 x。

### 4.5 核心公有方法

#### get —— 查找

```cpp
V get(K key) {
    if (map.find(key) == map.end()) {
        return V();    // key 不存在，返回默认值
    }
    return map[key]->val;  // 通过哈希表找到节点，取出 val
}
```

**C++ 知识点**：

- `map.find(key)`：在 `unordered_map` 中查找 key，返回迭代器
- `map.end()`：指向容器「末尾之后」的迭代器。`find` 返回 `end()` 表示没找到
- `map[key]->val`：`map[key]` 得到 `Node*`（节点指针），`->` 是通过指针访问成员

#### put —— 插入/更新

**第一步：最简单的例子**

往「签到册」里加新学生（或改成绩）。新学生要在索引簿里注册，也要在签到册末页记录：

```cpp
void registerStudent(string name, int score) {
    if (studentIndex.count(name)) {      // 学生已经注册过
        studentIndex[name]->score = score;  // 直接改成绩
        return;
    }
    // 新学生：先创建节点，加入末页，再加入索引簿
    Student* node = new Student(name, score);
    addStudentToEnd(node);               // 加到链表末尾
    studentIndex[name] = node;           // 注册到索引
}
```

> 下面的代码完全相同的思路。

```cpp
void put(K key, V val) {
    if (map.find(key) == map.end()) {
        // key 不存在 → 新插入
        Node<K, V>* node = new Node<K, V>(key, val);        // ⚠️ 动态创建节点
        addLastNode(node);                                  // 加入链表尾部
        map[key] = node;                                    // 加入哈希表
        return;
    }
    // key 已存在 → 只更新 value
    map[key]->val = val;
}
```

> ✅ 关键：新 key 永远加在链表的尾部，这样就自动维护了「按插入顺序」的特性。

#### remove —— 删除

**第一步：最简单的例子**

删除一个学生：从索引簿里删掉，也从签到册里撕掉：

```cpp
void deleteStudent(string name) {
    if (!studentIndex.count(name)) return;    // 没有这个学生
    
    Student* node = studentIndex[name];       // 先拿到节点
    studentIndex.erase(name);                 // 从索引簿删掉
    removeStudentFromQueue(node);             // 从签到册删掉
    delete node;                              // 释放内存
}
```

> 下面的代码完全相同。

```cpp
void remove(K key) {
    if (map.find(key) == map.end()) {
        return;  // key 不存在，啥也不做
    }
    Node<K, V>* node = map[key];              // 通过哈希表快速找到节点
    map.erase(key);                           // 从哈希表中删除
    removeNode(node);                         // 从链表中删除（O(1)）
    delete node;                              // 释放内存
}
```

> ✅ 一共三行操作，每一行都是 O(1)，所以总复杂度也是 O(1)。

#### keys —— 按插入顺序获取所有 key

**第一步：最简单的例子**

从签到册里，按顺序读出所有学生的名字。从第一个学生开始，一直往下看，直到最后一个：

```cpp
vector<string> getAllStudentNames() {
    vector<string> names;
    for (Student* p = head->next; p != tail; p = p->next) {
        names.push_back(p->name);        // 按顺序收集所有名字
    }
    return names;
}
```

> 下面的代码完全相同，只是改成了通用的「key」。

```cpp
vector<K> keys() {
    vector<K> keyList;
    // 从 head 的下一个开始（跳过头哨兵），走到 tail 之前（跳过尾哨兵）
    for (Node<K, V>* p = head->next; p != tail; p = p->next) {
        keyList.push_back(p->key);      // ⚠️ 遍历链表，收集所有 key
    }
    return keyList;                     // 返回的顺序就是插入顺序
}
```

> ✅ 这里遍历的就是双链表，保证了顺序不会乱。

---

## 五、完整使用示例

```cpp
int main() {
    MyLinkedHashMap<string, int> map;
    map.put("a", 1);
    map.put("b", 2);
    map.put("c", 3);
    map.put("d", 4);
    map.put("e", 5);

    // 输出: a b c d e（按插入顺序！）
    for (const auto& key : map.keys()) {
        cout << key << " ";
    }
    cout << endl;

    map.remove("c");

    // 输出: a b d e（c 被删除了，其他顺序不变）
    for (const auto& key : map.keys()) {
        cout << key << " ";
    }
    cout << endl;

    return 0;
}
```

---

## 六、Golang map 遍历顺序为什么每次不同？

文中提到 Go 的 map 每次遍历顺序都是随机的。这其实是 **Go 语言故意设计的**：

Go 在遍历 map 时会**随机选择起始位置**，目的就是让开发者**不要依赖遍历顺序**。如果每次顺序都一样，有人可能会不小心写出依赖这个顺序的代码，但这个顺序其实是不保证的，未来可能因为实现变化而改变。所以 Go 直接把它搞成随机的，强制你不要依赖它。

**类比**：就像老师批卷子时故意每次打乱试卷顺序，防止你以为「我交的早就改的早」。

---

## 七、LinkedHashSet

**一句话**：LinkedHashSet 就是 LinkedHashMap 的简化版——只要 key，不要 value。

实现方式：直接包装 LinkedHashMap，把 value 设为一个固定的无意义值就行。

```cpp
// 伪代码思路
class LinkedHashSet<K> {
    MyLinkedHashMap<K, bool> map;  // value 用 bool 占位

    void add(K key) { map.put(key, true); }
    void remove(K key) { map.remove(key); }
    bool contains(K key) { return map.containsKey(key); }
};
```

---

## 八、关键概念速查表

| 概念                      | 说明                                                        |
| ------------------------- | ----------------------------------------------------------- |
| **哈希表**          | key → value 的映射，增删查改 O(1)，但 key 无序             |
| **双链表**          | 每个节点有 prev 和 next 指针，支持 O(1) 的任意位置插入/删除 |
| **哈希链表**        | 哈希表 + 双链表的组合，既有 O(1) 操作，又能按序遍历         |
| **哨兵节点**        | 不存数据的 head/tail 节点，简化边界处理                     |
| **模板 (template)** | C++ 泛型机制，让类/函数支持多种数据类型                     |
| **析构函数**        | C++ 中对象销毁时自动调用，用来释放 `new` 出来的内存       |

---

## 九、竞赛中的应用场景

哈希链表在竞赛中最经典的应用是 **LRU Cache（最近最少使用缓存）**，这也是 LeetCode 上的经典题目（LeetCode 146）。

LRU Cache 的思路和哈希链表几乎一样，只是多了一个「容量限制」和「每次访问时把节点移到最前面」的操作。学会了本文的内容，LRU Cache 就是一道顺水推舟的题目。

---

## 十、练习建议

1. **先理解原理**：确保你能在纸上画出「插入 3 个 key 后链表的样子」以及「删除中间一个 key 后链表的变化」
2. **自己实现一遍**：不看代码，试着自己用 C++ 写一个简化版的 LinkedHashMap
3. **做 LeetCode 146 (LRU Cache)**：这是哈希链表最经典的应用题
4. **做 LeetCode 460 (LFU Cache)**：进阶版，需要多个链表配合哈希表

---

*笔记整理时间：2026-03-16*
