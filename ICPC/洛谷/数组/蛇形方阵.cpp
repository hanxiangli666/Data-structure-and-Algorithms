#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int a[10][10] = {};
    int dr[] = {0, 1, 0, -1};  // 行变化：右下左上
    int dc[] = {1, 0, -1, 0};  // 列变化：右下左上
    
    int r = 0, c = 0, dir = 0;
    for (int num = 1; num <= n*n; num++) {
        a[r][c] = num;
        // 计算下一个位置
        int nr = r + dr[dir];
        int nc = c + dc[dir];
        // 越界或已填，转向
        if (nr < 0 || nr >= n || nc < 0 || nc >= n || a[nr][nc] != 0) {
            dir = (dir + 1) % 4;  // 顺时针转向
            nr = r + dr[dir];
            nc = c + dc[dir];
        }
        r = nr; c = nc;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%3d", a[i][j]);  // 占3个字符
        cout << "\n";
    }
    return 0;
}