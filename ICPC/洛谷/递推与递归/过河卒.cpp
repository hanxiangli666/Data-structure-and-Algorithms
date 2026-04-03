#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, cx, cy;
    cin >> n >> m >> cx >> cy;

    // 标记控制点
    // 棋盘大小是 (n+1) x (m+1)，坐标从(0,0)到(n,m)
    bool blocked[25][25] = {};

    // 马的8个跳跃偏移
    int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dy[] = {-1,  1, -2,  2, -2, 2, -1, 1};

    // 标记马本身
    blocked[cx][cy] = true;

    // 标记马能跳到的8个点
    for (int k = 0; k < 8; k++) {
        int nx = cx + dx[k];
        int ny = cy + dy[k];
        if (nx >= 0 && nx <= n && ny >= 0 && ny <= m) {
            blocked[nx][ny] = true;
        }
    }

    // DP
    long long f[25][25] = {};
    f[0][0] = 1;

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0 && j == 0) continue; // 起点已设好
            if (blocked[i][j]) {             // 控制点，路径数为0
                f[i][j] = 0;
                continue;
            }
            if (i > 0) f[i][j] += f[i-1][j]; // 从上方来
            if (j > 0) f[i][j] += f[i][j-1]; // 从左方来
        }
    }

    cout << f[n][m] << "\n";
    return 0;
}