#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    stack<int> s;

    // push：压入栈顶
    s.push(10);
    s.push(20);
    s.push(30);
    // 现在栈从底到顶：10, 20, 30

    // size：元素个数
    cout << s.size() << "\n";   // 3

    // empty：是否为空
    cout << s.empty() << "\n";  // 0（false，不为空）

    // top：看栈顶，不删除
    cout << s.top() << "\n";    // 30

    // pop：删除栈顶（注意：不返回值！）
    // 正确做法：先用 top() 取值，再用 pop() 删除
    int val = s.top();  // 先取出 30
    s.pop();            // 再删除
    cout << val << "\n";        // 30
    cout << s.top() << "\n";    // 20（30已被删除）

    // size 再次确认
    cout << s.size() << "\n";   // 2

    // 清空：stack 没有 clear()，用 while 循环代替
    while (!s.empty()) {
        s.pop();
    }

    // 确认已空
    if (s.empty()) {
        cout << "stack 已空\n";
    }

    return 0;
}