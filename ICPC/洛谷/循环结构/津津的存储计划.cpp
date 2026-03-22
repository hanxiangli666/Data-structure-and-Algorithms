#include <iostream>
using namespace std;

int main() {
    int budget[12];
    for (int i = 0; i < 12; i++) cin >> budget[i];
    
    int hand = 0, save = 0;
    
    for (int i = 0; i < 12; i++) {
        hand += 300;
        
        if (hand < budget[i]) {
            cout << -(i + 1) << "\n";
            return 0;
        }

        
        // 月末剩余
        int remain = hand - budget[i];
        // 存整百给妈妈，自己留零头
        int give = ( remain /100) * 100;
        hand = remain % 100;
        save += give;
        
    }
    
    // 输出年末总钱数
    double total = save * 1.2 + hand;
    cout << total << "\n";
    return 0;
}