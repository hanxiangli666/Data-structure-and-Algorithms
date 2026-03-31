#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n, m;
    cin >> n >> m;
    
    long long sq = 0, rect = 0;
    
    for (long long i = 1; i <= n; i++) {
        for (long long j = 1; j <= m; j++) {
            long long cnt = (n - i + 1) * (m - j + 1); // 该尺寸矩形的数量
            if (i == j) {
                sq += cnt;   // 正方形
            } else {
                rect += cnt; // 非正方形长方形
            }
        }
    }
    
    cout << sq << " " << rect << endl;
    
    return 0;
}