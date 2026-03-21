#include <iostream>
using namespace std;

int main() {
    int a, b;
    cin >> a >> b;
    
    // 算总角数
    int total_mon = a * 10 + b;
    // 算最多买几支
    int ans = total_mon / 19;
    // 输出
    cout << ans << "\n";
    return 0;
}