#include <iostream>
#include <string>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    string s = "";  // 存所有展开的字符
    int x;
    char c = '0';   // 从0开始交替
    
    while (cin >> x) {
        // 把x个c追加到s
        s += string(x, c);  // 生成x个c组成的字符串
        c = (c == '0') ? '1' : '0';  // 交替切换
    }
    
    // 每N个字符输出一行 s.substr(i, n) 就是从位置i取n个字符
    for (int i = 0; i < n*n; i += n) {
        cout << s.substr(i, n) << "\n";
    }
    return 0;
}