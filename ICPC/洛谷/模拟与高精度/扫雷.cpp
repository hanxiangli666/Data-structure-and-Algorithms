#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    for (int i = 0; i < n; i++) cin >> grid[i];

    // 8个方向的偏移量
    int dx[] = {-1,-1,-1, 0, 0,+1,+1,+1};
    int dy[] = {-1, 0,+1,-1,+1,-1, 0,+1};

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '*') {
                cout << '*';
            } else {
                int cnt = 0;
                for (int k = 0; k < 8; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    // 判断邻居是否在网格范围内
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        if (grid[ni][nj] == '*') cnt++;
                    }
                }
                cout << cnt;
            }
        }
        cout << "\n";
    }

    return 0;
}