#include <iostream>
using namespace std;

int main() {
    long long n;
    cin >> n;
    
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            // 找到较小质数，输出较大的
            cout << n / i << "\n";
            break;
        }
    }
    
    return 0;
}