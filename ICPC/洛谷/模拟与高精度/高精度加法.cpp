#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    cin >> a >> b;

    string result;
    int i = a.size() - 1; // 指向a的末位
    int j = b.size() - 1; // 指向b的末位
    int carry = 0;         // 进位

    // 只要还有位没处理，或者还有进位，就继续
    while (i >= 0 || j >= 0 || carry) {
        int digitA = (i >= 0) ? (a[i--] - '0') : 0; // a取完后补0
        int digitB = (j >= 0) ? (b[j--] - '0') : 0; // b取完后补0

        int sum = digitA + digitB + carry;
        result += (char)('0' + sum % 10); // 当前位存入结果
        carry = sum / 10;                 // 更新进位
    }

    // 结果是从低位到高位存的，翻转一下
    reverse(result.begin(), result.end());

    // 特判：如果两个都是0，结果是空串，输出"0"
    if (result.empty()) result = "0";

    cout << result << "\n";
    return 0;
}