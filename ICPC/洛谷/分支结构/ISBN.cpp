#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cin >> s;
    int sum = 0;
    int weight = 1;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '-') continue;
        if (weight > 9) break;
        sum += (s[i] - '0') * weight;
        weight++;
    }
    int check = sum % 11;
    char correct = (check == 10) ? 'X' : ('0' + check);
    if (s.back() == correct) cout << "Right" << endl;
    else {
        for (int i = 0; i < s.size() - 1; i++) {
            cout << s[i];
        }
        cout << correct;
    }

    return 0;
}