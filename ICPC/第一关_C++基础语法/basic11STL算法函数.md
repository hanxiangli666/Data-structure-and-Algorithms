# 第四课：排序和常用STL算法函数

需要引入头文件：

```cpp
#include <algorithm>
```

这个头文件包含了 C++ 标准库里绝大多数算法函数。"algorithm"就是"算法"的意思——凡是不知道某个操作在哪个头文件里，优先试试它。

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

`v.begin()` 和 `v.end()` 是 **迭代器** ，你暂时可以理解为"数组的起点和终点"，sort需要知道排哪个范围。迭代器是 C++ STL 的核心概念，它的作用是让算法和容器解耦——`sort` 不需要知道你用的是 `vector` 还是别的容器，只要你给它两个迭代器，它就知道该排哪段数据。*就像迭代器和算法的关系，就像遥控器和电视的关系——遥控器不需要知道电视内部的电路，只要接口统一，任何电视都能用同一个遥控器操作。*

> 💡 **老师提醒：** `sort` 的时间复杂度是 O(n log n)，是竞赛里最常用的排序。不要自己手写冒泡或选择排序，那是 O(n²)，数据量稍大就会超时。直接用 `sort`，又快又省事。

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

`[](...){...}` 叫做 **lambda函数** ，可以理解为临时定义的比较规则：返回true表示a排在b前面。记住这个约定：lambda 里写 `return a < b` 就是升序（小的在前）；写 `return a > b` 就是降序（大的在前）。自定义字段排序时，把 `a`、`b` 换成对应字段就行，后面 struct 排序会大量用到。

> 💡 **老师提醒：** lambda 的比较函数必须满足"严格弱序"——也就是说不能出现 `a < a` 为 true 的情况，否则 sort 会崩溃。最常见的错误是写 `return a.score >= b.score`（用了 `>=` 而不是 `>`），这会导致相等时两个互相"比对方小"，引发未定义行为。记住：相等时必须返回 `false`。

> ✅ sort + lambda 这个组合是竞赛里最万能的排序写法，掌握了它，几乎所有排序需求都能解决！

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

注意 `max_element` 前面有个 `*`，因为它返回的是迭代器（地址），要用 `*` 取值。这和之前讲的指针解引用是同一个道理——迭代器本质上就是一种"指向容器内某个元素的指针"，用 `*` 才能拿到它指向的那个值。

> 💡 **老师提醒：** `max` 和 `max_element` 是两个不同的函数：`max(a, b)` 比较两个**值**，`max_element(begin, end)` 找整个范围里的**最大值位置**。漏写 `*` 是初学者最常犯的错，编译器不会报错，但你会得到一个莫名其妙的大数字（迭代器的地址）。

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

`reverse` 直接在原容器上操作，不返回新容器。字符串反转是竞赛里的高频操作，`reverse(s.begin(), s.end())` 一行搞定，不需要自己写交换逻辑。

> ✅ reverse 和 sort 都能直接用在 string 上，凡是有 begin/end 的容器，这些算法就通用——这就是迭代器的价值！

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

`find` 返回的是指向找到元素的迭代器；如果没找到，返回 `v.end()`。所以判断"找没找到"就是判断 `it != v.end()`。`(it - v.begin())` 用来算出下标，两个迭代器相减得到距离，这是把迭代器位置换算成索引的标准写法。

> 💡 **老师提醒：** `find` 是线性查找，时间复杂度 O(n)。如果数组已经排好序，用后面的 `binary_search` 或 `lower_bound` 更快（O(log n)）。数据量大时，这个区别会决定能不能通过。

---

## 五、accumulate 求和

```cpp
#include <numeric>   // 注意这个头文件不一样！

vector<int> v = {1, 2, 3, 4, 5};
int sum = accumulate(v.begin(), v.end(), 0);  // 0是初始值
// sum = 15
```

第三个参数是初始值，结果 = 初始值 + 容器里所有元素之和。大多数情况传 `0` 就对了；如果你要求乘积，可以传 `1`，然后传入自定义操作（进阶用法）。`accumulate` 在 `<numeric>` 头文件里，不是 `<algorithm>`——这是最容易忘的地方。

> 💡 **老师提醒：** `accumulate` 的初始值类型决定了结果类型。如果你的 vector 里是 `int`，但求和结果可能溢出，记得把初始值写成 `0LL`（long long 类型的 0），否则求和过程全程用 `int` 算，溢出了你根本不知道。

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

`lower_bound` 和 `upper_bound` 的区别记住这一点：`lower_bound` 找的是"第一个不比目标小的位置"，`upper_bound` 找的是"第一个比目标大的位置"。*就像 lower_bound 和 upper_bound 的区别，就像找人站队时"从第一个不低于 170cm 的人开始"和"从第一个高于 170cm 的人开始"的区别——前者包含 170cm，后者不包含。*

> 💡 **老师提醒：** 二分查找的前提是**有序**，这一点必须保证。对无序数组调用 `binary_search` 不会报错，但结果是错的。使用前先检查：是否已经 `sort` 过？

这两个函数在竞赛中 **非常高频** ，后面讲二分算法时还会深入讲。

> ✅ lower_bound 和 upper_bound 掌握了，以后遇到"找某个范围内有多少个值"的题，`upper_bound - lower_bound` 直接出答案，竞赛里超实用！

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

`unique` 把重复元素移到末尾，`erase` 把末尾那部分删掉，两步配合使用。理解这个原理：`unique` 只是把不重复的元素"紧缩"到前面，并返回一个指向"垃圾区域"开头的迭代器；真正删除内存还需要 `erase`。*就像 unique 和 erase 的配合，就像把一个书架上重复的书都推到右边，然后再把右边那段直接锯掉——两步缺一不可。*

> 💡 **老师提醒：** `unique` 必须在**排序之后**使用，因为它只能合并**相邻**的重复元素。如果你的数组是 `{1, 2, 1}`，`unique` 后结果是 `{1, 2, 1}`——第一个和第三个 `1` 不相邻，它不会处理。先 `sort` 再 `unique` 才能正确去重。

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
