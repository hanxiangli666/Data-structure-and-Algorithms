#include <iostream>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    int M = N / 52;  // 每周存入总量
    
    for (int X = 100; X >= 1; X--) {
        // 计算K，判断是否合法
        int K = (M/7 - X) / 3;
        if (K > 0 && (X + 3 * K == M/7)) {
            cout << X << "\n"
            << K << "\n";
            break;
        }
    }
    
    return 0;
}