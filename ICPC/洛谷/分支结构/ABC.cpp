#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

int main() {
    int arr[3];
    cin >> arr[0] >> arr[1] >> arr[2];
    sort(arr, arr+3);  // arr[0]=A, arr[1]=B, arr[2]=C
    
    string order;
    cin >> order;
    
    for (int i = 0; i < 3; i++) {
        // 根据order[i]是'A','B'还是'C'，输出对应的值
        cout << arr[order[i] - 'A'];
        if (i < 2) cout << " ";  // 数字之间加空格
    }
    cout << "\n";
    return 0;
}