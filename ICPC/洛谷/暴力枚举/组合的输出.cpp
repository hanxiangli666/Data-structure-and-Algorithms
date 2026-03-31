#include <bits/stdc++.h>
#include <iomanip>   // setw 需要这个头文件
using namespace std;

int n, r;
int chosen[25];   // 记录已选的数

void dfs(int start, int cnt) {
    if (cnt == r) {              // 选够r个了，输出
        for (int i = 0; i < r; i++) {
            cout << setw(3) << chosen[i];
        }
        cout << "\n";
        return;
    }

    for (int i = start; i <= n; i++) {
        chosen[cnt] = i;                // 记录选了i
        dfs(i + 1, cnt + 1);            // 下一层从i+1开始
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> r;
    dfs(1, 0);   // 从1开始选，已选0个

    return 0;
}