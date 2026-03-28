#include <bits/stdc++.h>
using namespace std;

// 模拟一种分制下的所有局
// s: 完整的比分字符串, target: 11 或 21
void simulate(const string& s, int target) {
    int w = 0, l = 0; // 当前局华华和对手的得分

    for (char c : s) {
        if (c == 'W') w++;
        else l++;

        // 判断当前局是否结束
        // 条件：某方达到目标分 且 分差 >= 2
        if ((w >= target || l >= target) && abs(w - l) >= 2) {
            cout << w << ":" << l << "\n";
            w = 0; l = 0; // 重置，开始新的一局
        }
    }

    // 最后一局（可能没打完）也要输出
    cout << w << ":" << l << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s; // 存储所有有效字符（E之前）
    string line;

    while (cin >> line) {
        for (char c : line) {
            if (c == 'E') goto done; // 遇到E立刻停止读取
            if (c == 'W' || c == 'L') s += c;
        }
    }
    done:

    simulate(s, 11);
    cout << "\n";
    simulate(s, 21);

    return 0;
}