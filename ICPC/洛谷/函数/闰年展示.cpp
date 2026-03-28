#include <bits/stdc++.h>
using namespace std;

bool isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int main() {
    int x, y;
    cin >> x >> y;

    vector<int> leaps;

    for (int year = x; year <= y; year++) {
        if (isLeap(year)) {
            leaps.push_back(year);
        }
    }

    cout << leaps.size() << "\n";

    for (int i = 0; i < (int)leaps.size(); i++) {
        if (i > 0) cout << " ";
        cout << leaps[i];
    }
    cout << "\n";

    return 0;
}