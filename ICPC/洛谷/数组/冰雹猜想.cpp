#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    int a[10001], cnt = 0;
    a[cnt++] = n;

    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
            a[cnt++] = n;
        } else {
            n = n * 3 + 1;
            a[cnt++] = n;
        }
    }
    for (int i = cnt - 1; i >= 0; i--) {
        if (i > 0) cout << a[i] << " ";
        else cout << a[i] << "\n";
    }
    return 0;
}