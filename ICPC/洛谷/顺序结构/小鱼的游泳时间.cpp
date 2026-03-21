#include <iostream>
using namespace std;

int main() {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    
    // 算总分钟
    int total_min = (c * 60 + d) - (a * 60 + b);  // 结束 - 开始
    // 算小时和分钟
    int hour = total_min / 60;
    int min = total_min % 60;
    // 输出
    cout << hour << " " << min << "\n";
    return 0;
}