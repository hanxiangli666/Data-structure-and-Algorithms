#include <bits/stdc++.h>
using namespace std;

int main() {
    // 按键盘分组，每组字母对应1,2,3,...次
    // 字符串里第几个字母就要按几下
    string keys[] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

    int cost[26] = {0};  // cost[i] = 第i个字母需要按几下

    for (int i = 0; i < 8; i++) {          // 遍历8个按键
        for (int j = 0; j < keys[i].size(); j++) {  // 遍历这个键上的字母
            char c = keys[i][j];
            cost[c - 'a'] = j + 1;         // 第j个字母要按j+1下
        }
    }

    string s;
    getline(cin, s);  // 句子可能有空格，用getline读整行

    int total = 0;
    for (char c : s) {
        if (c == ' ') total += 1;          // 空格按0键一下
        else total += cost[c - 'a'];       // 字母查表
    }

    cout << total << "\n";
    return 0;
}
