#include <iostream>
using namespace std;

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    
    bool light[101][101] = {};
    
    // 火把偏移量
    int dr[] = {0,0,0,0,1,-1,2,-2,1,1,-1,-1};
    int dc[] = {1,-1,2,-2,0,0,0,0,1,-1,1,-1};
    
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        light[x][y] = true;  // 火把本身
        for (int d = 0; d < 12; d++) {
            int nx = x+dr[d], ny = y+dc[d];
            if (nx>=1 && nx<=n && ny>=1 && ny<=n)
                light[nx][ny] = true;
        }
    }
    
    for (int i = 0; i < k; i++) {
        int x, y;
        cin >> x >> y;
        // 萤石照亮5×5范围
        for (int dx = -2; dx <= 2; dx++)
            for (int dy = -2; dy <= 2; dy++) {
                int nx = x+dx, ny = y+dy;
                if (nx>=1 && nx<=n && ny>=1 && ny<=n)
                    light[nx][ny] = true;
            }
    }
    
    // 统计暗格
    int cnt = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (!light[i][j]) cnt++;
    
    cout << cnt << "\n";
    return 0;
}