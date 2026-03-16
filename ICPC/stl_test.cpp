#include <iostream>
using namespace std;

cin >> n;
vector<int> a(n);
for (int i = 0; i < n; i++) 
{
    cin >> a[i];
}

sort(a.begin(), a.end());
a.erase(unique(a.begin(), a.end()), a.end());

for (int i = 0; i < n; i++)
{
    cout << a[i] << endl;
}