好，我们一个一个来，彻底搞清楚。先从最基础的开始。

---

# 第一部分：`&` 和 `*` ——最容易混淆的两个符号

这两个符号最让人头疼的原因是：**同一个符号在不同位置含义完全不同。**

我们先把所有情况列出来：

---

## `&` 的两种含义

**含义一：取地址（放在变量前面）**

```cpp
int a = 10;
&a;   // 取a的地址，得到一个数字（比如0x1000）
```

**含义二：声明引用（放在类型后面）**

```cpp
int &ref = a;   // ref是a的引用（别名）
```

判断方法很简单：

```cpp
// 看&左边有没有类型名
int &ref = a;   // 左边有int，是声明引用
&a;             // 左边没有类型名，是取地址
```

---

## `*` 的两种含义

**含义一：声明指针（放在类型后面）**

```cpp
int *p;   // 声明p是一个指针
```

**含义二：解引用（放在指针变量前面）**

```cpp
*p;   // 去p存的地址那里取值
```

判断方法：

```cpp
int *p = &a;   // 左边有int，是声明指针
*p = 20;       // 左边没有类型名，是解引用
```

---

## 把四种情况放在一起看

```cpp
int a = 10;

int &ref = a;    // & 在类型后：声明引用
&a;              // & 在变量前：取地址

int *p = &a;     // * 在类型后：声明指针
*p = 20;         // * 在变量前：解引用（取值/赋值）
```

---

## 引用 vs 指针的区别

用一个图来理解：

```
变量a：  [10]  地址：1000

引用ref：      直接就是a，没有自己的地址
              ref = 20 就是 a = 20

指针p：  [1000] 地址：2000
              p存的是a的地址1000
              *p = 20 才能修改a
```

```cpp
int a = 10;

// 引用：ref就是a的另一个名字，用起来和a完全一样
int &ref = a;
ref = 20;        // a变成20，不需要任何特殊符号

// 指针：p存的是a的地址，要用*才能拿到值
int *p = &a;
*p = 30;         // a变成30，必须加*
p = 30;          // ❌ 错误！这是把地址改成30，不是改a的值
```

**竞赛中的实际使用：**

```cpp
// 引用：函数参数中最常用
void addOne(int &x) {
    x++;   // 直接修改，不需要*
}

// 指针：现阶段只在new/delete中见到
int *p = new int[10];
delete[] p;
```

---

# 第二部分：`new` 和 `delete`

## 为什么需要new？

C++申请内存有两种方式：

**方式一：普通声明（栈内存，自动管理）**

```cpp
int a = 10;          // 自动分配，函数结束自动释放
vector<int> v(10);   // 自动分配，函数结束自动释放
```

**方式二：new（堆内存，手动管理）**

```cpp
int *p = new int;        // 手动申请一个int的空间
int *arr = new int[10];  // 手动申请10个int的空间
// 用完必须手动释放！
delete p;
delete[] arr;
```

**直觉理解：**

```
栈内存：酒店房间，退房（函数结束）自动清理
堆内存：自己买的房子，要自己打扫，不打扫就一直占着（内存泄漏）
```

**什么时候用new？**

竞赛中其实很少直接用new，因为vector已经帮你管理好了。只有手写数据结构（比如链表、树）时才会用到。

---

# 第三部分：`class`、`public`、`private`

## class是什么？

`class` 就是 **把数据和操作数据的函数打包在一起** 。

你已经用过 `struct`，`class` 和 `struct` 几乎一样，唯一区别是：

```cpp
struct Student {
    // 默认public，外部可以直接访问
    string name;
    int age;
};

class Student {
    // 默认private，外部不能直接访问
    string name;
    int age;
};
```

---

## public 和 private

```cpp
class Student {
private:
    // 私有：只有class内部能访问
    string name;
    int age;

public:
    // 公开：外部可以访问
    void setName(string n) {
        name = n;   // 内部可以访问private成员
    }

    string getName() {
        return name;
    }
};

int main() {
    Student s;
    s.name = "Alice";      // ❌ private，不能直接访问
    s.setName("Alice");    // ✅ public，可以访问
    cout << s.getName();   // ✅ public，可以访问
}
```

**直觉理解：**

```
private = 内部实现细节，不让外人碰
public  = 对外提供的接口，外人通过这里操作

就像手机：
private = 内部电路板（你不能直接摸）
public  = 按钮和屏幕（你通过这里操作）
```

---

## this 是什么？

```cpp
class Student {
private:
    string name;

public:
    void setName(string name) {
        this->name = name;  // this->name是成员变量
                            // name是参数
    }
};
```

`this` 就是Python里的 `self`，表示"这个对象自己"。当参数名和成员变量名一样时，用 `this->` 区分。

---

# 第四部分：template

## 为什么需要template？

假设你要写一个"找最大值"的函数：

```cpp
int maxInt(int a, int b) { return a > b ? a : b; }
double maxDouble(double a, double b) { return a > b ? a : b; }
string maxString(string a, string b) { return a > b ? a : b; }
// 逻辑完全一样，只是类型不同，要写三遍？
```

用template，写一次搞定所有类型：

```cpp
template<typename T>   // T是类型占位符
T maxVal(T a, T b) {
    return a > b ? a : b;
}

int main() {
    cout << maxVal(3, 5) << endl;          // T=int
    cout << maxVal(3.14, 2.71) << endl;    // T=double
    cout << maxVal("abc", "xyz") << endl;  // T=string
}
```

`T` 只是个名字，叫 `E`、`Type`、`whatever` 都行，只是习惯用 `T`。

---

## class中的template

```cpp
template<typename T>
class MyList {
private:
    T* data;   // T是什么类型，用的时候再决定
    int size;

public:
    void add(T item) { ... }
    T get(int index) { ... }
};

int main() {
    MyList<int> intList;      // T=int
    MyList<string> strList;   // T=string
}
```

这就是你之前看到的 `MyArrayList<int>` 的原理，`<int>` 就是告诉编译器 `T` 是 `int`。

> 两种写法 **完全一样** ，只是风格不同。
>
> ```cpp
> T* data;   // 风格一：* 靠近类型
> T *data;   // 风格二：* 靠近变量名
> T * data;  // 风格三：* 在中间
> ```
>
> 三种写法对编译器来说没有任何区别，完全等价。
>
> ---
>
> # 但是有一个坑
>
> `*` 靠近类型的写法会让人产生误解：
>
> ```cpp
> int* a, b;   // 你可能以为a和b都是指针
>              // 实际上只有a是指针，b是普通int！
> ```
>
> 正确理解：
>
> ```cpp
> int* a, b;   // 等价于 int *a, b;
>              // *只修饰紧跟着的那个变量名
> ```
>
> 如果想声明两个指针：
>
> ```cpp
> int *a, *b;  // 每个变量前都要加*
> ```
>
> ---
>
> # 为什么会有两种风格？
>
> 这是C++社区里一个经典争论：
>
> **`T* p` 派** ：`T*` 是一个整体，表示"指向T的指针"这种类型
>
> **`T *p` 派** ：`*` 属于变量名，更准确反映上面那个坑
>
> 两种风格都有人用，竞赛中见到哪种都要认识。
>
> **你自己写的时候统一用一种就行，我建议用 `T *p`** ，避免上面那个坑。

---

# 最后：全部串起来看那段代码

```cpp
template<typename E>      // E是类型占位符
class MyArrayList {       // 定义一个类
private:                  // 外部不能直接访问
    E* data;              // 指针，指向动态数组
    int size;
    int cap;

public:                   // 外部可以访问
    MyArrayList() {
        data = new E[1];  // 申请堆内存
        size = 0;
        cap = 1;
    }

    void addLast(E e) {
        if (size == cap) resize(2 * cap);
        data[size] = e;   // 用指针访问数组
        size++;
    }

    ~MyArrayList() {
        delete[] data;    // 释放堆内存
    }
};

int main() {
    MyArrayList<int> arr;  // E=int
    arr.addLast(1);
}
```

> 好问题，这是理解指针的关键。
>
> ---
>
> # 先搞清楚：`new` 返回的是什么？
>
> ```cpp
> new E[1]
> ```
>
> `new` 做了两件事：
>
> 1. 在内存里申请一块空间
> 2. **返回这块空间的起始地址**
>
> 所以 `new E[1]` 的结果就是一个 **地址** ，比如 `0x1000`。
>
> ---
>
> # 指针存的就是地址
>
> ```cpp
> E *data;        // data是一个指针，专门用来存地址
> data = new E[1]; // new返回地址，存进data
> ```
>
> 所以这行代码的意思是：
>
> ```
> 申请一块内存 → 得到它的地址（比如0x1000）→ 把0x1000存进data
> ```
>
> 图示：
>
> ```
> data: [0x1000]  ←  data里存的是地址0x1000
>
> 内存：
> 0x1000: [ ]  ←  new申请的空间在这里
> ```
>
> ---
>
> # 类比理解
>
> 把内存想象成一栋大楼，每个房间有门牌号（地址）：
>
> ```
> new E[1] = 订了一个房间，返回房间的门牌号（比如301）
>
> E *data  = 一张纸，专门用来记门牌号
>
> data = new E[1] = 把301写在这张纸上
> ```
>
> 以后你要用这个房间：
>
> ```cpp
> data[0] = 10;  // 去门牌号301的房间，存入10
> *data = 10;    // 同上，两种写法等价
> ```
>
> ---
>
> # 所以赋值的不是"内存本身"
>
> ```cpp
> data = new E[1];
> ```
>
> 这里赋值的不是内存，而是内存的 **地址** 。
>
> 指针存的永远是地址，`new` 返回的也是地址，所以两者可以直接赋值，类型匹配。
>
> 清楚了吗？

> 好问题！这是指针最神奇的地方。
>
> ---
>
> # 指针和数组在C++里本质是同一件事
>
> 当你写：
>
> ```cpp
> E *data = new E[5];
> ```
>
> 内存里长这样：
>
> ```
> data存的地址：0x1000
>
> 0x1000: [ ] ← data[0]
> 0x1004: [ ] ← data[1]
> 0x1008: [ ] ← data[2]
> 0x100C: [ ] ← data[3]
> 0x1010: [ ] ← data[4]
> ```
>
> `new E[5]` 申请的是 **一块连续的空间** ，`data` 存的是这块空间 **第一个元素的地址** 。
>
> ---
>
> # `data[i]` 只是一个语法糖
>
> `data[i]` 在C++里实际上等价于：
>
> ```cpp
> data[i]  ==  *(data + i)
> ```
>
> 意思是：**从data的地址出发，往后移i个位置，然后取那里的值。**
>
> ```cpp
> data[0]  ==  *(data + 0)  // 第0个位置
> data[1]  ==  *(data + 1)  // 往后移1个
> data[2]  ==  *(data + 2)  // 往后移2个
> ```
>
> 图示：
>
> ```
> data → 0x1000
>
> data+0 → 0x1000 → data[0]
> data+1 → 0x1004 → data[1]
> data+2 → 0x1008 → data[2]
> ```
>
> ---
>
> # 所以指针没有"变成数组"
>
> `data` 始终是个指针，存的始终是地址。
>
> 只是C++允许你用 `data[i]` 这种数组语法来访问指针指向的连续内存，本质上是 `*(data + i)`。
>
> ```cpp
> E *data = new E[5];
>
> data[2] = 99;       // 写法一：数组语法
> *(data + 2) = 99;   // 写法二：指针语法
> // 完全等价！
> ```
>
> ---
>
> # 一句话总结
>
> ```
> 指针[i] = 从指针的地址出发，往后移i个位置取值
> 数组名本身就是指向第一个元素的指针
>
> 所以指针和数组可以用同样的语法访问
> ```
>
> 清楚了吗？

> 不是，这是那段代码里 **自己定义的函数** 。
>
> 你往上翻那段 `MyArrayList` 的代码，里面有这个函数：
>
> ```cpp
> void resize(int newCap) {
>     E *temp = new E[newCap];  // 申请新的更大的数组
>
>     for (int i = 0; i < size; i++) {
>         temp[i] = data[i];    // 把旧数据复制过去
>     }
>
>     delete[] data;            // 释放旧数组
>     data = temp;              // 指针指向新数组
>     cap = newCap;             // 更新容量
> }
> ```
>
> ---
>
> # 和vector的resize区分一下
>
> `vector` 有一个**内置的** `resize`：
>
> ```cpp
> vector<int> v(3);   // {0, 0, 0}
> v.resize(5);        // {0, 0, 0, 0, 0}
> ```
>
> 但那段代码里的 `resize` 是作者 **自己写的** ，用来模拟vector内部的扩容逻辑。
>
> ---
>
> # 怎么区分是内置还是自定义？
>
> ```cpp
> v.resize(5);      // 用 . 调用，是某个对象的方法（可能内置）
> resize(2 * cap);  // 直接调用，去当前类/文件里找定义
> ```
>
> 看到直接调用的函数名，先在当前代码里搜一下，找不到再去想是不是内置的。

> 好，我用图一步一步带你走一遍。
>
> ---
>
> # 场景设定
>
> 假设现在数组已经满了：
>
> ```
> cap = 3，size = 3
> data → [1] [2] [3]
>         ↑
>       地址0x1000
> ```
>
> 现在要扩容到 `newCap = 6`，调用 `resize(6)`。
>
> ---
>
> # 第一步：申请新的更大的数组
>
> ```cpp
> E *temp = new E[newCap];
> ```
>
> ```
> temp → [ ] [ ] [ ] [ ] [ ] [ ]
>         ↑
>       地址0x2000（新申请的）
>
> data → [1] [2] [3]
>         ↑
>       地址0x1000（旧的还在）
> ```
>
> ---
>
> # 第二步：把旧数据复制过去
>
> ```cpp
> for (int i = 0; i < size; i++) {
>     temp[i] = data[i];
> }
> ```
>
> ```
> temp → [1] [2] [3] [ ] [ ] [ ]
>         ↑
>       地址0x2000
>
> data → [1] [2] [3]
>         ↑
>       地址0x1000（旧的还在）
> ```
>
> ---
>
> # 第三步：释放旧数组
>
> ```cpp
> delete[] data;
> ```
>
> ```
> temp → [1] [2] [3] [ ] [ ] [ ]
>         ↑
>       地址0x2000
>
> 地址0x1000：已释放，不能再用了
> ```
>
> ---
>
> # 第四步：把data指向新数组
>
> ```cpp
> data = temp;
> ```
>
> ```
> data → [1] [2] [3] [ ] [ ] [ ]
>         ↑
>       地址0x2000（data现在指向新数组）
>
> temp也指向0x2000，但temp是局部变量，函数结束就消失了
> ```
>
> ---
>
> # 第五步：更新容量
>
> ```cpp
> cap = newCap;   // cap从3更新为6
> ```
>
> ---
>
> # 整个过程总结
>
> ```
> 扩容前：
> data → [1][2][3]          cap=3
>
> 扩容中：
> temp → [1][2][3][ ][ ][ ] （新数组）
> data → [1][2][3]           （旧数组，即将释放）
>
> 扩容后：
> data → [1][2][3][ ][ ][ ] cap=6
> ```
>
> 本质就是： **开新房子→搬家→拆旧房子→更新地址** 。
>
> 这也是 `vector` 内部扩容的原理，只不过 `vector` 帮你自动做了这一切。
>
> 清楚了吗？

---

# 总结速查

| 符号/关键字              | 含义             | 例子           |
| ------------------------ | ---------------- | -------------- |
| `int &ref = a`         | 声明引用（别名） | ref就是a       |
| `&a`                   | 取a的地址        | 得到一个地址值 |
| `int *p`               | 声明指针         | p存地址        |
| `*p`                   | 解引用           | 取p指向的值    |
| `new T[n]`             | 申请堆内存       | 手动管理       |
| `delete[] p`           | 释放堆内存       | 必须手动做     |
| `class`                | 定义类           | 数据+函数打包  |
| `private`              | 内部成员         | 外部不能访问   |
| `public`               | 公开接口         | 外部可以访问   |
| `this->`               | 指向自己         | 相当于self     |
| `template<typename T>` | 类型占位符       | T用时再确定    |

---

**竞赛中的实际优先级：**

```
必须掌握：引用(&)、vector、STL
了解原理：指针(*)、new/delete
看懂即可：class、template（竞赛中直接用STL，不需要手写）
```

哪个还不清楚可以继续问，清楚了我们开始刷题！
