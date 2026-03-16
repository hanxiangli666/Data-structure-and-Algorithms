# 你最常出现的错误

## 错误一：代码写在main外面（出现5次）

```cpp
// ❌ 你经常这样写
int n;
cin >> n;        // 执行语句跑到外面了
vector<int> a(n);

int main() { ... }

// ✅ 正确
int main() {
    int n;
    cin >> n;
    vector<int> a(n);
}
```

**根本原因：** Python没有main函数的概念，习惯带过来了。

---

## 错误二：变量没有声明类型（出现3次）

```cpp
// ❌ Python习惯
int maxOfThree(a, b, c) { ... }
cin >> n;

// ✅ C++必须声明类型
int maxOfThree(int a, int b, int c) { ... }
int n;
cin >> n;
```

**根本原因：** Python是动态类型，C++是静态类型，每个变量必须提前声明类型。

---

## 错误三：花括号没有配对（出现3次）

```cpp
// ❌ 你写的
for (char c : line) {
if (isupper(c)){
count++;}}   // 乱配对

// ✅ 正确，每个{立刻找到对应的}
for (char c : line) {
    if (isupper(c)) {
        count++;
    }
}
```

**根本原因：** 没有养成"写{立刻写}"的习惯。

---

## 错误四：计数器定义在循环里（出现1次）

```cpp
// ❌ 每次循环都重置为0
for (char c : line) {
    int count = 0;
    if (isupper(c)) count++;
}

// ✅ 定义在循环外
int count = 0;
for (char c : line) {
    if (isupper(c)) count++;
}
```

**根本原因：** 没理解变量作用域，定义在{}里的变量出了{}就消失。

---

## 错误五：去重后还用旧的n遍历（出现1次）

```cpp
// ❌ 去重后size变了，n还是原来的值
a.erase(unique(a.begin(), a.end()), a.end());
for (int i = 0; i < n; i++) { ... }

// ✅ 用实际大小
for (int x : a) { ... }
```

**根本原因：** 没意识到erase会改变vector的大小。

---

## 错误六：赋值和计算混淆（出现1次）

```cpp
// ❌ 只计算，没有存回去
n >> 1;

// ✅ 要存回去
n >>= 1;   // 或者 n = n >> 1
```

**根本原因：** 和Python的 `n // 2` 不一样，C++不会自动存回去。

---

# 一句话总结

```
你90%的错误来自两个习惯没改过来：
1. Python不需要main，C++所有执行代码必须在main里
2. Python不需要声明类型，C++每个变量必须声明类型
```

开始二分查找吗？
