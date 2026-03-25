#include <bits/stdc++.h>
using namespace std;

int main() {
    int q;
    cin >> q;
    string s;
    cin >> s;

    for (int i = 0; i < q; i++) {
        int op;
        cin >> op;

        if (op == 1) {
            string str;
            cin >> str;
            s += str;                    // 末尾追加
            cout << s << "\n";

        } else if (op == 2) {
            int a, b;
            cin >> a >> b;
            s = s.substr(a, b);          // 从第a个字符起，取b个
            cout << s << "\n";

        } else if (op == 3) {
            int a;
            string str;
            cin >> a >> str;
            s.insert(a, str);            // 在第a个字符前插入str
            cout << s << "\n";

        } else {
            string str;
            cin >> str;
            int pos = s.find(str);       // 查找子串
            if (pos == string::npos) cout << -1 << "\n";
            else cout << pos << "\n";
        }
    }

    return 0;
}