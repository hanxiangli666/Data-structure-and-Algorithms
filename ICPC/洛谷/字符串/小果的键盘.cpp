#include <bits/stdc++.h>
using namespace std;

// 统计字符串中VK出现次数
int countVK(string s) {
    int cnt = 0;
    for (int i = 0; i + 1 < s.size(); i++) {
        if (s[i] == 'V' && s[i+1] == 'K') cnt++;
    }
    return cnt;
}

int main() {
    int n;
    string s;
    cin >> n >> s;

    int ans = countVK(s);  // 不改任何字符的情况

    for (int i = 0; i < n; i++) {
        string t = s;  // 复制一份，不破坏原字符串
        // 把第i个字符改成另一个
        t[i] = (s[i] == 'V') ? 'K' : 'V';
        ans = max(ans, countVK(t));
    }

    cout << ans << "\n";
    return 0;
}