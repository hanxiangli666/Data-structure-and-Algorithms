#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    // 正方形
    int num1 = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%02d", num1++);
        }
        cout << "\n";
    }
    
    cout << "\n";  // 空行
    int num2 = 1;
    // 三角形
    for (int i = 1; i <= n; i++) {
        // 先输出前导空格，再输出i个数
        for (int k = 0; k < (n - i) * 2; k++) cout << " ";
        for (int j = 0; j < i; j++) printf("%02d", num2++);
        cout << "\n";
    }
    
    return 0;
}