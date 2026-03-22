#include <iostream>
using namespace std;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

void check(int p, int a, int b) {
    if (p >= a && p <= b && isPrime(p))
        cout << p << "\n";
}

int main() {
    int a, b;
    cin >> a >> b;
    
    // 1位回文质数
    for (int d1 = 1; d1 <= 9; d1 += 2)
        check(d1, a, b);
    
    // 2位只有11
    check(11, a, b);
    
    // 3位：d1 d2 d1
    for (int d1 = 1; d1 <= 9; d1 += 2)
        for (int d2 = 0; d2 <= 9; d2++)
            check(100*d1 + 10*d2 + d1, a, b);
    
    // 5位：d1 d2 d3 d2 d1（题目已给）
    for (int d1 = 1; d1 <= 9; d1 += 2)
        for (int d2 = 0; d2 <= 9; d2++)
            for (int d3 = 0; d3 <= 9; d3++)
                check(10000*d1 + 1000*d2 + 100*d3 + 10*d2 + d1, a, b);
    
    // 7位：自己补全
    for (int d1 = 1; d1 <= 9; d1 += 2)
        for (int d2 = 0; d2 <= 9; d2++)
            for (int d3 = 0; d3 <= 9; d3++)
                for (int d4 = 0; d4 <= 9; d4++) check(1000000*d1 + 100000*d2 + 10000*d3 + 1000*d4 + 100*d3 + 10*d2 + d1, a, b);    
    return 0;
}