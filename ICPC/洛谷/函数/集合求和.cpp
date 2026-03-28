#include <bits/stdc++.h>
using namespace std;

int main() {
    long long sum = 0;
    long long n = 0;
    long long x;

    // 循环读入直到文件结束（EOF）
    while (cin >> x) {
        sum += x;
        n++;
    }

    // 答案 = 元素总和 × 2^(n-1)
    // 1LL << (n-1) 是 long long 类型的 2^(n-1)
    cout << sum * (1LL << (n - 1)) << "\n";

    return 0;
}