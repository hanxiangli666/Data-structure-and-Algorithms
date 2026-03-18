# 第三课：数组和字符串

## 一、原始数组（了解即可）

C++有两种数组，先说最基础的：

```cpp
int arr[5];                    // 声明一个大小为5的int数组
int arr2[5] = {1, 2, 3, 4, 5}; // 声明并初始化
```

但是这种数组有个大问题： **大小固定，不能动态调整** 。它在编译期就确定了大小，运行时无法扩容，如果你不知道数据有多少个，用它就会很麻烦——要么开太大浪费内存，要么开太小导致越界崩溃。*就像原始数组和 vector 的区别，就像事先订好固定人数的包间，和随时可以加椅子的大厅的区别——人数确定时包间没问题，人数不定时大厅才灵活。*

所以竞赛中 **统一用 `vector`** ，不用原始数组（除非题目有特殊要求）。

你之前已经学过vector了，这里不重复。

---

## 二、字符串 string

这个在竞赛中非常高频，要重点掌握。

### 基本操作

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "hello";

    // 长度
    cout << s.size() << endl;    // 5

    // 访问单个字符（和数组一样用下标）
    cout << s[0] << endl;        // 'h'
    cout << s[1] << endl;        // 'e'

    // 拼接
    string t = "world";
    string combined = s + " " + t;
    cout << combined << endl;    // "hello world"

    // 比较（可以直接用 == != < >）
    cout << (s == t) << endl;    // 0 (false)
    cout << (s < t) << endl;     // 1 (true)，按字典序比较

    return 0;
}
```

> 💡 **老师提醒：** `s.size()` 返回的是 `size_t` 类型，是**无符号整数**。如果你写 `s.size() - 1` 而 `s` 是空字符串，结果不是 `-1` 而是一个超大正数（无符号数下溢）！遇到需要 `s.size() - 1` 的场合，先用 `(int)s.size()` 强转成有符号整数更安全。

> ✅ string 的拼接、比较、下标访问这三个操作最常用，熟悉它们，字符串题就有了基本盘！

### 常用方法

```cpp
string s = "hello world";

// 截取子串：substr(起始位置, 长度)
string sub = s.substr(0, 5);   // "hello"
string sub2 = s.substr(6);     // "world"（从6到结尾）

// 查找：find()，找不到返回 string::npos
int pos = s.find("world");     // 6
int pos2 = s.find("xyz");      // string::npos（找不到）

if (s.find("world") != string::npos) {
    cout << "找到了" << endl;
}

// 遍历
for (char c : s) {
    cout << c << " ";
}
// 输出：h e l l o   w o r l d
```

`string::npos` 是一个特殊值，表示"没找到"。它的实际值是 `size_t` 类型的最大值，所以你不需要记这个数字，只需要记得用 `!= string::npos` 来判断是否找到了。

> 💡 **老师提醒：** 用 `find()` 的返回值时，不要把它存进 `int` 再判断——`string::npos` 是无符号数的最大值，存进 `int` 会变成 `-1`，虽然 `!= -1` 的判断凑巧也能工作，但语义不清晰。竞赛里直接 `s.find(...) != string::npos` 最安全。

### 字符和ASCII码

竞赛中经常要判断一个字符是字母还是数字：

```cpp
char c = 'A';

// 字符可以直接和整数运算（因为字符底层就是数字）
cout << (int)c << endl;     // 65（A的ASCII码）
cout << (int)'a' << endl;   // 97
cout << (int)'0' << endl;   // 48

// 常用技巧：大写转小写
char lower = c + 32;        // 'a'
// 或者直接
char lower2 = tolower(c);   // 'a'
char upper = toupper('a');   // 'A'

// 判断字符类型
isalpha(c);   // 是否是字母
isdigit(c);   // 是否是数字
islower(c);   // 是否是小写字母
isupper(c);   // 是否是大写字母
```

`char` 在内存里就是一个 1 字节的整数，`'A'` 存的就是数字 65，`'a'` 存的就是 97，所以 `'a' - 'A' == 32`。大写转小写加 32、小写转大写减 32，背后的原因就是 ASCII 表里大小写字母之间正好差了 32。*就像字符和数字的关系，就像音符和频率的关系——听起来是音乐，底层全是数学。*

> 💡 **老师提醒：** `c + 32` 这种写法有个隐患——如果 `c` 本来就是小写字母，再加 32 就超出了字母范围。竞赛里更安全的做法是先判断 `isupper(c)` 再转换，或者直接用 `tolower(c)`，它会自己检查。

> ✅ ASCII 码的规律记住了，字符处理类的题（判断大小写、统计字母频率）直接就能上手，非常高频！

### 字符串和数字互转

```cpp
#include <string>

// 数字 → 字符串
string s = to_string(123);    // "123"
string s2 = to_string(3.14);  // "3.140000"

// 字符串 → 数字
int n = stoi("123");          // 123
double d = stod("3.14");      // 3.14
```

`to_string` 和 `stoi` 是 C++11 引入的，之前 C++ 程序员要用 `stringstream` 来做这件事，非常繁琐。现在直接用这两个函数就行，简洁很多。

> 💡 **老师提醒：** `stoi` 如果传入的字符串不是合法数字（比如 `"abc"`），会抛出异常导致程序崩溃。竞赛里输入一般是合法的，不用担心；但如果题目输入可能不规范，要注意这个问题。

---

## 三、读取字符串输入

```cpp
// 读一个单词（遇到空格停止）
string s;
cin >> s;

// 读一整行（包括空格）
string line;
getline(cin, line);
```

⚠️ **一个常见坑：**

```cpp
int n;
cin >> n;
getline(cin, line);   // ❌ 这里会读到一个空行！
```

原因是 `cin >> n` 读完数字后，回车符还留在缓冲区，`getline` 会把这个回车当成一行读走。这是 C++ I/O 里最经典的陷阱，几乎每个初学者都踩过一次。*就像 `cin >> n` 和 `getline` 混用，就像上一个人用完洗手台没关水，你过来直接被水喷了——上一个操作的"尾巴"影响了你的操作。*

解决方法：

```cpp
int n;
cin >> n;
cin.ignore();          // 忽略掉那个残留的回车
getline(cin, line);    // ✅ 现在才是真正的下一行
```

> 💡 **老师提醒：** `cin.ignore()` 默认只忽略一个字符。如果你不确定缓冲区里有多少残留，可以用 `cin.ignore(numeric_limits<streamsize>::max(), '\n')` 清空到下一个换行符，但竞赛里通常 `cin.ignore()` 一个就够了。

> ✅ getline 的坑记住了，以后遇到"先读数字、再读整行"的题，直接加 `cin.ignore()`，不会再被坑！

---

## 现在你来练习

> **题目：** 输入一个字符串，统计其中有多少个字母是大写的，输出数量。
>
> 输入：`HeLLo WorLD`
> 输出：`6`

自己写，发给我！
