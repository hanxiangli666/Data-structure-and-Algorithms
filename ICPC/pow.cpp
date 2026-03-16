#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

long long a, b;
const int mod = 1e9 + 7;
cin >> a >> b;
long long power(long long a, long long b, long long mod) {
    long long result = 1;  // 最终答案，初始为1
    a %= mod;              // 先对底数取模

    while (b > 0) {
        if (b & 1) {                    
            result = result * a % mod;  
        }
        a = a * a % mod;  
        b >>= 1;          
    }

    return result;
}
int main() 
{
    cout << power(2, 11, mod) << endl;
    return 0;
}
