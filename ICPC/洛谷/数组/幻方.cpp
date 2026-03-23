#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int a[40][40] = {};
    int r = 1, c = (n+1)/2;  // 起点
    a[r][c] = 1;
    
    for (int k = 2; k <= n*n; k++) {
        int nr, nc;  // 下一个位置
        
        if (r == 1 && c == n) {
            // 规则3：向下
            nr = r+1; nc = c;
        } else if (r == 1) {
            // 规则1：最后一行，右一列
            nr = n; nc = c+1;
        } else if (c == n) {
            // 规则2：第一列，上一行
            nr = r-1; nc = 1;
        } else {
            // 规则4：右上为空就去右上，否则向下
            if (a[r-1][c+1] == 0) {
                nr = r-1; nc = c+1;
            } else {
                nr = r+1; nc = c;
            }
        }
        
        r = nr; c = nc;
        a[r][c] = k;
    }
    
    // 输出
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (j > 1) cout << " ";
            cout << a[i][j];
        }
        cout << "\n";
    }
    return 0;
}