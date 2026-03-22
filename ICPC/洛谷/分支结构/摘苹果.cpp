#include <iostream>
using namespace std;

int main() {
    int apples[10];
    for (int i = 0; i < 10; i++) cin >> apples[i];
    int count = 0;
    int h;
    cin >> h;
    for (int i = 0; i < 10; i++) {
        if (apples[i] <= h + 30) count++;
    }
    cout << count << "\n";
    return 0;
}