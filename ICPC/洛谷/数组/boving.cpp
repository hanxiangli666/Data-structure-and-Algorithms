#include <iostream>
using namespace std;

int main() {
    int s1, s2, s3;
    cin >> s1 >> s2 >> s3;
    
    int cnt[100] = {};
    
    for (int i = 1; i <= s1; i++)
        for (int j = 1; j <= s2; j++)
            for (int k = 1; k <= s3; k++)
                cnt[i+j+k]++;
    
    // 找cnt最大值
    int maxCnt = 0;
    for (int i = 0; i < 100; i++)
        maxCnt = max(maxCnt, cnt[i]);
    
    // 找最大值对应的最小下标
    for (int i = 0; i < 100; i++) {
        if (cnt[i] == maxCnt) {
            cout << i << "\n";
            break;  // 找到最小的就停
        }
    }
    return 0;
}