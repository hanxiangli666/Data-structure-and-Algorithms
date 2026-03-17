思路对了大半！但有几个结构性的问题，我们一起来看。

---

# 主要问题

**问题一：struct缺分号，代码又跑到main外面了**

```cpp
struct Student {
    string name;
    double score;
};  // ← 忘了分号

// 下面这些必须在main里面！
cin >> n;
vector<Student> students[n];
```

**问题二：vector写法错了**

```cpp
vector<Student> students[n];  // ❌ 这是数组的写法
vector<Student> students(n);  // ✅ vector用小括号
```

**问题三：读入方式不对**

```cpp
cin >> s[i];  // ❌ cin不知道怎么读一个Student
// 要分开读name和score
cin >> students[i].name >> students[i].score;  // ✅
```

**问题四：sort的lambda不需要if/else**

多关键字排序不是两个sort，是 **一个sort里面写逻辑** ：

```cpp
sort(students.begin(), students.end(), [](Student &a, Student &b) {
    if (a.score != b.score) return a.score > b.score;
    return a.name < b.name;  // 成绩相同才走这里
});
```

---

# 完整正确代码

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Student {
    string name;
    double score;
};  // 分号别忘

int main() {
    int n;
    cin >> n;

    vector<Student> students(n);
    for (int i = 0; i < n; i++) {
        cin >> students[i].name >> students[i].score;
    }

    sort(students.begin(), students.end(), [](Student &a, Student &b) {
        if (a.score != b.score) return a.score > b.score;
        return a.name < b.name;
    });

    for (auto &s : students) {
        cout << s.name << " " << s.score << endl;
    }

    return 0;
}
```

---

# 你这次的进步

| 项目         | 情况                         |
| ------------ | ---------------------------- |
| struct定义   | ✅ 结构对了                  |
| vector使用   | 🔺 小括号写成中括号          |
| sort+lambda  | ✅ 知道该用lambda            |
| 多关键字逻辑 | ✅ if/else思路对，位置放错了 |
| 框架问题     | ❌ 还是把代码写到main外面    |

框架问题还是出现了，**再强调一次，每次写题第一步：**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    // 所有代码写在这里
    return 0;
}
```

---

整体来说进步很明显，struct和lambda的思路都有了。

下一个知识点： **位运算** ，准备好了继续！
