#include <bits/stdc++.h>
using namespace std;

const int MAXN = 10001;
bool isPrime[MAXN]; // isPrime[i] = true 表示 i 是质数

// 埃氏筛：筛出 [2, MAXN) 内所有质数
void sieve() {
    fill(isPrime, isPrime + MAXN, true); // 先全部标记为质数
    isPrime[0] = isPrime[1] = false;     // 0 和 1 不是质数

    for (int i = 2; i < MAXN; i++) {
        if (isPrime[i]) {
            // i 是质数，把 i 的所有倍数标记为合数
            // 从 i*i 开始（i*i 之前的倍数已经被更小的质数筛过了）
            for (long long j = (long long)i * i; j < MAXN; j += i) {
                isPrime[j] = false;
            }
        }
    }
}

int main() {
    sieve();

    int n;
    cin >> n;

    for (int x = 4; x <= n; x += 2) {   // 枚举每个偶数
        for (int a = 2; a <= x / 2; a++) { // a 从小到大枚举，不超过 x/2
            if (isPrime[a] && isPrime[x - a]) {
                cout << x << "=" << a << "+" << (x - a) << "\n";
                break; // 找到第一个（a 最小的）就停止
            }
        }
    }

    return 0;
}
