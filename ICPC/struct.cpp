#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Student
{
    string name;
    double score;
};

int main()
{
    int n;
    cin >> n;
    vector<Student> students(n);
    for (int i = 0; i < n; i++)
    {
        cin >> students[i].name >> students[i].score;
    }

    sort(students.begin(), students.end(), [](const Student &a, const Student &b) {
        if (a.score != b.score)
            return a.score > b.score;
        else
            return a.name < b.name;
    });

    for (int i = 0; i < n; i++)
    {
        cout << students[i].name << " " << students[i].score << endl;
    }
    return 0;
}
