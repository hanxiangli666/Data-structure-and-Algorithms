#include <bits/stdc++.h>
using namespace std;

// 高精度加法：返回 a + b
string add(const string& a, const string& b) {
    string result;
    int i = a.size() - 1;
    int j = b.size() - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry) {
        int da = (i >= 0) ? (a[i--] - '0') : 0;
        int db = (j >= 0) ? (b[j--] - '0') : 0;
        int sum = da + db + carry;
        result += (char)('0' + sum % 10);
        carry = sum / 10;
    }

    reverse(result.begin(), result.end());
    if (result.empty()) result = "0";
    return result;
}

// 高精度 × 普通整数：返回 a * x
string mul(const string& a, int x) {
    string result;
    int carry = 0;

    for (int i = a.size() - 1; i >= 0; i--) {
        int prod = (a[i] - '0') * x + carry;
        result += (char)('0' + prod % 10);
        carry = prod / 10;
    }

    // 处理最高位的进位（可能不止一位）
    while (carry) {
        result += (char)('0' + carry % 10);
        carry /= 10;
    }

    reverse(result.begin(), result.end());
    if (result.empty()) result = "0";
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    string fact = "1"; // 当前阶乘，从 1! 开始
    string sum  = "0"; // 累加和

    for (int i = 1; i <= n; i++) {
        fact = mul(fact, i);  // fact = i!
        sum  = add(sum, fact); // sum += i!
    }

    cout << sum << "\n";
    return 0;
}