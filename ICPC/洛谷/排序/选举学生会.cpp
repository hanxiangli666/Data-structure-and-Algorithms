#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<int> cnt(n + 1, 0);  // cnt[i] 表示编号 i 出现的次数，下标从 1 到 n

    for (int i = 0; i < m; i++) {
        int vote;
        cin >> vote;
        cnt[vote]++;
    }

    bool first = true;  // 控制输出格式：数字之间用空格隔开，行末不多一个空格
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < cnt[i]; j++) {
            if (!first) cout << ' ';
            cout << i;
            first = false;
        }
    }
    cout << endl;

    return 0;
}