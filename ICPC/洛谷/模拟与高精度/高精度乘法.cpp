#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    cin >> a >> b;

    int n = a.size(), m = b.size();
    vector<int> res(n + m, 0); // 结果数组，最多 n+m 位

    // i 从 a 末尾往前，j 从 b 末尾往前
    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            int mul = (a[i] - '0') * (b[j] - '0');
            // a[i] 是从末尾数第 (n-1-i) 位
            // b[j] 是从末尾数第 (m-1-j) 位
            // 两者相乘贡献到结果的第 (n-1-i)+(m-1-j) 位
            int pos = (n - 1 - i) + (m - 1 - j);
            res[pos] += mul;
        }
    }

    // 统一处理进位（从低位到高位）
    for (int i = 0; i < n + m - 1; i++) {
        res[i + 1] += res[i] / 10;
        res[i] %= 10;
    }

    // 从高位找到第一个非零位开始输出
    int start = n + m - 1;
    while (start > 0 && res[start] == 0) start--;

    for (int i = start; i >= 0; i--) {
        cout << res[i];
    }
    cout << "\n";

    return 0;
}