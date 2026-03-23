#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    bool win[34] = {};  // 标记中奖号码
    for (int i = 0; i < 7; i++) {
        int x; cin >> x;
        win[x] = true;
    }
    
    int prize[8] = {};  // prize[0]=特等, prize[1]=一等...prize[6]=六等
    
    for (int i = 0; i < n; i++) {
        int match = 0;
        for (int j = 0; j < 7; j++) {
            int x; cin >> x;
            // 判断x是否在中奖号码中
            if (win[x] == true) match++;
            
        }
        // 根据match更新prize数组
        if (match > 0) prize[7 - match]++;
    }
    
    // 输出prize[0]到prize[6]
    for (int i = 0; i < 7; i++) cout << prize[i] << " ";
    return 0;
}