#include <bits/stdc++.h>
using namespace std;

char grid[10][10]; // 存储地图，grid[行][列]

// 四个方向的行列偏移量（顺时针顺序：北、东、南、西）
// dr = delta row（行的变化量），dc = delta col（列的变化量）
int dr[] = {-1, 0, 1, 0}; // 北：行-1，东：行不变，南：行+1，西：行不变
int dc[] = { 0, 1, 0,-1}; // 北：列不变，东：列+1，南：列不变，西：列-1

// 尝试移动一步
// r, c : 当前行列坐标（传引用，函数内修改会影响外部变量）
// d    : 当前朝向（传引用，转向时会修改）
void move(int& r, int& c, int& d) {
    int nr = r + dr[d]; // nr = next row，尝试移动后的新行坐标
    int nc = c + dc[d]; // nc = next col，尝试移动后的新列坐标

    // 判断新位置是否合法（不越界 且 不是障碍物）
    if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10 && grid[nr][nc] != '*') {
        r = nr; // 合法：更新行坐标
        c = nc; // 合法：更新列坐标
    } else {
        d = (d + 1) % 4; // 不合法：顺时针转90°（0北→1东→2南→3西→0北）
        // 注意：转向这一分钟位置不变，下一分钟才按新方向走
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 牛的坐标：cr = cow row（牛所在行），cc = cow col（牛所在列）
    // John的坐标：fr = farmer row（John所在行），fc = farmer col（John所在列）
    int cr, cc, fr, fc;

    // 读入地图，同时找到牛(C)和John(F)的初始位置
    for (int i = 0; i < 10; i++) {
        cin >> grid[i];
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == 'C') { cr = i; cc = j; } // 记录牛的初始位置
            if (grid[i][j] == 'F') { fr = i; fc = j; } // 记录John的初始位置
        }
    }

    int cd = 0; // cow direction：牛的初始朝向，0 = 北
    int fd = 0; // farmer direction：John的初始朝向，0 = 北

    // 状态总数 = 100(牛位置) × 4(牛朝向) × 100(John位置) × 4(John朝向) = 160000
    // 超过160000步没相遇，说明已进入循环，永远不会相遇
    for (int t = 1; t <= 160000; t++) {
        move(cr, cc, cd); // 牛移动一步
        move(fr, fc, fd); // John移动一步

        // 判断是否相遇（移动结束后在同一格）
        if (cr == fr && cc == fc) {
            cout << t << "\n";
            return 0;
        }
    }

    // 超过最大步数仍未相遇，输出0
    cout << 0 << "\n";
    return 0;
}