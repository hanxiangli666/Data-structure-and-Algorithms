#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; // 多项式的最高次数
    cin >> n;

    vector<int> coefficients(n + 1); // 存所有项的系数
    for (int i = 0; i <= n; i++) cin >> coefficients[i];
    // coefficients[i] 对应 x 的幂次为 (n-i) 的项
    // 例如 n=5 时：coefficients[0]对应x^5，coefficients[5]对应x^0

    bool has_output = false; // 是否已经输出过至少一项（用来判断是不是第一项）

    for (int i = 0; i <= n; i++) {
        int coefficient = coefficients[i]; // 当前项的系数
        int power       = n - i;           // 当前项 x 的幂次

        if (coefficient == 0) continue; // 系数为0，跳过这一项

        // --- 第一步：输出符号 ---
        if (!has_output) {
            // 第一个非零项：负数输出'-'，正数不输出任何符号
            if (coefficient < 0) cout << "-";
        } else {
            // 后续项：正数输出'+'，负数输出'-'
            if (coefficient > 0) cout << "+";
            else                 cout << "-";
        }

        // --- 第二步：输出系数绝对值 ---
        int abs_coefficient = abs(coefficient);
        if (power == 0) {
            // 常数项（x^0）：直接输出系数绝对值
            cout << abs_coefficient;
        } else {
            // 含 x 的项：系数绝对值为1时省略不写
            if (abs_coefficient != 1) cout << abs_coefficient;
        }

        // --- 第三步：输出 x 和幂次 ---
        if (power == 1)     cout << "x";       // x^1 写成 x
        else if (power > 1) cout << "x^" << power; // x^2 及以上写完整
        // power == 0 时不输出 x

        has_output = true;
    }

    // 全部系数都为0的情况，直接输出 0
    if (!has_output) cout << "0";

    cout << "\n";
    return 0;
}