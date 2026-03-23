#include <iostream>
using namespace std;

int main() {
    int w, x, h;
    cin >> w >> x >> h;
    
    bool cube[21][21][21];
    // 初始化全为true
    for (int i = 1; i <= w; i++)
        for (int j = 1; j <= x; j++)
            for (int k = 1; k <= h; k++)
                cube[i][j][k] = true;
    
    int q;
    cin >> q;
    while (q--) {
        int x1,y1,z1,x2,y2,z2;
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
        // 把区域内全标记为false
        for (int i = x1; i <= x2; i++)
            for (int j = y1; j <= y2; j++)
                for (int k = z1; k <= z2; k++)
                    cube[i][j][k] = false;
        
    }
    
    // 统计剩余方块
    int cnt = 0;
    for (int i = 1; i <= w; i++)
        for (int j = 1; j <= x; j++)
            for (int k = 1; k <= h; k++)
                if (cube[i][j][k]) cnt++;
    cout << cnt << "\n";
    return 0;
}