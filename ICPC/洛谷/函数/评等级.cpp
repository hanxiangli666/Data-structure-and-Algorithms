#include <bits/stdc++.h>
using namespace std;

struct Student {
    int id;
    int academic;    // 学业成绩
    int quality;     // 素质拓展成绩
    double score;    // 综合分数

    // 成员函数：返回两科总分
    int total() const {
        return academic + quality;
    }
};

// 判断是否优秀
bool isExcellent(const Student& s) {
    bool totalOk = s.total() > 140;
    bool scoreOk = s.academic * 7 + s.quality * 3 >= 800; // 避免浮点精度问题
    return totalOk && scoreOk;
}

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        Student s;
        cin >> s.id >> s.academic >> s.quality;
        s.score = s.academic * 0.7 + s.quality * 0.3; // 存储用浮点没问题，比较时不用它

        if (isExcellent(s)) {
            cout << "Excellent\n";
        } else {
            cout << "Not excellent\n";
        }
    }

    return 0;
}