#include <iostream>
using namespace std;

int main() {
    int y, m;
    cin >> y >> m;
    
    bool leap = (y%4==0 && y%100!=0) || y%400==0;
    
    switch (m) {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            cout << 31 << "\n";
            break;
        case 4: case 6: case 9: case 11:
            cout << 30 << "\n";
            break;
        case 2:
            if (leap) cout << 29 << "\n";
            else cout << 28 << "\n";
            break;
    }
    
    return 0;
}