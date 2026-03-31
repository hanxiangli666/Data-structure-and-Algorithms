#include <bits/stdc++.h>
using namespace std;

// 检查 x, y, z 三个数合起来是否恰好用了1~9各一次
bool check(int x, int y, int z) {
    int digits[9];
    int idx = 0;
    // 拆出 x 的三位
    digits[idx++] = x / 100;
    digits[idx++] = x / 10 % 10;
    digits[idx++] = x % 10;
    // 拆出 y 的三位
    digits[idx++] = y / 100;
    digits[idx++] = y / 10 % 10;
    digits[idx++] = y % 10;
    // 拆出 z 的三位
    digits[idx++] = z / 100;
    digits[idx++] = z / 10 % 10;
    digits[idx++] = z % 10;

    sort(digits, digits + 9);

    for (int i = 0; i < 9; i++) {
        if (digits[i] != i + 1) return false; // 应该是1,2,3,...,9
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int A, B, C;
    cin >> A >> B >> C;

    if (A == 0) {           // x = k*0 = 0，不是三位数，直接无解
        cout << "No!!!\n";
        return 0;
    }

    bool found = false;

    for (int k = 1; k <= 999; k++) {
        int x = k * A;
        int y = k * B;
        int z = k * C;

        // 三个数必须都是三位数
        if (x < 100 || x > 999) continue;
        if (y < 100 || y > 999) continue;
        if (z < 100 || z > 999) continue;

        if (check(x, y, z)) {
            cout << x << " " << y << " " << z << "\n";
            found = true;
        }
    }

    if (!found) cout << "No!!!\n";

    return 0;
}