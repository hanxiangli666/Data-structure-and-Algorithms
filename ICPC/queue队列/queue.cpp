#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    queue<int> q;

    // push：加入队尾
    q.push(10);
    q.push(20);
    q.push(30);
    // 现在队列从头到尾：10, 20, 30

    // size：元素个数
    cout << q.size() << "\n";   // 3

    // empty：是否为空
    cout << q.empty() << "\n";  // 0（false，不为空）

    // front：看队头，不删除
    cout << q.front() << "\n";  // 10

    // back：看队尾，不删除
    cout << q.back() << "\n";   // 30

    // pop：删除队头（注意：不返回值！）
    // 正确做法：先用 front() 取值，再用 pop() 删除
    int val = q.front();  // 先取出 10
    q.pop();              // 再删除
    cout << val << "\n";        // 10
    cout << q.front() << "\n";  // 20（10 已被删除）

    // size 再次确认
    cout << q.size() << "\n";   // 2

    // queue 也没有 clear()，同样用 while 循环清空
    while (!q.empty()) {
        q.pop();
    }

    // 确认已空
    if (q.empty()) {
        cout << "queue 已空\n";
    }

    return 0;
}