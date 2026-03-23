#include <iostream>
using namespace std;

int main() {
    long long M, N;
    cin >> M >> N;
    
    long long cnt[10] = {};  // cnt[0]~cnt[9]
    
    for (long long i = M; i <= N; i++) {
        long long num = i;
        while (num > 0) {
            cnt[num % 10]++;
            num /= 10;
        }
    }
    
    for (int i = 0; i < 10; i++) {
        if (i > 0) cout << " ";
        cout << cnt[i];
    }
    cout << "\n";
    return 0;
}