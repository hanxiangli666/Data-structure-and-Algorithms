#include <bits/stdc++.h>
using namespace std;

// 计算一个选手的得分：去掉最高分和最低分后求平均
double calcScore(vector<int>& scores) {
    int total = 0;
    int maxVal = scores[0];
    int minVal = scores[0];

    for (int s : scores) {
        total += s;
        maxVal = max(maxVal, s);
        minVal = min(minVal, s);
    }

    int m = scores.size();
    return (double)(total - maxVal - minVal) / (m - 2);
}

int main() {
    int n, m;
    cin >> n >> m;

    double best = -1.0;

    for (int i = 0; i < n; i++) {
        vector<int> scores(m);
        for (int j = 0; j < m; j++) {
            cin >> scores[j];
        }
        double s = calcScore(scores);
        best = max(best, s);
    }

    cout << fixed << setprecision(2) << best << "\n";

    return 0;
}