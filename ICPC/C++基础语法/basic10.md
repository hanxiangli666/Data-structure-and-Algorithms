思路完全对！有几个问题，我们来看：

---

# 问题一：代码不能写在函数外面

C++所有执行的代码 **必须在函数里** ，你把 `getline`、`for` 循环都写在了 `main` 外面，这是不行的。

---

# 问题二：`count` 定义在循环里面

```cpp
for (char c : line) {
    int count = 0;   // ❌ 每次循环都重置为0了！
```

`count` 应该在循环**外面**定义，循环里只做 `count++`。

变量的**作用域**规则：定义在 `{}` 里的变量，出了这个 `{}` 就不存在了。

```cpp
int count = 0;        // 在循环外定义
for (char c : line) {
    if (isupper(c)) {
        count++;      // 只做累加
    }
}
cout << count << endl; // 出了循环还能访问
```

---

# 问题三：花括号没有正确配对

你的 `}}` 把 `for` 和 `if` 都关掉了，但 `main` 没有正确开始和结束。

---

# 修正后的完整代码

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string line;
    getline(cin, line);   // 读整行

    int count = 0;        // 在循环外定义！
    for (char c : line) {
        if (isupper(c)) {
            count++;
        }
    }

    cout << count << endl;
    return 0;
}
```

---

# 这次你犯的错误规律

| 错误              | 根本原因                       |
| ----------------- | ------------------------------ |
| 代码写在main外面  | 没养成"先写框架再填内容"的习惯 |
| count定义在循环里 | 没理解作用域                   |
| 花括号混乱        | 没有及时配对                   |

**建议你以后写代码的顺序：**

```cpp
// 第一步：先把框架写完整
#include <iostream>
using namespace std;

int main() {

    return 0;
}

// 第二步：再往里填逻辑
```

先搭骨架，再填肉，花括号永远成对出现。

---

理解了吗？理解了我们继续下一个知识点： **排序和常用STL算法函数** ，竞赛中超高频！
