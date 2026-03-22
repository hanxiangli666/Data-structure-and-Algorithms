#include <iostream>
using namespace std;

int main() {
    int k;
    cin >> k;
    
    long long sum = 0;
    int stage = 1;  // 当前阶段（每天金币数）
    
    while (k > 0) {
        if (k >= stage) {
            // 完整取一个阶段
            k -= stage;
            sum += stage * stage;
        } else {
            // 不够一个阶段，取剩余的k天
            sum += stage * k;
            k = 0;
        }
        stage++;
    }
    
    cout << sum << "\n";
    return 0;
}