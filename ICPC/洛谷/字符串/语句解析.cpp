#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cin >> s;
    
    int val[3] = {};  // val[0]=a, val[1]=b, val[2]=c
    
    for (int i = 0; i < s.size(); i += 5) {
        int left = s[i] - 'a';   // 0/1/2 对应 a/b/c
        char right = s[i+3];
        
        if (right >= '0' && right <= '9') {
            // 右边是数字
            val[left] = right - '0';
        } else {
            // 右边是变量
            val[left] = val[right - 'a'];
        }
    }
    
    cout << val[0] << " " << val[1] << " " << val[2] << "\n";
    return 0;
}