#include <iostream>
using namespace std;

int main() {
    int a;
    cin >> a;

    int day = 1;
    while (a > 1) {
        day++;
        a /= 2;
    }
    cout << day << "\n";

    return 0;
}