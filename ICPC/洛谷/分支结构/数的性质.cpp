#include <iostream>
using namespace std;

int main() {
    int x;
    cin >> x;
    
    bool p1 = (x % 2 == 0);  // 填条件
    bool p2 = (x > 4) && (x <= 12);  // 填条件
    
    cout << (p1 && p2) << " "
         << (p1 || p2) << " "
         << (p1 != p2) << " "
         << (!p1 && !p2) << "\n";
    
    return 0;
}