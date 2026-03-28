#include <bits/stdc++.h>
using namespace std;

int grid[1024][1024]; // 2^10 = 1024，开最大

// r,c 是当前矩阵左上角坐标，size 是当前矩阵边长
void solve(int r, int c, int size) {
    if (size == 1) return; // 单个格子，无法再分

    int half = size / 2;

    // 把左上角 half×half 的区域全部填 0（赦免）
    for (int i = r; i < r + half; i++) {
        for (int j = c; j < c + half; j++) {
            grid[i][j] = 0;
        }
    }

    // 对另外三个子矩阵递归处理
    solve(r,        c + half, half); // 右上
    solve(r + half, c,        half); // 左下
    solve(r + half, c + half, half); // 右下
}

int main() {
    int n;
    cin >> n;
    int size = 1 << n; // 2^n，等价于 pow(2,n) 但更快

    // 初始化全部为 1
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            grid[i][j] = 1;

    solve(0, 0, size);

    // 输出
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j > 0) cout << " ";
            cout << grid[i][j];
        }
        cout << "\n";
    }

    return 0;
}