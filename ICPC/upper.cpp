#include <iostream>
#include <string>
using namespace std;

int main()
{
    string line;
    getline(cin, line);

    int count = 0;
    for (char c : line)
    {
        if (isupper(c))
        {
            count++;
        }
    }
    cout << count << endl;
    return 0;
}