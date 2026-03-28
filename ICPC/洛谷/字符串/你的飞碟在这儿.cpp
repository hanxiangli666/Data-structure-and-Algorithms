#include <iostream>
#include <string>
using namespace std;

int main() {
    string a, b;
    cin >> a >> b;

    long long pa = 1, pb = 1;
    for (char c : a) pa *= (c - 'A' + 1);
    for (char c : b) pb *= (c - 'A' + 1);

    cout << (pa%47 == pb%47 ? "GO" : "STAY") << "\n";
    return 0;

}