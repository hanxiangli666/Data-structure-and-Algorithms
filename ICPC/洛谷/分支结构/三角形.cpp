#include <bits/stdc++.h>
using namespace std;

int main() {
    int a, b, c;
    cin >> a >> b >> c;
    
    // 第一步：能否构成三角形
    if ((a + b <= c) || (b + c <= a) || (a + c <= b)) {
        cout << "Not triangle" << endl;
        return 0;  // 直接结束，后面不用判断了
    }
    
    // 第二步：角的类型（三选一，用 if-else）
    int maxSide = max({a, b, c});
    int sumSq = a*a + b*b + c*c - maxSide*maxSide; // 两短边平方和
    
    if (sumSq == maxSide*maxSide) {
        cout << "Right triangle" << endl;
    } else if (sumSq > maxSide*maxSide) {
        cout << "Acute triangle" << endl;
    } else {
        cout << "Obtuse triangle" << endl;
    }
    
    // 第三步：等腰？（独立 if）
    if (a == b || b == c || a == c) {
        cout << "Isosceles triangle" << endl;
    }
    
    // 第四步：等边？（独立 if）
    if (a == b && b == c) {
        cout << "Equilateral triangle" << endl;
    }
    
    return 0;
}