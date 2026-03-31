#include <bits/stdc++.h>
using namespace std;

int n, k;
int x[25];
int ans = 0;

// 判断是否为素数
bool isPrime(int num) {
    if (num < 2) return false;
    for (int i = 2; (long long)i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

// start: 从哪个下标开始选
// cnt:   已经选了几个数
// sum:   已选数字的和
void dfs(int start, int cnt, int sum) {
    if (cnt == k) {              // 已选够k个
        if (isPrime(sum)) ans++;
        return;
    }

    for (int i = start; i < n; i++) {
        dfs(i + 1, cnt + 1, sum + x[i]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> k;
    for (int i = 0; i < n; i++) cin >> x[i];

    dfs(0, 0, 0);

    cout << ans << "\n";
    return 0;
}