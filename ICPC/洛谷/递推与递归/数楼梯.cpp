#include <bits/stdc++.h>
using namespace std;

// 用数组模拟大数，digits[0]是个位，digits[1]是十位，以此类推（低位在前）
const int MAXD = 2000; // f(5000)大约1045位，开2000够用
int f[5001][MAXD];     // f[i] 表示走i阶楼梯的方案数（大数）

// 把大数 b 加到大数 a 上（a += b），len是当前最大位数
void bigAdd(int a[], int b[], int len) {
    for (int i = 0; i < len; i++) {
        a[i] += b[i];
        if (a[i] >= 10) {        // 产生进位
            a[i] -= 10;
            a[i + 1] += 1;       // 进到高位
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // 初始值
    f[1][0] = 1; // f(1) = 1
    f[2][0] = 2; // f(2) = 2

    // 递推，从第3阶开始
    for (int i = 3; i <= n; i++) {
        bigAdd(f[i], f[i - 1], MAXD - 1);
        bigAdd(f[i], f[i - 2], MAXD - 1);
    }

    // 找最高位（从高位往低位找第一个非零位）
    int top = MAXD - 1;
    while (top > 0 && f[n][top] == 0) top--;

    // 从高位到低位输出
    for (int i = top; i >= 0; i--) {
        cout << f[n][i];
    }
    cout << "\n";

    return 0;
}