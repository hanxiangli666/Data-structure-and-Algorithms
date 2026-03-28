#include <bits/stdc++.h>
using namespace std;

int win[5][5] = {
    { 0, -1,  1,  1, -1},
    { 1,  0, -1,  1, -1},
    {-1,  1,  0, -1,  1},
    {-1, -1,  1,  0,  1},
    { 1,  1, -1, -1,  0}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Na, Nb;
    cin >> N >> Na >> Nb;

    vector<int> a(Na), b(Nb);
    for (int i = 0; i < Na; i++) cin >> a[i];
    for (int i = 0; i < Nb; i++) cin >> b[i];

    int scoreA = 0, scoreB = 0;

    for (int i = 0; i < N; i++) {
        int ga = a[i % Na]; // A 第 i 轮出的手势
        int gb = b[i % Nb]; // B 第 i 轮出的手势

        int result = win[ga][gb]; // 从A视角看
        if (result == 1)       scoreA++;
        else if (result == -1) scoreB++;
        // result == 0 平局，都不加分
    }

    cout << scoreA << " " << scoreB << "\n";
    return 0;
}