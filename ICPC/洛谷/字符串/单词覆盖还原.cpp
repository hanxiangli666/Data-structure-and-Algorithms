#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    cin >> s;

    // 统计各字母出现次数
    int cnt[26] = {0};
    for (char c : s) {
        if (c != '.') cnt[c - 'a']++;
    }

    int boy  = max({cnt['b'-'a'], cnt['o'-'a'], cnt['y'-'a']});
    int girl = max({cnt['g'-'a'], cnt['i'-'a'], cnt['r'-'a'], cnt['l'-'a']});

    cout << boy  << "\n";
    cout << girl << "\n";
    return 0;
}