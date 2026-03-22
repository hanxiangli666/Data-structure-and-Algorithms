#include <iostream>
using namespace std;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}


int main() {
    int L;
    cin >> L;
    int sum = 0, cnt = 0;
    for (int i = 2; sum + i <= L; i++) {
        if (isPrime(i)) {
            cout << i << "\n";
            sum += i;
            cnt++;
        }
    }
    cout << cnt << "\n";


    return 0;
}