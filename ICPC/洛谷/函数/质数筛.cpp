#include <bits/stdc++.h>
using namespace std;

// 判断 n 是否是质数
bool isPrime(int n) {
    if (n < 2) return false;          // 0 和 1 不是质数
    for (int i = 2; i * i <= n; i++) { // 只需要试除到 sqrt(n)
        if (n % i == 0) return false;  // 能被整除，不是质数
    }
    return true;
}

int main() {
    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    bool first = true; // 用来控制输出格式：第一个数前面不加空格
    for (int i = 0; i < n; i++) {
        if (isPrime(a[i])) {
            if (!first) cout << " "; // 不是第一个质数，先输出空格
            cout << a[i];
            first = false;
        }
    }
    cout << endl;

    return 0;
}