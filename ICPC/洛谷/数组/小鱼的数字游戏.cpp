#include <iostream>
using namespace std;

int main() {
    int a[101], n = 0;
    int x;
    while (cin >> x && x != 0) {
        a[n++] = x;
    }
    
    // 倒序输出
    for (int i = n - 1; i >= 0; i--) {
        if (i > 0) cout << a[i] << " ";
        else cout << a[i]<< "\n";
    }
    return 0;
}