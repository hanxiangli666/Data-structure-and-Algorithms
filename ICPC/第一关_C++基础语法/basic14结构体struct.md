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

用struct把相关数据 **打包在一起** ：

```cpp
struct Student {
    string name;
    int age;
    double score;
};

vector<Student> students;  // 一个数组搞定
```

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

**场景二：存边（图论题超高频）**

```cpp
struct Edge {
    int from, to, weight;
};

vector<Edge> edges;
edges.push_back({1, 2, 5});  // 从节点1到节点2，权重5
```

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
