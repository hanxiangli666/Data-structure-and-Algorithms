#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int min_val = 1e9;  // 初始设一个很大的数
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        if (x < min_val) min_val = x;
    }
    
    cout << min_val << "\n";
    return 0;
}