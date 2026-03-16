思路完全正确！就三个问题：

---

**问题一：缺少头文件和主函数框架**

你又忘了写框架了，记住先搭骨架再填内容。

---

**问题二：`n` 没有声明类型**

```cpp
cin >> n;        // ❌ n 是什么类型？
int n;           // 要先声明
cin >> n;        // ✅
```

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

---

下一个知识点： **结构体 struct** ，准备好了继续！
