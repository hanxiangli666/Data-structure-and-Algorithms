#include <bits/stdc++.h>
using namespace std;

struct Student {
    string name;
    int chinese, math, english;
    
    int total() const {          // 计算总分的成员函数
        return chinese + math + english;
    }
};

int main() {
    int n;
    cin >> n;

    Student best;               // 记录当前最厉害的学生
    int bestTotal = -1;         // 记录当前最高总分

    for (int i = 0; i < n; i++) {
        Student s;
        cin >> s.name >> s.chinese >> s.math >> s.english;

        if (s.total() > bestTotal) {  // 严格大于：相同分数保留靠前的
            bestTotal = s.total();
            best = s;
        }
    }

    cout << best.name << " "
         << best.chinese << " "
         << best.math << " "
         << best.english << "\n";

    return 0;
}