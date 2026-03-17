思路完全正确，快速幂函数写得很好！就两个问题：

---

**问题一：`cin >> a >> b` 又跑到main外面了**

这个问题出现四五次了，以后我不会再提醒你了，你自己要养成习惯。

**问题二：输出没有用读入的a和b**

```cpp
cout << power(2, 11, mod) << endl;  // ❌ 把输入写死了
cout << power(a, b, mod) << endl;   // ✅ 用读进来的变量
```

---

# 修正后完整代码

```cpp
#include <iostream>
using namespace std;

const long long MOD = 1e9 + 7;

long long power(long long a, long long b, long long mod) {
    long long result = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) {
            result = result * a % mod;
        }
        a = a * a % mod;
        b >>= 1;
    }
    return result;
}

int main() {
    long long a, b;
    cin >> a >> b;
    cout << power(a, b, MOD) << endl;
    return 0;
}
```

---

# 你目前学完的内容

```
✅ 第一课：输入输出
✅ 第二课：函数
✅ 第三课：数组和字符串
✅ 第四课：STL算法函数
✅ 第五课：结构体struct
✅ 第六课：位运算
✅ 第七课：常用数学（取模、快速幂）
```

C++基础部分基本覆盖完了，剩下一个重要的知识点：

**第八课：竞赛输入输出优化**

这个很短，但竞赛中非常实用，学完之后我们就可以开始刷真正的算法题了。继续吗？
