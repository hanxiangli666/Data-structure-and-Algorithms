#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    double sq5 = sqrt(5.0);
    // 套公式计算，printf保留两位小数输出
    double a = (1 + sq5) / 2;
    double b = (1 - sq5) / 2;
    double result = (pow(a, n) - pow(b, n)) / sq5;
    printf("%.2f", result);
    return 0;
}