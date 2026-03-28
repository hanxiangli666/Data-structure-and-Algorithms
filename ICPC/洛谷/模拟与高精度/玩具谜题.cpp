#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> dir(n);       // 朝向：0朝内，1朝外
    vector<string> name(n);   // 职业名

    for (int i = 0; i < n; i++) {
        cin >> dir[i] >> name[i];
    }

    int pos = 0; // 当前位置，从第0个小人开始

    for (int i = 0; i < m; i++) {
        int a, s;
        cin >> a >> s;

        // dir[pos] == a：朝内左数 或 朝外右数 → 逆时针 → 下标减小
        // dir[pos] != a：朝内右数 或 朝外左数 → 顺时针 → 下标增大
        if (dir[pos] == a) {
            pos = (pos - s % n + n) % n;
        } else {
            pos = (pos + s) % n;
        }
    }

    cout << name[pos] << "\n";
    return 0;
}