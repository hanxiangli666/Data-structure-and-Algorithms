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

struct 结尾分号漏掉，编译器会把后面的代码误读为 struct 定义的一部分，产生非常难懂的报错信息。这个分号是 struct 的标志性语法，每次写完 `}` 立刻补上 `;`，形成条件反射。

---

**问题二：vector写法错了**

```cpp
vector<Student> students[n];  // ❌ 这是数组的写法
vector<Student> students(n);  // ✅ vector用小括号
```

`[]` 是 C 风格数组的声明语法，`()` 才是 vector 构造函数的调用语法。两者长得很像，但含义完全不同。*就像 `vector<T>[n]` 和 `vector<T>(n)` 的区别，就像"买 n 瓶水"和"买一个能装 n 瓶水的桶"的区别——前者是数组，后者才是你想要的动态容器。*

> 💡 **老师提醒：** `vector<Student> students[n]` 创建的是一个有 `n` 个 vector 的数组（每个元素本身是一个空 vector），而不是一个有 `n` 个 Student 的 vector。这种写法不但错了，还合法——编译器不报错，但运行结果完全不是你想的。记住：vector 用圆括号初始化容量。

---

**问题三：读入方式不对**

```cpp
cin >> s[i];  // ❌ cin不知道怎么读一个Student
// 要分开读name和score
cin >> students[i].name >> students[i].score;  // ✅
```

`cin` 只认识基本类型（int、double、string 等），不知道怎么把一行输入"拆进"一个 struct 的各个字段。你需要把字段一个一个地读，用 `.` 访问每个成员。

---

**问题四：sort的lambda不需要if/else**

多关键字排序不是两个sort，是 **一个sort里面写逻辑** ：

```cpp
sort(students.begin(), students.end(), [](Student &a, Student &b) {
    if (a.score != b.score) return a.score > b.score;
    return a.name < b.name;  // 成绩相同才走这里
});
```

两个 `sort` 调用会互相覆盖——第二个 sort 执行完，第一个 sort 的结果就被打乱了。多关键字排序的正确做法是：在**一个** lambda 里，用 `if` 按优先级逐层判断，上一层相等时才进入下一层。

> 💡 **老师提醒：** `if (a.score != b.score) return a.score > b.score;` 这一行非常关键——当成绩相等时，它不返回，继续往下执行 `return a.name < b.name`。如果把条件改成 `if (a.score > b.score)` 漏掉了"相等时不返回"的情况，当 a.score == b.score 时程序会跳过次关键字判断，直接返回 false，导致同分学生的名字顺序是随机的。

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

> ✅ 能独立想到用 struct 存多字段、用 lambda 做自定义排序，说明你对这套工具已经有感觉了，接下来就是把细节打磨得更扎实！

下一个知识点： **位运算** ，准备好了继续！
