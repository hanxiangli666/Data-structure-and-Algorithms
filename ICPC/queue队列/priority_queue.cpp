#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 最大堆（默认）：值最大的先出
    priority_queue<int> pq;

    // push：加入堆
    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);

    // size：元素个数
    cout << pq.size() << "\n";  // 4

    // empty：是否为空
    cout << pq.empty() << "\n"; // 0（false，不为空）

    // top：看堆顶（最大值），不删除
    cout << pq.top() << "\n";   // 50

    // pop：删除堆顶（不返回值！）
    // 正确做法：先 top() 取值，再 pop() 删除
    int val = pq.top();  // 先取出 50
    pq.pop();            // 再删除
    cout << val << "\n";        // 50
    cout << pq.top() << "\n";   // 30（下一个最大值）

    // size 再次确认
    cout << pq.size() << "\n";  // 3

    // 清空：同样没有 clear()，用 while 循环
    while (!pq.empty()) {
        pq.pop();
    }
    if (pq.empty()) {
        cout << "最大堆已空\n";
    }

    // ----------------------------------------
    // 最小堆：值最小的先出（写法固定，背下来）
    priority_queue<int, vector<int>, greater<int>> pq2;

    pq2.push(30);
    pq2.push(10);
    pq2.push(50);
    pq2.push(20);

    // top：看堆顶（最小值）
    cout << pq2.top() << "\n";  // 10

    pq2.pop();
    cout << pq2.top() << "\n";  // 20（下一个最小值）

    return 0;
}