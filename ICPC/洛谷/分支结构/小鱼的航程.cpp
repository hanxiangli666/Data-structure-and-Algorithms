#include <iostream>
using namespace std;

int main() {
    int x, n;
    cin >> x >> n;
    
    int total = 0;
    int day = x;  // 当前星期几
    
    for (int i = 0; i < n; i++) {
        // 判断是否是工作日，累加路程
        if (day < 6) total += 250;
        // 更新到下一天（注意周日后回到周一）
        day = day % 7 + 1;
    }
    
    cout << total << "\n";
    return 0;
}