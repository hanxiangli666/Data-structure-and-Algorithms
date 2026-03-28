#include <bits/stdc++.h>
using namespace std;

int grid[505][505];
int tmp[505][505];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    // 初始化：按行从左到右填 1 到 n²
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            grid[i][j] = (i - 1) * n + j;

    while (m--) {
        int x, y, r, z;
        cin >> x >> y >> r >> z;

        int k = 2 * r + 1; // 子矩阵边长
        int x0 = x - r;    // 子矩阵左上角行
        int y0 = y - r;    // 子矩阵左上角列

        // 第一步：拷贝子矩阵到 tmp（用局部坐标 0~k-1）
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++)
                tmp[i][j] = grid[x0 + i][y0 + j];

        // 第二步：按旋转公式写回
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                if (z == 0) {
                    // 顺时针：(i,j) → (j, k-1-i)
                    grid[x0 + j][y0 + k - 1 - i] = tmp[i][j];
                } else {
                    // 逆时针：(i,j) → (k-1-j, i)
                    grid[x0 + k - 1 - j][y0 + i] = tmp[i][j];
                }
            }
        }
    }

    // 输出
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << grid[i][j];
            if (j < n) cout << " ";
        }
        cout << "\n";
    }

    return 0;
}