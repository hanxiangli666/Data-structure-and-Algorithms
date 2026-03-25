#include <bits/stdc++.h>
using namespace std;

// 判断是否是质数
bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {  // 只需试除到根号n
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    string s;
    cin >> s;

    int cnt[26] = {0};  // 统计每个字母出现次数，初始化为0

    for (int i = 0; i < s.size(); i++) {
        cnt[s[i] - 'a']++;  // 当前字母对应的计数+1
    }

    int maxn = 0, minn = INT_MAX;  // INT_MAX 是整数最大值，用来初始化minn
    for (int i = 0; i < 26; i++) {
        if (cnt[i] > 0) {  // 只看出现过的字母
            maxn = max(maxn, cnt[i]);
            minn = min(minn, cnt[i]);
        }
    }

    int diff = maxn - minn;
    if (isPrime(diff)) {
        cout << "Lucky Word" << endl;
        cout << diff << endl;
    } else {
        cout << "No Answer" << endl;
        cout << 0 << endl;
    }

    return 0;
}