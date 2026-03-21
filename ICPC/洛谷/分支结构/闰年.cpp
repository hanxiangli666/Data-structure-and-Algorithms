#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    bool leap = (n % 4 == 0 && n %100 !=0) || (n % 400 == 0);
    cout << leap <<"\n";
    return 0;
}