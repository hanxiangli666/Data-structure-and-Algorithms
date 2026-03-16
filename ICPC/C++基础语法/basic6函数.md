代码本身完全正确！逻辑没问题，语法也对。

不过有一个小问题： **缺少完整的程序框架** 。

你写的只是核心逻辑，但C++程序必须有完整结构才能编译运行：

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

区别就是C++要**声明每个参数的类型**和 **返回值的类型** 。

---

## 没有返回值用 void

```cpp
void printSum(int a, int b) {
    cout << a + b << endl;
    // 不需要 return
}
```

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

---

## 竞赛中函数的典型用法

竞赛中函数主要用来 **把重复逻辑封装起来** ，让代码更清晰：

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

---

## 现在你来练习

> **题目：** 写一个函数 `maxOfThree`，接收三个整数，返回其中最大的那个。然后在 main 里读取三个数，调用这个函数，输出结果。
>
> 输入：`3 7 5`
> 输出：`7`

试着写写看！
