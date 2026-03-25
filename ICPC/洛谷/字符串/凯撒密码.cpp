#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    string s;
    cin >> n >> s;

    for (int i = 0; i < s.size(); i++) {
        // 套公式：向后移n位，超过z就循环
        s[i] = 'a' + (s[i] - 'a' + n) % 26;
    }

    cout << s << endl;
    return 0;
}