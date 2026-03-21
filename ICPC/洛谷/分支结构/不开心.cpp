#include <bits/stdc++.h>
using namespace std;

int main() {
    int maxUnhappy = 0;  // 记录最大不高兴值
    int ans = 0;         // 记录答案（哪天最不高兴）
    
    for (int i = 1; i <= 7; i++) {
        int a, b;
        cin >> a >> b;
        
        int total = a + b; // 当天总时间
        int unhappy = 0;
        
        if (total > 8) {
            unhappy = total - 8; // 不高兴值
        }
        
        // 注意：平局取靠前的天，所以用严格大于
        if (unhappy > maxUnhappy) {
            maxUnhappy = unhappy;
            ans = i; // 记录是第几天
        }
    }
    cout << ans << endl;
    return 0;
}