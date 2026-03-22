#include <iostream>
using namespace std;

int main() {
    double s;
    cin >> s;
    
    double sum = 0, dist = 2.0;
    int steps = 0;
    
    while (sum < s) {
        // 累加距离，步数+1，更新下一步距离
        sum += dist;
        steps++;
        dist *= 0.98;
    }
    
    cout << steps << "\n";
    return 0;
}