# 第三课：数组和字符串

## 一、原始数组（了解即可）

C++有两种数组，先说最基础的：

```cpp
int arr[5];                    // 声明一个大小为5的int数组
int arr2[5] = {1, 2, 3, 4, 5}; // 声明并初始化
```

但是这种数组有个大问题： **大小固定，不能动态调整** 。

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

原因是 `cin >> n` 读完数字后，回车符还留在缓冲区，`getline` 会把这个回车当成一行读走。

解决方法：

```cpp
int n;
cin >> n;
cin.ignore();          // 忽略掉那个残留的回车
getline(cin, line);    // ✅ 现在才是真正的下一行
```

---

## 现在你来练习

> **题目：** 输入一个字符串，统计其中有多少个字母是大写的，输出数量。
>
> 输入：`HeLLo WorLD`
> 输出：`6`

自己写，发给我！
