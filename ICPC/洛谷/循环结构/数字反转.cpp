#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    string s;
    cin >> s;
    
    bool negative = (s[0] == '-');
    string num = negative ? s.substr(1) : s;
    
    reverse(num.begin(), num.end());  // 直接反转字符串
    
    // 转成整数输出（自动去前导零）
    int result = stoi(num);  // stoi把字符串转成整数
    
    if (negative) cout << "-";
    cout << result << "\n";
    return 0;
}