#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    // 外层大小固定为 n+1（下标从1开始）
    // 内层先都是空 vector，按需扩容
    vector<vector<int>> cabinet(n + 1);

    while (q--) {
        int op;
        cin >> op;

        if (op == 1) {
            int i, j, k;
            cin >> i >> j >> k;

            // 如果格子 j 超出当前内层大小，扩容
            if ((int)cabinet[i].size() <= j) {
                cabinet[i].resize(j + 1, 0); // 新格子默认为0
            }
            cabinet[i][j] = k;

        } else {
            int i, j;
            cin >> i >> j;
            cout << cabinet[i][j] << "\n";
        }
    }

    return 0;
}