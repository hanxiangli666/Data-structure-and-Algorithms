思路完全正确！就三个问题：

---

**问题一：缺少头文件和主函数框架**

你又忘了写框架了，记住先搭骨架再填内容。这个问题已经出现不止一次了，说明"先框架"还没形成肌肉记忆——每次写题，第一件事不是想逻辑，是打出 `#include` 和 `main`。

---

**问题二：`n` 没有声明类型**

```cpp
cin >> n;        // ❌ n 是什么类型？
int n;           // 要先声明
cin >> n;        // ✅
```

C++ 里变量必须先声明再使用，编译器不会自动推断你想要的类型。这和 Python 不同——Python 赋值时自动创建变量，C++ 要你明确告诉编译器"这是一个 int"，它才会分配对应大小的内存。

---

**问题三：去重后不能用原来的 `n` 遍历**

```cpp
a.erase(unique(a.begin(), a.end()), a.end());
// 去重后 a 的大小已经变了，不再是 n 了！

for (int i = 0; i < n; i++)      // ❌ 会多打印
for (int i = 0; i < a.size(); i++) // ✅ 用实际大小
// 或者用范围for循环更简洁
for (int x : a)                   // ✅
```

`erase` 真正改变了 vector 的大小，但 `n` 这个变量存的是原始数量，它不会自动更新。这是一个很典型的"状态不同步"问题：你改了容器，但忘了相关变量也要跟着变。用范围 for 循环能从根本上避免这个问题，因为它不依赖任何外部计数变量。

> 💡 **老师提醒：** 任何时候你对容器做了增删操作（`push_back`、`erase`、`unique+erase` 等），原来记录大小的变量 `n` 就失效了。以后凡是遍历"可能被修改过的容器"，优先用范围 for 或 `a.size()`，不要用手动记录的 `n`。

---

# 修正后完整代码

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()), a.end());

    for (int x : a) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
```

注意输出我改成了空格分隔，因为题目要求是 `1 2 3` 而不是每个数占一行。

---

# 你的进步

对比第一次练习，这次：

* ✅ 变量定义在main里了
* ✅ 作用域没问题
* ✅ 算法函数用对了
* ❌ 还是忘了写框架

**给你一个固定模板，每次写题先把这个打出来：**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {

    return 0;
}
```

然后再往里填逻辑，养成习惯就不会忘了。

> ✅ 算法函数能用对、变量作用域没问题，说明知识点本身已经掌握了，剩下的框架习惯只是练习次数的问题，多写几道题就自然了！

---

下一个知识点： **结构体 struct** ，准备好了继续！
