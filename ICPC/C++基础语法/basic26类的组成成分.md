好，从头捋清楚。

---

# 先回答"这些是什么的组成成分"

它们都是 **class（类）** 的组成部分。

```cpp
class MyArrayList {
//  ↑ 一个类，里面包含：

    E *data;        // 成员变量
    int size;       // 成员变量
    int cap;        // 成员变量

    MyArrayList() {}   // 构造函数
    ~MyArrayList() {}  // 析构函数
    void addLast() {}  // 成员函数
    void resize() {}   // 成员函数
};
```

---

# 逐个解释

## 成员变量：这个对象"有什么"

```cpp
class Student {
    string name;   // 成员变量：学生有名字
    int age;       // 成员变量：学生有年龄
    double score;  // 成员变量：学生有分数
};
```

就是这个对象身上携带的数据。

---

## 成员函数：这个对象"能做什么"

```cpp
class Student {
    string name;
    int age;

    void introduce() {           // 成员函数：学生能自我介绍
        cout << name << endl;    // 成员函数里可以直接用成员变量
    }

    void birthday() {            // 成员函数：学生能过生日
        age++;
    }
};
```

---

## 构造函数：对象"出生时"自动执行

```cpp
class Student {
    string name;
    int age;

    Student(string n, int a) {  // 构造函数：名字和类名一样，没有返回类型
        name = n;
        age = a;
    }
};

int main() {
    Student s("Alice", 20);  // 创建对象时，构造函数自动执行
}
```

**什么时候触发：** 创建对象的那一刻，自动执行，不需要手动调用。

---

## 析构函数：对象"死亡时"自动执行

```cpp
class Student {
    int *data;

    Student() {
        data = new int[100];  // 构造时申请内存
    }

    ~Student() {              // 析构函数：名字是~加类名
        delete[] data;        // 析构时释放内存
    }
};

int main() {
    Student s;
    // ...
}   // ← s在这里消亡，~Student()自动执行
```

**什么时候触发：** 对象消亡时（函数结束、程序退出），自动执行。

**主要用途：** 释放构造函数里申请的内存，防止内存泄漏。

---

# 用一个完整例子串起来

```cpp
class Student {
private:
    string name;    // 成员变量
    int age;        // 成员变量

public:
    // 构造函数：创建对象时执行
    Student(string n, int a) {
        name = n;
        age = a;
        cout << name << "出生了" << endl;
    }

    // 成员函数：对象能做的事
    void introduce() {
        cout << "我是" << name << "，" << age << "岁" << endl;
    }

    // 析构函数：对象消亡时执行
    ~Student() {
        cout << name << "消亡了" << endl;
    }
};

int main() {
    Student s("Alice", 20);  // 构造函数执行：输出"Alice出生了"
    s.introduce();            // 成员函数：输出"我是Alice，20岁"
}   // 析构函数执行：输出"Alice消亡了"
```

输出：

```
Alice出生了
我是Alice，20岁
Alice消亡了
```

---

# 什么时候用这些东西？

| 东西     | 用途           | 什么时候需要      |
| -------- | -------------- | ----------------- |
| 成员变量 | 存对象的数据   | 每次定义类都有    |
| 成员函数 | 定义对象的行为 | 每次定义类都有    |
| 构造函数 | 初始化成员变量 | 每次定义类都有    |
| 析构函数 | 释放申请的内存 | 只有用了new才需要 |

---

# 竞赛中的实际情况

说实话，竞赛中 **很少自己写class** 。

```
竞赛中实际用到这些知识的场景：

✅ struct：存坐标、边、学生信息等（高频）
✅ 成员变量：struct里的各个字段（高频）
⚠️ class：偶尔手写数据结构时用
⚠️ 构造函数：手写数据结构时用
❌ 析构函数：竞赛中基本不写，内存泄漏无所谓
```

竞赛不是工程项目，程序跑完就结束了，内存泄漏不影响结果，所以析构函数基本可以忽略。

**你现在最需要掌握的是 `struct`，class这些看懂别人代码就行。**

清楚了吗？可以开始刷题了！
