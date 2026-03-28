#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    int cnt[26] = {};
    string line;
    for (int i = 0; i < 4; i++) {
        getline(cin, line);
        for (char c : line)
            if (c >= 'A' && c <= 'Z') cnt[c - 'A']++;
    }

    int maxCnt = *max_element(cnt, cnt + 26);
    for (int row = maxCnt; row >= 1; row--) {
        int last = -1;
        for (int i = 25; i >= 0; i--) {
            if (cnt[i] >= row) {last = i; break; }
        }
        for (int i = 0; i <= last; i++) {
            if (i > 0) cout << " ";
            cout << (cnt[i] >= row ? "*" : " ");
        }
        cout << "\n";
    }
    // 字母行固定输出A到Z
    for (int i = 0; i < 26; i++) {
    if (i > 0) cout << " ";
    cout << (char)('A' + i);
    }
    cout << "\n";

}