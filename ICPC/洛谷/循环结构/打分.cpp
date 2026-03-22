#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int sum = 0, maxVal = 0, minVal = 10;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        // 更新sum、maxVal、minVal
        sum += x;
        maxVal = max(maxVal, x);
        minVal = min(minVal, x);
    }
    // 计算并输出
    printf("%.2f\n", (double)(sum - maxVal - minVal) / (n - 2));
    return 0;
}