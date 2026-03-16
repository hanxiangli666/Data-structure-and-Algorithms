# 第四课：排序和常用STL算法函数

需要引入头文件：

```cpp
#include <algorithm>
```

---

## 一、排序 sort

```cpp
vector<int> v = {5, 3, 1, 4, 2};

// 从小到大（默认）
sort(v.begin(), v.end());
// v = {1, 2, 3, 4, 5}

// 从大到小
sort(v.begin(), v.end(), greater<int>());
// v = {5, 4, 3, 2, 1}
```

`v.begin()` 和 `v.end()` 是 **迭代器** ，你暂时可以理解为"数组的起点和终点"，sort需要知道排哪个范围。

**字符串也可以排序：**

```cpp
string s = "dcba";
sort(s.begin(), s.end());
// s = "abcd"
```

**自定义排序规则：**

```cpp
// 按绝对值从小到大排
vector<int> v = {-3, 1, -2, 4};
sort(v.begin(), v.end(), [](int a, int b) {
    return abs(a) < abs(b);
});
// v = {1, -2, -3, 4}
```

`[](...){...}` 叫做 **lambda函数** ，可以理解为临时定义的比较规则：返回true表示a排在b前面。

---

## 二、最大值和最小值

```cpp
// 两个数比较
int a = 3, b = 7;
cout << max(a, b) << endl;   // 7
cout << min(a, b) << endl;   // 3

// 数组/vector中的最大最小值
vector<int> v = {3, 1, 4, 1, 5, 9};
int maxVal = *max_element(v.begin(), v.end());  // 9
int minVal = *min_element(v.begin(), v.end());  // 1
```

注意 `max_element` 前面有个 `*`，因为它返回的是迭代器（地址），要用 `*` 取值。

---

## 三、reverse 反转

```cpp
vector<int> v = {1, 2, 3, 4, 5};
reverse(v.begin(), v.end());
// v = {5, 4, 3, 2, 1}

string s = "hello";
reverse(s.begin(), s.end());
// s = "olleh"
```

---

## 四、find 查找

```cpp
vector<int> v = {1, 2, 3, 4, 5};

auto it = find(v.begin(), v.end(), 3);  // 查找值为3的元素

if (it != v.end()) {
    cout << "找到了" << endl;
    cout << "位置：" << (it - v.begin()) << endl;  // 2
} else {
    cout << "没找到" << endl;
}
```

---

## 五、accumulate 求和

```cpp
#include <numeric>   // 注意这个头文件不一样！

vector<int> v = {1, 2, 3, 4, 5};
int sum = accumulate(v.begin(), v.end(), 0);  // 0是初始值
// sum = 15
```

---

## 六、二分查找 binary_search / lower_bound / upper_bound

**前提：数组必须已经排好序！**

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 判断某个值存不存在
bool found = binary_search(v.begin(), v.end(), 3);  // true

// lower_bound：第一个 >= 目标值 的位置
auto it = lower_bound(v.begin(), v.end(), 3);
cout << (it - v.begin()) << endl;   // 2（索引2，值为3）

// upper_bound：第一个 > 目标值 的位置
auto it2 = upper_bound(v.begin(), v.end(), 3);
cout << (it2 - v.begin()) << endl;  // 3（索引3，值为4）
```

图示：

```
数组：  1   2   3   4   5
索引：  0   1   2   3   4
                ↑       ↑
          lower_bound(3) upper_bound(3)
```

这两个函数在竞赛中 **非常高频** ，后面讲二分算法时还会深入讲。

---

## 七、unique 去重

```cpp
// 先排序，再去重
vector<int> v = {3, 1, 2, 1, 3, 2};
sort(v.begin(), v.end());
// v = {1, 1, 2, 2, 3, 3}

v.erase(unique(v.begin(), v.end()), v.end());
// v = {1, 2, 3}
```

`unique` 把重复元素移到末尾，`erase` 把末尾那部分删掉，两步配合使用。

---

## 常用函数速查表

| 函数                          | 作用                 | 头文件    |
| ----------------------------- | -------------------- | --------- |
| `sort`                      | 排序                 | algorithm |
| `reverse`                   | 反转                 | algorithm |
| `max / min`                 | 两数比较             | algorithm |
| `max_element / min_element` | 数组最大最小值       | algorithm |
| `find`                      | 查找元素             | algorithm |
| `binary_search`             | 二分查找（存不存在） | algorithm |
| `lower_bound`               | 二分查找（第一个>=） | algorithm |
| `upper_bound`               | 二分查找（第一个>）  | algorithm |
| `accumulate`                | 求和                 | numeric   |
| `unique`                    | 去重                 | algorithm |

---

## 现在你来练习

> **题目：** 输入n个整数，输出排序后去重的结果。
>
> 输入：
>
> ```
> 6
> 3 1 2 1 3 2
> ```
>
> 输出：`1 2 3`

写完发给我！
