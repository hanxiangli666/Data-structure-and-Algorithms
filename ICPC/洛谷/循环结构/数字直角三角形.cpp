#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int num = 1;  // 当前数字
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < n-i+1; j++) {
            // 输出num，两位补零，然后num++
            printf("%02d", num);
            num++;
        }
        cout << "\n";
    }
    return 0;
}