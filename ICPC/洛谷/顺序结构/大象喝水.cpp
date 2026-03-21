#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int h, r;
    cin >> h >> r;
    
    // 算体积
    double v = 3.14 * r * r * h;
    // 算桶数（向上取整）
    int bottle = ceil(20000.0 / v);
    // 输出
    cout << bottle << "\n";
    return 0;
}