#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string revInt(string s) {
    reverse(s.begin(), s.end());
    // 手动去前导零
    int start = 0;
    while (start < s.size()-1 && s[start] == '0')
        start++;
    return s.substr(start);
}

string revDec(string s) {
    reverse(s.begin(), s.end());
    // 去前导零
    int start = 0;
    while (start < s.size()-1 && s[start] == '0')
        start++;
    s = s.substr(start);
    
    // 去末尾零
    int end = s.size()-1;
    while (end > 0 && s[end] == '0')
        end--;
    return s.substr(0, end+1);
}

int main() {
    string s;
    cin >> s;
    
    if (s.back() == '%') {
        // 百分数：去掉%，反转数字，加回%
        string num = s.substr(0, s.size()-1);
        cout << revInt(num) << "%" << "\n";
        
    } else if (s.find('/') != string::npos) {
        // 分数：按/分割，分别反转
        int pos = s.find('/');
        string num = s.substr(0, pos);      // 分子
        string den = s.substr(pos+1);       // 分母
        cout << revInt(num) << "/" << revInt(den) << "\n";
        
    } else if (s.find('.') != string::npos) {
        // 小数：按.分割，分别反转
        int pos = s.find('.');
        string intPart = s.substr(0, pos);  // 整数部分
        string decPart = s.substr(pos+1);   // 小数部分
        cout << revInt(intPart) << "." << revDec(decPart) << "\n";
        
    } else {
        // 整数：直接反转
        cout << revInt(s) << "\n";
    }
    return 0;
}