#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    long long a, b, c;
    cin >> a >> b >> c;
    
    // 找最大值（斜边）
    long long hyp = max({a, b, c});
    
    // 找较短的直角边
    // 提示：三个数之和减去斜边，再减去较长直角边
    // 或者用 min
    long long leg = min({a, b, c});
    
    // 约分并输出
    long long g = __gcd(leg, hyp);
    leg /= g;
    hyp /= g;
    cout << leg << "/" << hyp << "\n";
    return 0;
}