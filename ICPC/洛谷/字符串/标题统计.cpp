#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    getline(cin, s);  // 读整行，保留空格

    int cnt = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != ' ') cnt++;  // 不是空格就计数
    }

    cout << cnt << endl;
    return 0;
}