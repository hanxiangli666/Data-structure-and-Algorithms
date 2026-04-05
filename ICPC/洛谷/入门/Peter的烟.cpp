#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, k;
    cin >> n >> k;

    long long total = n;   // 总共抽的根数
    long long butts = n;   // 当前手里的烟蒂数

    while (butts >= k) {
        long long new_cigs = butts / k;   // 能换来几根新烟
        butts = butts % k + new_cigs;     // 换完剩的烟蒂 + 新烟抽完产生的烟蒂
        total += new_cigs;
    }

    cout << total << endl;

    return 0;
}