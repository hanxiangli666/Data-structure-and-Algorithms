#include <bits/stdc++.h>
using namespace std;

int a[2000001]; // 开在全局，避免栈溢出

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        cin >> a[i]; // 从下标1开始存，和题目"第i个"对齐
    }

    for (int j = 0; j < m; j++) {
        int q;
        cin >> q;
        cout << a[q] << "\n";
    }

    return 0;
}