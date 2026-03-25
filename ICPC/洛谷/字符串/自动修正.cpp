#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    cin >> s;  // 读入字符串（不含空格）
    
    for (int i = 0; i < s.size(); i++) {
        // 用 toupper() 把每个字符转为大写
        // toupper() 对非字母字符直接原样返回，不用担心数字或符号
        s[i] = toupper(s[i]);
    }
    
    cout << s << endl;
    return 0;
}