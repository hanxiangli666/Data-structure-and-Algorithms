#include <iostream>
#include <string>
using namespace std;

// 0-9每个数字的5行点阵
string digit[10][5] = {
    {"XXX","X.X","X.X","X.X","XXX"},  // 0
    {"..X","..X","..X","..X","..X"},  // 1
    {"XXX","..X","XXX","X..","XXX"},  // 2
    {"XXX","..X","XXX","..X","XXX"},  // 3
    {"X.X","X.X","XXX","..X","..X"},  // 4
    {"XXX","X..","XXX","..X","XXX"},  // 5
    {"XXX","X..","XXX","X.X","XXX"},  // 6
    {"XXX","..X","..X","..X","..X"},  // 7
    {"XXX","X.X","XXX","X.X","XXX"},  // 8
    {"XXX","X.X","XXX","..X","XXX"},  // 9
};

int main() {
    int n;
    string s;
    cin >> n >> s;
    
    for (int row = 0; row < 5; row++) {
        for (int i = 0; i < n; i++) {
            if (i > 0) cout << ".";  // 数字间隔
            cout << digit[s[i]-'0'][row];
        }
        cout << "\n";
    }
    return 0;
}