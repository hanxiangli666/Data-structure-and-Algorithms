#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    int a[101];
    for (int i = 0; i < n; i++) cin >> a[i];
    
    for (int i = 0; i < n; i++) {
        int cnt = 0;
        // 遍历左边所有鱼，统计比a[i]小的
        for (int j = 0; j < i; j++) {
            if (a[j] < a[i]) cnt++;
        }
        if (i < n-1) cout << cnt << " ";
        else cout << cnt << "\n";
    }
    return 0;
}