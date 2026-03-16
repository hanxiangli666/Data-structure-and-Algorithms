# break 和 continue

两个都是用来**控制循环流程**的，但效果不同。

---

## break：直接跳出整个循环

```cpp
for (int i = 0; i < 5; i++) {
    if (i == 3) break;  // i等于3时，直接结束循环
    cout << i << " ";
}
// 输出：0 1 2
```

图示：

```
i=0 → 打印0
i=1 → 打印1
i=2 → 打印2
i=3 → 遇到break，整个循环结束
i=4 → 永远不会执行
```

---

## continue：跳过本次，继续下一次

```cpp
for (int i = 0; i < 5; i++) {
    if (i == 3) continue;  // i等于3时，跳过这次，继续下一次
    cout << i << " ";
}
// 输出：0 1 2 4
```

图示：

```
i=0 → 打印0
i=1 → 打印1
i=2 → 打印2
i=3 → 遇到continue，跳过cout，直接去i=4
i=4 → 打印4
```

---

## 一句话区别

```
break    = 我不玩了，整个循环结束
continue = 这把不算，直接下一把
```

---

## while循环里也一样

```cpp
int i = 0;
while (i < 10) {
    i++;
    if (i % 2 == 0) continue;  // 偶数跳过
    if (i > 7) break;           // 大于7结束
    cout << i << " ";
}
// 输出：1 3 5 7
```

---

## 嵌套循环里的break/continue

⚠️ 注意： **break和continue只影响最近的那一层循环** 。

```cpp
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (j == 1) break;  // 只跳出内层循环，外层继续
        cout << i << " " << j << endl;
    }
}
// 输出：
// 0 0
// 1 0
// 2 0
```

如果想直接跳出外层循环，竞赛中常用一个flag：

```cpp
bool found = false;
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (i == 1 && j == 1) {
            found = true;
            break;  // 跳出内层
        }
    }
    if (found) break;  // 跳出外层
}
```

---

## 竞赛中最常见的用法

```cpp
// 1. 找到目标就停止
for (int i = 0; i < n; i++) {
    if (a[i] == target) {
        cout << i << endl;
        break;  // 找到了就不用继续找
    }
}

// 2. 跳过不需要处理的情况
for (int i = 0; i < n; i++) {
    if (a[i] < 0) continue;  // 负数跳过
    // 处理非负数
    cout << a[i] << endl;
}
```

---

就这些，很简单。现在开始二分查找吗？
