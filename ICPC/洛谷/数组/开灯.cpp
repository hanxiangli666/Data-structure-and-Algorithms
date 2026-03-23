#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    bool lamp[2000001] = {};
    
    for (int i = 0; i < n; i++) {
        double a;
        int t;
        cin >> a >> t;
        
        for (int k = 1; k <= t; k++) {
            int idx = (int)(k * a);  // 取整数部分
            if (idx > 0) lamp[idx] = !lamp[idx];
        }
    }
    
    for (int i = 1; i <= 2000000; i++) {
        if (lamp[i]) {
            cout << i << "\n";
            break;
        }
    }
    return 0;
}