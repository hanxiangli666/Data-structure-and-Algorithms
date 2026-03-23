#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    int a[101];
    bool exist[20001] = {};
    
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        exist[a[i]] = true;
    }
    
    int cnt = 0;
    bool counted[20001] = {};  // 防止重复计数
    
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            int sum = a[i] + a[j];
            // 判断sum是否在集合中且未被计数过
            if (exist[sum] && !counted[sum]) {
                cnt++;
                counted[sum] = true;
            }
        }
    }
    
    cout << cnt << "\n";
    return 0;
}