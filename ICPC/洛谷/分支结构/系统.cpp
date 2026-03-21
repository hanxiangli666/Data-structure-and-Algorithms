#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int local  = 5 * n;   // 填
    int luogu  = 3 * n + 11;   // 填
    
    if (local < luogu) {         // 填条件
        cout << "Local\n";
    } else {
        cout << "Luogu\n";
    }
    
    return 0;
}