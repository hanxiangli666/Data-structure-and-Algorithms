#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    set<int> s;

    // insert：插入元素
    s.insert(30);
    s.insert(10);
    s.insert(50);
    s.insert(10);  // 重复插入，自动忽略

    // size：元素个数
    cout << s.size() << "\n";   // 3

    // empty：是否为空
    cout << s.empty() << "\n";  // 0（false）

    // count：判断元素是否存在
    if (s.count(10)) {
        cout << "10 存在\n";
    }
    if (!s.count(99)) {
        cout << "99 不存在\n";
    }

    // erase：删除元素
    s.erase(10);
    cout << s.size() << "\n";   // 2

    // 确认删除
    if (!s.count(10)) {
        cout << "10 已删除\n";
    }

    // 遍历：自动按升序输出（这是和 unordered_set 最大的区别）
    for (int x : s) {
        cout << x << " ";
    }
    cout << "\n";  // 30 50（升序）

    // set 独有：lower_bound 和 upper_bound
    s.insert(20);
    s.insert(40);
    // 现在 set：{20, 30, 40, 50}

    // lower_bound：第一个 >= 目标值 的迭代器
    auto it1 = s.lower_bound(30);
    cout << *it1 << "\n";  // 30

    // upper_bound：第一个 > 目标值 的迭代器
    auto it2 = s.upper_bound(30);
    cout << *it2 << "\n";  // 40

    // clear：清空
    s.clear();
    if (s.empty()) {
        cout << "set 已空\n";
    }

    return 0;
}