#include <bits/stdc++.h>
using namespace std;

// 返回第 day 天早上的桃子数，共 n 天
int peach(int day, int n) {
    if (day == n) return 1;                    // 第 n 天只剩 1 个
    return (peach(day + 1, n) + 1) * 2;       // 往前推一天
}

int main() {
    int n;
    cin >> n;
    cout << peach(1, n) << "\n";
    return 0;
}