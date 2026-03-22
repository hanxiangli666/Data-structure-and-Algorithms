#include <iostream>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    
    int sumA = 0, cntA = 0;
    int sumB = 0, cntB = 0;
    
    for (int i = 1; i <= n; i++) {
        if (i % k == 0) {
            cntA++;
            sumA += i;
        } else {
            cntB++;
            sumB += i;
        }
        
    }
    
    printf("%.1f %.1f\n", (double)sumA/cntA, (double)sumB/cntB);
    return 0;
}