#include <bits/stdc++.h>
using namespace std;

int main() {
    int x;
    cin >> x;
    
    if ((x == 1) || (x == 0)) {
        cout << "Today, I ate " << x << " apple." << "\n";
    } else {
        cout << "Today, I ate " << x << " apples." << "\n";
    }
    
    return 0;
}