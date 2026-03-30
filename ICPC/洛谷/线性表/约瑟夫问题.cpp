#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    queue<int> q;
    for (int i = 1; i <= n; i++) {
        q.push(i); // 1到n依次入队
    }

    while (!q.empty()) {
        // 数 m-1 次：把队头移到队尾
        for (int i = 1; i <= m - 1; i++) {
            q.push(q.front());
            q.pop();
        }
        // 第 m 个人出列
        cout << q.front();
        q.pop();

        if (!q.empty()) cout << " "; // 数字之间空格，最后不加
    }
    cout << "\n";

    return 0;
}