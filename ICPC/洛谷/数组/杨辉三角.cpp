#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int a[21][21] = {};
    for (int i = 0; i < n; i++) {
        a[i][0] = a[i][i] = 1;  // 两端为1
        for (int j = 1; j < i; j++)
            // 填写中间的数
            a[i][j] = a[i-1][j-1] + a[i-1][j]; 
        // 输出第i行
        for (int j = 0; j <= i; j++) {
            if (j > 0) cout << " ";
                cout << a[i][j];
        }
        cout << "\n";
    }
    return 0;
}