#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    while (cin >> n) {          // while 循环处理多组测试数据，读到 EOF 为止
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        if (n == 1) {
            cout << "Jolly" << endl;
            continue;           // 处理完这组，继续读下一组
        }

        // 计算相邻差的绝对值
        vector<int> diff(n - 1);
        for (int i = 0; i < n - 1; i++) {
            diff[i] = abs(a[i + 1] - a[i]);
        }

        // 排序差值数组
        sort(diff.begin(), diff.end());

        // 检查是否等于 [1, 2, ..., n-1]
        bool jolly = true;
        for (int i = 0; i < n - 1; i++) {
            if (diff[i] != i + 1) {
                jolly = false;
                break;
            }
        }

        cout << (jolly ? "Jolly" : "Not jolly") << endl;
    }

    return 0;
}