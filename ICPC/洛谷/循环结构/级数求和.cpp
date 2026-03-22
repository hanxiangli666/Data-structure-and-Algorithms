#include <iostream>
using namespace std;

int main() {
    int k;
    cin >> k;
    
    double sum = 0;
    int n = 0;
    while (sum <= k) {
        // n++，sum加上1.0/n
        n++;
        sum += 1.0 / n;
    }
    
    cout << n << "\n";
    return 0;
}