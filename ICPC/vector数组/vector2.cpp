#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 初始化 vector，包含 {1, 2, 3}
    vector<int> v = {1, 2, 3};

    // push_back：尾部添加 4 → {1, 2, 3, 4}
    v.push_back(4);

    // pop_back：删除尾部 → {1, 2, 3}
    v.pop_back();

    // front：看第一个元素，不删除
    cout << v.front() << "\n";  // 1

    // back：看最后一个元素，不删除
    cout << v.back() << "\n";   // 3

    // size：元素个数
    cout << v.size() << "\n";   // 3

    // empty：是否为空（返回 0 或 1）
    cout << v.empty() << "\n";  // 0（false，不为空）

    // !empty：取反，判断"不为空"更直观
    cout << !v.empty() << "\n"; // 1（true，确实不为空）

    // resize：调整大小到 10，多出来的位置自动填 0
    // {1, 2, 3, 0, 0, 0, 0, 0, 0, 0}
    v.resize(10);

    // 确认 resize 后 size 变了
    cout << v.size() << "\n";   // 10

    // v[1]：访问索引 1 的元素
    cout << v[1] << "\n";       // 2

    // clear：清空所有元素，size 变为 0
    v.clear();

    // 访问前先判断是否为空，防止越界
    if (!v.empty()) {
        cout << v[1] << "\n";       // 安全
    } else {
        cout << "vector 已空\n";    // 走这里
    }

    return 0;
}