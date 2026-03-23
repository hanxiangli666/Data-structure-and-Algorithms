#include <iostream>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    int a[3001];
    for (int i = 0; i < n; i++) cin >> a[i];
    
    int minSum = 1e9;
    for (int i = 0; i <= n-m; i++) {
        int sum = 0;
        // 计算从i开始的m个数之和
        for (int j = i; j < i + m; j++) {
            sum += a[j];
        }
        
        minSum = min(minSum, sum);
    }
    
    cout << minSum << "\n";
    return 0;
}