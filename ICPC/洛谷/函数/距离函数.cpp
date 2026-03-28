#include <bits/stdc++.h>
using namespace std;

// 计算两点之间的欧氏距离
// sqrt() 和 pow() 都在 <cmath> 里，bits/stdc++.h 已经全部包含了
double dist(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int main() {
    double x1, y1, x2, y2, x3, y3;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    double perimeter = dist(x1, y1, x2, y2)
                     + dist(x2, y2, x3, y3)
                     + dist(x1, y1, x3, y3);

    // fixed + setprecision(2) 表示：用定点小数格式，保留两位小数
    cout << fixed << setprecision(2) << perimeter << endl;

    return 0;
}