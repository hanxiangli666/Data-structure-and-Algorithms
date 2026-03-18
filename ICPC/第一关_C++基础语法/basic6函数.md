代码本身完全正确！逻辑没问题，语法也对。

不过有一个小问题： **缺少完整的程序框架** 。

你写的只是核心逻辑，但C++程序必须有完整结构才能编译运行。C++ 的编译器不像 Python 解释器那样可以直接执行零散的代码行，它需要一个入口点——也就是 `main` 函数——才知道从哪里开始执行。*就像只写了文章正文，缺了题目和结尾，整篇文章是不完整的。*

```cpp
#include <iostream>
using namespace std;

int main() {
    int a, b;
    while (cin >> a >> b) {
        cout << a + b << endl;
    }
    return 0;
}
```

另外，这道题的输入是**固定的一行** `3 5`，只读一次，所以其实不需要 `while`：

```cpp
#include <iostream>
using namespace std;

int main() {
    int a, b;
    cin >> a >> b;
    cout << a + b << endl;
    return 0;
}
```

你用 `while` 版本也不算错，只是 `while` 是"不知道有多少组输入"时用的。这道题只有一组，用 `while` 有点大材小用。

**记住这个习惯：** 看到题目说"多组输入"或"读到EOF"才用 `while(cin >> ...)`，否则直接 `cin >> a >> b` 就够了。

> ✅ 能写出正确的核心逻辑，说明思路没问题，框架问题是初学阶段最容易忽略的，多写几次就成习惯了！

---

总体来说写得很好，思路是对的。

下面进入下一个知识点： **函数** 。这个掌握了之后，写稍微复杂的题就方便很多。

---

# 第二课：函数

## 基本写法

```cpp
返回类型 函数名(参数列表) {
    // 函数体
    return 返回值;
}
```

一个简单例子：

```cpp
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(3, 5);
    cout << result << endl;  // 输出：8
    return 0;
}
```

对应Python的：

```python
def add(a, b):
    return a + b
```

区别就是C++要**声明每个参数的类型**和 **返回值的类型** 。这不是繁琐，而是必要的——编译器需要在编译期就知道每个变量占几个字节、如何在寄存器里传递，类型信息缺一不可。

> 💡 **老师提醒：** 函数的**返回类型**和 `return` 后面的值类型必须匹配。如果你声明了 `int` 函数却返回了一个 `string`，编译器会报错；如果非要用 `return 3.14` 返回给 `int`，小数部分会被静默截断。养成习惯：写函数签名的时候就想好"我要返回什么类型"。

---

## 没有返回值用 void

```cpp
void printSum(int a, int b) {
    cout << a + b << endl;
    // 不需要 return
}
```

`void` 字面意思是"空"，告诉编译器这个函数不返回任何值。不写 `return` 也没关系，函数执行到末尾自动结束。如果你确实想提前结束一个 `void` 函数，可以写 `return;`（不带任何值）。

---

## 函数里传引用（之前讲过）

```cpp
void addOne(int &x) {
    x++;   // 直接修改原变量
}

int main() {
    int a = 5;
    addOne(a);
    cout << a << endl;  // 输出：6
    return 0;
}
```

传引用和传值的区别，之前已经讲透了。这里只记住：**想在函数内部修改外部变量的值，就加 `&`**，否则函数里改的只是副本，外面看不到效果。

> 💡 **老师提醒：** 对于大型对象（比如 `vector`、`string`），即使你不打算修改它，也建议用 `const &` 传入，避免复制开销：`void process(const vector<int> &v)`。竞赛里这个细节有时会决定是否超时。

---

## 竞赛中函数的典型用法

竞赛中函数主要用来 **把重复逻辑封装起来** ，让代码更清晰。一道题里如果同一段逻辑要用三次，就值得写成函数；如果只用一次，直接写在 `main` 里就行，不要过度封装。

```cpp
#include <iostream>
using namespace std;

// 判断一个数是否是偶数
bool isEven(int n) {
    return n % 2 == 0;
}

int main() {
    int n;
    cin >> n;
    if (isEven(n)) {
        cout << "even" << endl;
    } else {
        cout << "odd" << endl;
    }
    return 0;
}
```

> ✅ 函数是组织代码的核心工具，能写出结构清晰的函数，说明你对问题的拆解能力已经上了一个台阶！

---

## 现在你来练习

> **题目：** 写一个函数 `maxOfThree`，接收三个整数，返回其中最大的那个。然后在 main 里读取三个数，调用这个函数，输出结果。
>
> 输入：`3 7 5`
> 输出：`7`

试着写写看！
