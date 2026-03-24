#include <iostream>
#include <string>
#include <cmath>
using namespace std;

int main() {
    string line, s = "";
    while (cin >> line) s += line;
    
    int n = (int)sqrt(s.size());
    cout << n;
    
    char cur = '0';  // 从0开始
    int cnt = 0;
    for (char c : s) {
        if (c == cur) {
            cnt++;
        } else {
            cout << " " << cnt;
            cnt = 1;
            cur = (cur == '0') ? '1' : '0';
        }
    }
    cout << " " << cnt << "\n";  
    // 最后一段的cnt永远没机会触发"遇到不同字符"的条件，所以循环结束后必须手动输出一次
    
    return 0;
}