#include <bits/stdc++.h>
using namespace std;

struct Student {
    string name;
    int chinese, math, english;
    
    int total() const {
        return chinese + math + english;
    }
};

// 判断两名学生是否旗鼓相当
bool isMatch(const Student& a, const Student& b) {
    return abs(a.chinese - b.chinese) <= 5 &&
           abs(a.math    - b.math)    <= 5 &&
           abs(a.english - b.english) <= 5 &&
           abs(a.total() - b.total()) <= 10;
}

int main() {
    int n;
    cin >> n;

    vector<Student> students(n);
    for (int i = 0; i < n; i++) {
        cin >> students[i].name
            >> students[i].chinese
            >> students[i].math
            >> students[i].english;
    }

    // 双层循环枚举所有对，i < j 保证不重复且顺序正确
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (isMatch(students[i], students[j])) {
                cout << students[i].name << " "
                     << students[j].name << "\n";
            }
        }
    }

    return 0;
}