# 第五课：结构体 struct

## 为什么需要struct？

假设你要存10个学生的信息，每个学生有姓名、年龄、成绩。

如果不用struct：

```cpp
vector<string> names;
vector<int> ages;
vector<double> scores;
// 三个数组要同步管理，很麻烦
```

这样写的问题是：三个数组之间没有任何关联，你要保证 `names[i]`、`ages[i]`、`scores[i]` 始终对应同一个学生。一旦对其中一个数组排序，其他两个不会跟着动，数据就乱了。

用struct把相关数据 **打包在一起** ：

```cpp
struct Student {
    string name;
    int age;
    double score;
};

vector<Student> students;  // 一个数组搞定
```

现在每个学生的所有信息绑定在同一个对象里，排序时三个字段一起移动，永远不会乱。*就像用三个独立列表和用一张表格的区别——三个列表你要自己维护行号对应关系，表格天然就是按行绑定的。*

> ✅ struct 是竞赛里组织多字段数据的标准工具，掌握它，解图论题、排序题都会顺手很多！

---

## 基本写法

```cpp
struct Student {
    string name;
    int age;
    double score;
};  // 注意这里有分号！

int main() {
    // 创建一个Student
    Student s;
    s.name = "Alice";
    s.age = 20;
    s.score = 95.5;

    // 或者直接初始化
    Student s2 = {"Bob", 21, 88.0};

    // 访问成员用 .
    cout << s.name << " " << s.age << endl;  // Alice 20

    return 0;
}
```

struct 定义结尾的分号是 C++ 语法规定的，不能省。这是初学者最容易漏写的地方。原因是 struct 的定义语法允许在 `}` 后面直接声明变量（比如 `} alice, bob;`），所以需要用分号来明确"这里结束了"。

> 💡 **老师提醒：** struct 定义必须放在 `main` **外面**（或者任何用到它的函数外面），因为它是类型定义，不是执行语句。把 struct 写在 main 里面在技术上也能编译，但是一旦多个函数都要用这个类型，就没法共享，所以统一写在文件顶部是标准做法。

---

## 结合vector使用

```cpp
struct Student {
    string name;
    int age;
    double score;
};

int main() {
    vector<Student> students;

    // 添加学生
    students.push_back({"Alice", 20, 95.5});
    students.push_back({"Bob", 21, 88.0});
    students.push_back({"Charlie", 19, 92.0});

    // 遍历
    for (auto &s : students) {
        cout << s.name << " " << s.score << endl;
    }

    return 0;
}
```

`{"Alice", 20, 95.5}` 是聚合初始化，按照 struct 字段定义的顺序填值。遍历时记得加 `&`，避免每次循环复制整个 Student 对象。

---

## 对struct排序（竞赛最常用！）

```cpp
struct Student {
    string name;
    int age;
    double score;
};

int main() {
    vector<Student> students = {
        {"Alice", 20, 95.5},
        {"Bob", 21, 88.0},
        {"Charlie", 19, 92.0}
    };

    // 按成绩从高到低排序
    sort(students.begin(), students.end(), [](Student &a, Student &b) {
        return a.score > b.score;  // > 是从大到小
    });

    for (auto &s : students) {
        cout << s.name << " " << s.score << endl;
    }
    // 输出：
    // Alice 95.5
    // Charlie 92.0
    // Bob 88.0

    return 0;
}
```

lambda 里的 `return a.score > b.score` 意思是：如果 a 的成绩比 b 大，就让 a 排在前面——这就是降序。把 `>` 换成 `<` 就是升序。你可以把这个 lambda 读成一句话："a 应该排在 b 前面，当且仅当 a.score > b.score"。

> 💡 **老师提醒：** lambda 里访问 struct 的字段，用的是参数名而不是 struct 名。`a.score` 是"第一个参数 a 的 score 字段"，不要写成 `Student.score`——那是语法错误。

---

## 竞赛中struct的典型场景

**场景一：存坐标点**

```cpp
struct Point {
    int x, y;
};

Point p = {3, 4};
cout << p.x << " " << p.y << endl;
```

坐标点是竞赛里最简单的 struct 用法，几何题、BFS/DFS 里存位置都会用到。

**场景二：存边（图论题超高频）**

```cpp
struct Edge {
    int from, to, weight;
};

vector<Edge> edges;
edges.push_back({1, 2, 5});  // 从节点1到节点2，权重5
```

图论题用 struct 存边是标准写法，Kruskal 最小生成树、Dijkstra 等算法都会这样组织数据。

**场景三：多关键字排序**

```cpp
struct Student {
    string name;
    int age;
    double score;
};

// 先按成绩从高到低，成绩相同按名字字典序
sort(students.begin(), students.end(), [](Student &a, Student &b) {
    if (a.score != b.score) return a.score > b.score;
    return a.name < b.name;
});
```

多关键字排序是**一个** sort，lambda 里用 `if` 依次判断每个关键字。逻辑是：先看主关键字是否能分出胜负，能的话直接返回；不能（相等）时才看次关键字。*就像多关键字排序，就像图书馆给书排架——先按类目，类目相同再按出版年份，年份相同再按书名，每一层只在上一层无法区分时才启用。*

> 💡 **老师提醒：** 多关键字排序时，`if (a.score != b.score)` 这个相等判断非常重要，不能省。如果直接写两个 `return`，第一个 `return` 执行后函数就结束了，第二个永远不会被执行。

> ✅ struct + sort + lambda 三件套是竞赛里处理"多字段排序"的完整解决方案，这个组合出现频率极高，熟练了之后很多题直接就有思路了！

---

## 现在你来练习

> **题目：** 输入n个学生的姓名和成绩，按成绩从高到低输出，成绩相同则按姓名字典序从小到大输出。
>
> 输入：
>
> ```
> 4
> Alice 90
> Bob 85
> Charlie 90
> David 78
> ```
>
> 输出：
>
> ```
> Alice 90
> Charlie 90
> Bob 85
> David 78
> ```

这次要用到：struct、vector、sort、lambda。

写完发给我！
