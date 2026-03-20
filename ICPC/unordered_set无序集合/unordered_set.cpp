#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_set<int> s;

    // insert：插入元素
    s.insert(30);
    s.insert(10);
    s.insert(50);
    s.insert(10);  // 重复插入，自动忽略

    // size：元素个数（重复的只算一个）
    cout << s.size() << "\n";   // 3

    // empty：是否为空
    cout << s.empty() << "\n";  // 0（false）

    // count：判断元素是否存在（返回 0 或 1）
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

    // 遍历：顺序随机
    for (int x : s) {
        cout << x << " ";
    }
    cout << "\n";

    // clear：清空
    s.clear();
    if (s.empty()) {
        cout << "unordered_set 已空\n";
    }

    return 0;
}