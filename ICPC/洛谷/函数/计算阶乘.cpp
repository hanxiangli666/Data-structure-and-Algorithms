#include <bits/stdc++.h>
using namespace std;

int factorial(int n) {
    if (n == 1) return 1;               // 终止条件
    return n * factorial(n - 1);        // 递推关系
}

int main() {
    int n;
    cin >> n;
    cout << factorial(n) << "\n";
    return 0;
}