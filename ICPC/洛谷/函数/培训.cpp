#include <bits/stdc++.h>
using namespace std;

struct Student {
    string name;
    int age;
    int score;
};

// 参数传入一个学生，返回培训后的学生
Student train(Student s) {
    s.age += 1;
    s.score = min(s.score * 6 / 5, 600); // 乘以 6/5 即提升 20%，不超过 600
    return s;
}

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        Student s;
        cin >> s.name >> s.age >> s.score;

        Student result = train(s);

        cout << result.name << " "
             << result.age  << " "
             << result.score << "\n";
    }

    return 0;
}