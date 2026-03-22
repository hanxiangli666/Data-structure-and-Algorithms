#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    int a[10001];
    for (int i = 0; i < n; i++) cin >> a[i];
    
    int maxLen = 1, curLen = 1;
    for (int i = 1; i < n; i++) {
        // 判断是否连号，更新curLen和maxLen
        if (a[i] == a[i-1] + 1) {
            curLen++;
        } else {
            curLen = 1;  // 不连号就重置
        }
        maxLen = max(maxLen, curLen);  // 每次都更新最大值
    }
    
    cout << maxLen << "\n";
    return 0;
}