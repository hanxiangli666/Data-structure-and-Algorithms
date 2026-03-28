# C++ 基础语法

C++ 是在 C 语言基础上发展而来的，增加了面向对象编程、模板、异常处理等特性，同时拥有非常强大的标准库（STL）。因为执行效率高、标准库完善，C++ 是算法竞赛和系统开发中最受欢迎的语言之一。

本站所有的算法代码都配有 C++ 版本，你学完基础之后可以在刷题过程中熟悉和应用这门语言。下面我们就开始学习 C++ 的基础语法。

## Hello World 与程序结构

先看一个最简单的 C++ 程序：

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
```


拆解一下这个结构：

```cpp
#include <iostream>      // 引入输入输出库
using namespace std;     // 使用标准命名空间

int main() {             // 程序入口函数
    // 你的代码写在这里
    return 0;            // 返回 0 表示程序正常结束
}
```

`#include <iostream>` 是预处理指令，告诉编译器引入输入输出库（**i**nput/**o**utput  **stream** ），这样你才能用 `cout`、`cin` 这些功能。

`using namespace std;` 是告诉编译器你要使用标准命名空间。不写这行的话，每次使用标准库的东西都要加 `std::` 前缀，比如 `std::cout`、`std::endl`。加了这行就可以省略前缀，写起来方便很多。

`int main()` 是程序的入口函数，C++ 程序运行时会自动找到并执行这个函数。`int` 表示这个函数返回一个整数，`return 0` 表示程序正常结束。

每条语句末尾要加分号 `;`，这是 C++ 的规矩。漏了分号编译器会报错，这是新手最常犯的错误之一。


## 标准输出 cout

`cout` 配合 `<<` 运算符（插入运算符）来输出内容，`endl` 表示换行：

```cpp
#include <iostream>
using namespace std;

int main() {
    // 输出字符串，endl 换行
    // 输出：Hello
    cout << "Hello" << endl;

    // 输出整数
    int age = 25;
    // 输出：年龄是 25
    cout << "年龄是 " << age << endl;

    // 输出浮点数
    double pi = 3.14159;
    // 输出：pi = 3.14159
    cout << "pi = " << pi << endl;

    // 链式输出：用多个 << 连接
    int x = 10, y = 20;
    // 输出：x = 10, y = 20
    cout << "x = " << x << ", y = " << y << endl;

    // 输出单个字符
    char c = 'A';
    // 输出：字符：A
    cout << "字符：" << c << endl;

    return 0;
}
```


`cout` 比 C 语言的 `printf` 方便的地方在于不用记格式化占位符（`%d`、`%f` 之类的），直接用 `<<` 把变量丢进去就行，编译器会自动根据类型来输出。

`endl` 和 `"\n"` 都能换行，区别是 `endl` 会多一步刷新缓冲区的操作。在算法题中大量输出时，用 `"\n"` 会比 `endl` 快一些，不过一般不用太在意。

如果想控制浮点数的小数位数，需要引入 `<iomanip>` 头文件：

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double pi = 3.14159265;

    // 默认输出
    // 输出：3.14159
    cout << pi << endl;

    // fixed + setprecision 控制小数位数
    // 输出：3.14
    cout << fixed << setprecision(2) << pi << endl;

    // 输出：3.1416（四舍五入到 4 位小数）
    cout << fixed << setprecision(4) << pi << endl;

    return 0;
}
```


`fixed` 表示用定点小数格式（而不是科学计数法），`setprecision(n)` 指定保留 n 位小数。设置之后对后续所有输出都生效，不需要每次都写。

## 读取输入 cin

`cin` 配合 `>>` 运算符（提取运算符）来读取输入：

```cpp
#include <iostream>
using namespace std;

int main() {
    string name;
    int age;

    // 读取一个字符串和一个整数
    cin >> name >> age;

    // 输出：姓名：Alice
    cout << "姓名：" << name << endl;
    // 输出：年龄：25
    cout << "年龄：" << age << endl;

    return 0;
}
```


`cin >>` 默认以空格和换行作为分隔符，所以输入 `Alice 25` 时，`name` 读到 `Alice`，`age` 读到 `25`。

上面的示例中，左下角的 stdin 区域就是模拟的用户输入，你可以修改输入内容再运行试试。

如果需要读取一整行（包括空格），可以用 `getline`：

```cpp
#include <iostream>
using namespace std;

int main() {
    string line;
    // 读取一整行（包括空格）
    getline(cin, line);
    // 输出：Hello World 你好
    cout << line << endl;
    return 0;
}
```


注意一个坑：如果在 `cin >>` 之后紧接着用 `getline`，需要先调用 `cin.ignore()` 清掉缓冲区里残留的换行符，否则 `getline` 会读到一个空行。

### 循环读取到输入结束

有些题目的输入数据量不确定，需要一直读取到输入结束（EOF）。把 `cin >>` 放到 `while` 的条件中就行：

```cpp
#include <iostream>
using namespace std;

int main() {
    // 不断读取整数，直到输入结束
    int x;
    while (cin >> x) {
        cout << x << "\n";
    }
    return 0;
}
```

```cpp
#include <iostream>
using namespace std;

int main() {
    // 不断读取两个整数，直到输入结束
    int a, b;
    while (cin >> a >> b) {
        cout << a + b << "\n";
    }
    return 0;
}
```


当输入结束时，`cin >>` 会返回一个"假"值，`while` 循环自动退出。

## 变量与数据类型

C++ 最常用的数据类型：

| 类型          | 说明           | 示例                            |
| ------------- | -------------- | ------------------------------- |
| `int`       | 整数（32位）   | `int a = 10;`                 |
| `long long` | 长整数（64位） | `long long b = 9999999999LL;` |
| `double`    | 浮点数（小数） | `double c = 3.14;`            |
| `char`      | 单个字符       | `char d = 'A';`               |
| `bool`      | 布尔值         | `bool e = true;`              |
| `string`    | 字符串         | `string s = "hello";`         |

```cpp
#include <iostream>
using namespace std;

int main() {
    // 整数
    int a = 42;
    // 长整数，注意末尾的 LL
    long long b = 9999999999LL;
    // 浮点数
    double c = 3.14;
    // 单个字符，用单引号
    char d = 'A';
    // 布尔值，true 或 false
    bool e = true;
    // 字符串，用双引号
    string s = "hello";

    cout << "int: " << a << endl;
    cout << "long long: " << b << endl;
    cout << "double: " << c << endl;
    cout << "char: " << d << endl;
    // bool 输出是 1（true）或 0（false）
    cout << "bool: " << e << endl;
    cout << "string: " << s << endl;

    return 0;
}
```


几个注意点：

`long long` 类型的字面量要加 `LL` 后缀（如 `9999999999LL`），否则编译器可能把它当成 `int`，超出范围就会出错。

`char` 用单引号 `'A'`，`string` 用双引号 `"Hello"`，这两个不能搞混。

`bool` 在 C++ 中是内置类型，不需要额外引入头文件。`true` 对应 `1`，`false` 对应 `0`。

`string` 是 C++ 标准库提供的字符串类型，比 C 语言的字符数组好用太多了——可以直接用 `+` 拼接、用 `==` 比较、用 `.size()` 获取长度，不用操心内存管理。

### 变量声明

C++ 是静态类型语言，变量必须先声明类型才能使用：

```cpp
// 先声明，后赋值
int x;
x = 10;

// 声明的同时赋值
int y = 20;

// 一行声明多个同类型变量
int a = 1, b = 2, c = 3;
```


### 常量

用 `const` 声明常量，声明后就不能修改了：

```
const int MAX_SIZE = 100;
const double PI = 3.14159;
```



## auto 类型推断

C++11 引入了 `auto` 关键字，让编译器自动推断变量的类型：

```cpp
#include <iostream>
using namespace std;

int main() {
    auto a = 42;          // 推断为 int
    auto b = 3.14;        // 推断为 double
    auto c = 'A';         // 推断为 char
    auto d = true;        // 推断为 bool
    auto s = string("hello"); // 推断为 string

    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << d << endl;
    cout << s << endl;

    return 0;
}
```


`auto` 本质上就是个语法糖，编译器在编译时就确定了类型，运行时和你手写类型没有任何区别。目前用处不大，但后面学到 STL 容器的迭代器时，类型名会特别长，用 `auto` 就省事多了。

## 数组基础

数组用来存储一组相同类型的数据，通过下标（索引）访问，下标从 `0` 开始：

```cpp
#include <iostream>
using namespace std;

int main() {
    // 声明并初始化
    int nums[] = {10, 20, 30, 40, 50};

    // 通过下标访问，下标从 0 开始
    // 输出：10
    cout << nums[0] << endl;
    // 输出：50
    cout << nums[4] << endl;

    // 修改元素
    nums[0] = 99;

    // 数组长度：用 sizeof 计算
    // sizeof(nums) 是整个数组的字节数，sizeof(nums[0]) 是单个元素的字节数
    // 两者相除就是元素个数
    int len = sizeof(nums) / sizeof(nums[0]);
    // 输出：5
    cout << len << endl;

    // 遍历数组
    // 输出：99 20 30 40 50
    for (int i = 0; i < len; i++) {
        cout << nums[i] << " ";
    }
    cout << endl;

    // 指定大小创建，用 {0} 把所有元素初始化为 0
    int arr[5] = {0};
    // 输出：0 0 0 0 0
    for (int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
```


C 风格数组的大小在创建后就固定了，不能动态增删元素。后面学到 STL 的 `vector` 之后，你就有动态数组可以用了，比这个灵活得多。

## 类型转换

小类型可以自动转换为大类型（不丢精度），反过来可能丢失数据：


```cpp
#include <iostream>
using namespace std;

int main() {
    // 隐式转换：int -> long long -> double
    int a = 100;
    long long b = a;
    double c = b;
    cout << a << " " << b << " " << c << endl;
    // 输出：100 100 100

    // 强制转换：double -> int（截断小数部分）
    double pi = 3.99;
    int rounded = (int)pi;
    // 输出：3（不是四舍五入，是直接截断）
    cout << rounded << endl;

    // 整数除法的坑
    // 输出：0（两个 int 相除，结果还是 int）
    cout << 1 / 2 << endl;
    // 输出：0.5（其中一个是 double，结果就是 double）
    cout << 1.0 / 2 << endl;

    return 0;
}
```



特别注意整数除法：`1 / 2` 的结果是 `0` 而不是 `0.5`，因为两个 `int` 相除结果还是 `int`，小数部分直接丢掉。这在算法题中是个常见的坑。

解决办法：让其中一个操作数变成 `double`，比如写成 `1.0 / 2`，或者用强制转换 `(double)a / b`。

## 运算符

### 算术运算符

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 17, b = 5;

    // 输出：22
    cout << a + b << endl;
    // 输出：12
    cout << a - b << endl;
    // 输出：85
    cout << a * b << endl;
    // 输出：3（整数除法，截断小数）
    cout << a / b << endl;
    // 输出：2（取余数，17 = 5 * 3 + 2）
    cout << a % b << endl;

    // 自增自减
    int c = 10;
    c++;
    // 输出：11
    cout << c << endl;
    c--;
    // 输出：10
    cout << c << endl;

    // 复合赋值
    c += 5;
    // 输出：15
    cout << c << endl;
    c *= 2;
    // 输出：30
    cout << c << endl;

    return 0;
}
```



取余运算符 `%` 在算法题中非常常用，比如判断奇偶（`n % 2 == 0`）、循环数组索引（`i % n`）等。

### 比较与逻辑运算符

比较运算符返回 `true` 或 `false`：`==`（等于）、`!=`（不等于）、`<`、`>`、`<=`、`>=`。

逻辑运算符用于组合条件：`&&`（与）、`||`（或）、`!`（非）。

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10, b = 20;

    // 比较运算符
    // 输出：0（false）
    cout << (a == b) << endl;
    // 输出：1（true）
    cout << (a != b) << endl;
    // 输出：1（true）
    cout << (a < b) << endl;

    // 逻辑运算符
    // &&：两个都为真才是真
    // 输出：1
    cout << (a > 5 && b > 15) << endl;
    // ||：有一个为真就是真
    // 输出：1
    cout << (a > 100 || b > 15) << endl;
    // !：取反
    // 输出：0
    cout << !(a > 5) << endl;

    return 0;
}
```



### 三元运算符

`条件 ? 值1 : 值2` 是 `if-else` 的简写形式，条件为真返回值1，否则返回值2：

```cpp
int a = 10, b = 20;
int maxVal = (a > b) ? a : b;  // maxVal = 20
```


### 位运算

位运算在处理二进制相关问题时非常高效，在算法题中可能会遇到。这里了解一下基本用法就行：

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;  // 二进制 1010
    int b = 12;  // 二进制 1100

    // 按位与：两个都是 1 才是 1
    // 1010 & 1100 = 1000（十进制 8）
    // 输出：8
    cout << (a & b) << endl;

    // 按位或：有一个是 1 就是 1
    // 1010 | 1100 = 1110（十进制 14）
    // 输出：14
    cout << (a | b) << endl;

    // 按位异或：不同为 1，相同为 0
    // 1010 ^ 1100 = 0110（十进制 6）
    // 输出：6
    cout << (a ^ b) << endl;

    // 左移：相当于乘以 2
    // 输出：20
    cout << (a << 1) << endl;
    // 右移：相当于除以 2
    // 输出：5
    cout << (a >> 1) << endl;

    // 一个经典用法：判断奇偶
    // n & 1 等价于 n % 2
    int n = 7;
    if ((n & 1) == 1) {
        cout << n << " 是奇数" << endl;
    }

    return 0;
}
```



## 控制流

### 条件判断 if / else

```cpp
#include <iostream>
using namespace std;

int main() {
    int score = 85;

    if (score >= 90) {
        cout << "优秀" << endl;
    } else if (score >= 60) {
        cout << "及格" << endl;
    } else {
        cout << "不及格" << endl;
    }
    // 输出：及格

    return 0;
}
```



### for 循环

`for` 循环是最常用的循环，由三部分组成：`for (初始化; 条件; 更新)`。

```cpp
#include <iostream>
using namespace std;

int main() {
    // 基本 for 循环
    // 输出：0 1 2 3 4
    for (int i = 0; i < 5; i++) {
        cout << i << " ";
    }
    cout << endl;

    // 倒序遍历
    // 输出：5 4 3 2 1
    for (int i = 5; i >= 1; i--) {
        cout << i << " ";
    }
    cout << endl;

    // 嵌套循环：打印乘法表的一小部分
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            cout << i * j << "\t";
        }
        cout << endl;
    }

    return 0;
}
```


### while 循环

`while` 循环适合循环次数不确定的场景：

```cpp
#include <iostream>
using namespace std;

int main() {
    // 计算 1 + 2 + ... + 100
    int sum = 0;
    int i = 1;
    while (i <= 100) {
        sum += i;
        i++;
    }
    // 输出：5050
    cout << sum << endl;

    // do-while：至少执行一次
    int n = 0;
    do {
        cout << "执行了一次，n = " << n << endl;
    } while (n > 0);
    // 虽然 n = 0 不满足条件，但还是会执行一次

    return 0;
}
```


`do-while` 和 `while` 的区别是：`do-while` 先执行一次再判断条件，所以循环体至少会执行一次。实际中 `do-while` 用得很少，了解一下就行。

### range-based for（for-each）

C++11 引入了基于范围的 `for` 循环，可以更简洁地遍历数组和容器：

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    // 遍历数组
    int nums[] = {10, 20, 30, 40, 50};
    // 输出：10 20 30 40 50
    for (int x : nums) {
        cout << x << " ";
    }
    cout << endl;

    // 遍历字符串中的每个字符
    string s = "hello";
    // 输出：h e l l o
    for (char c : s) {
        cout << c << " ";
    }
    cout << endl;

    // 用引用修改数组中的元素
    for (int& x : nums) {
        x *= 2;
    }
    // 输出：20 40 60 80 100
    for (int x : nums) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
```


`for (int x : nums)` 的意思是"对于 `nums` 中的每个元素，依次赋值给 `x`"。如果想在循环中修改原数组的元素，需要用引用 `int& x`，否则 `x` 只是元素的一个副本，修改它不会影响原数组。

这里也可以用 `auto` 来简化类型声明：`for (auto x : nums)` 或 `for (auto& x : nums)`。

### break 和 continue

`break` 跳出整个循环，`continue` 跳过当前这轮、进入下一轮：

```cpp
#include <iostream>
using namespace std;

int main() {
    // break：找到第一个能被 7 整除的数就停止
    for (int i = 1; i <= 100; i++) {
        if (i % 7 == 0) {
            cout << "第一个能被 7 整除的数：" << i << endl;
            break;
        }
    }

    // continue：跳过偶数，只打印奇数
    // 输出：1 3 5 7 9
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) {
            continue;
        }
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
```


### switch 语句

`switch` 适合对一个变量做多个值的判断，比一长串 `if-else` 更清晰：

```cpp
#include <iostream>
using namespace std;

int main() {
    int day;
    cin >> day;

    switch (day) {
        case 1:
            cout << "星期一" << endl;
            break;
        case 2:
            cout << "星期二" << endl;
            break;
        case 3:
            cout << "星期三" << endl;
            break;
        case 4:
            cout << "星期四" << endl;
            break;
        case 5:
            cout << "星期五" << endl;
            break;
        case 6:
            cout << "星期六" << endl;
            break;
        case 7:
            cout << "星期日" << endl;
            break;
        default:
            cout << "无效的输入" << endl;
            break;
    }

    return 0;
}
```



注意每个 `case` 后面要加 `break`，否则会"穿透"到下一个 `case` 继续执行。你可以试试把 `break` 去掉，看看会发生什么。

## 小结

这篇介绍了 C++ 的基础语法：程序结构、输入输出、数据类型、运算符和控制流。这些是所有 C++ 代码的基石。

下一篇我们来详细讲讲 C++ 的头文件和标准库，看看 C++ 的头文件和 C 有什么不同，`using namespace std` 是怎么回事，以及刷题中超好用的万能头文件。
