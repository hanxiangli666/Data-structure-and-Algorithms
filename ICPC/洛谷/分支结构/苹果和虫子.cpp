#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int m, t, s;
    cin >> m >> t >> s;
    
    if (t == 0) {
        // 特殊情况
        cout << 0 << "\n";
    } else {
        // 正常情况
        int eaten = s / t;
        if (s % t != 0) eaten++;
        cout << max(0, m - eaten) << "\n";
    }
    
    return 0;
}