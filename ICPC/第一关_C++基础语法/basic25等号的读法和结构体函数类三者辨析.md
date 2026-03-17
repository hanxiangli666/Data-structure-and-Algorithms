好，两个问题分开讲清楚。

---

# 第一个问题：等号怎么理解

## 永远从右往左读

```cpp
data = temp;
```

读法：**把右边的值，存进左边的变量。**

```
右边：temp（一个地址，比如0x2000）
左边：data（一个指针变量）
意思：把0x2000存进data
```

再看几个例子，全部从右往左：

```cpp
int a = 10;         // 把10，存进a
int b = a;          // 把a的值，存进b
cap = newCap;       // 把newCap的值，存进cap
data = temp;        // 把temp存的地址，存进data
a = a + 1;          // 把a+1的结果，存进a
```

**永远是右边算出一个值，塞给左边。左边只能是一个"容器"（变量）。**

---

## 和数学等号的区别

数学里：`a = b` 意思是a和b相等，是双向的。

C++里：`a = b` 意思是把b的值复制给a，是单向的。

```cpp
// 数学里 a = a + 1 没有意义
// C++里完全合法，意思是：
// 先算右边 a+1，再把结果存进左边的a
a = a + 1;
```

---

## 一个判断技巧

看到等号，问自己两个问题：

```
1. 右边算出来的是什么？
2. 左边是哪个容器接收它？
```

```cpp
data = temp;
// 右边：temp是个指针，存着地址0x2000
// 左边：data是个指针，接收这个地址
// 结果：data也指向0x2000
```

```cpp
cap = newCap;
// 右边：newCap是个int，值是6
// 左边：cap是个int，接收6
// 结果：cap变成6
```

---

# 第二个问题：结构体、函数、类怎么区分

你混淆的根本原因是：**名字长了之后就不知道这个名字代表什么东西了。**

我给你一个判断方法。

---

## 看定义时的关键字

```cpp
struct Point { ... };      // struct开头 → 结构体
class MyList { ... };      // class开头 → 类
int power(int a) { ... }   // 类型+名字+括号 → 函数
```

---

## 看使用时的形态

```cpp
// 结构体/类：用来创建变量
Point p;               // p是一个Point结构体变量
MyList<int> lst;       // lst是一个MyList类的变量

// 函数：后面有括号，直接调用
power(2, 10);          // 调用函数
resize(2 * cap);       // 调用函数

// 成员访问：用 . 访问内部的东西
p.x = 3;              // 访问结构体的成员变量
lst.addLast(1);        // 调用类的成员函数
```

---

## 用那段复杂代码举例

```cpp
template<typename E>
class MyArrayList {        // ← class，这是一个类的定义
private:
    E *data;               // ← 类的成员变量
    int size;              // ← 类的成员变量
    int cap;               // ← 类的成员变量

public:
    MyArrayList() { }      // ← 构造函数（特殊函数，名字和类一样）

    void addLast(E e) { }  // ← 类的成员函数

    void resize(int newCap) {   // ← 类的成员函数
        E *temp = new E[newCap]; // temp是局部变量
        data = temp;             // data是成员变量
        cap = newCap;            // cap是成员变量
    }
};
```

区分成员变量和局部变量的方法：

```
成员变量：定义在class/struct的{}里，函数外面
局部变量：定义在某个函数的{}里面
```

```cpp
class MyArrayList {
    int cap;              // 成员变量，整个对象都能用

    void resize(int newCap) {    // newCap是参数（局部）
        E *temp = new E[newCap]; // temp是局部变量
        cap = newCap;    // cap是成员变量，不用声明直接用
    }
};
```

---

## 一张总结表

| 东西     | 定义时长什么样            | 使用时长什么样    |
| -------- | ------------------------- | ----------------- |
| 结构体   | `struct 名字 {}`        | `名字 变量名`   |
| 类       | `class 名字 {}`         | `名字 变量名`   |
| 函数     | `返回类型 名字(参数){}` | `名字(参数)`    |
| 成员变量 | 在class/struct里，函数外  | `对象.变量名`   |
| 成员函数 | 在class/struct里          | `对象.函数名()` |
| 局部变量 | 在某个函数里              | 直接用变量名      |

---

## 实际读代码的方法

遇到一个陌生的名字，往上找它的定义：

```cpp
void resize(int newCap) {
    E *temp = new E[newCap];
    data = temp;   // data是什么？
}
```

往上找 `data` 的定义：

```cpp
class MyArrayList {
private:
    E *data;    // 找到了！data是类的成员变量，类型是E*
```

这样就清楚了。

---

清楚了吗？清楚了我们开始刷题！
