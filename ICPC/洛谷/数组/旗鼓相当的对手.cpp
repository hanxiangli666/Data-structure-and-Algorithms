#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int a[1001][3];  // a[i][0]=语文, a[i][1]=数学, a[i][2]=英语
    for (int i = 0; i < n; i++)
        cin >> a[i][0] >> a[i][1] >> a[i][2];
    
    int cnt = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            int sumI = a[i][0] + a[i][1] + a[i][2];
            int sumJ = a[j][0] + a[j][1] + a[j][2];
            // 判断i和j是否旗鼓相当
            if (abs(a[i][0]-a[j][0]) <= 5 &&
                abs(a[i][1]-a[j][1]) <= 5 &&
                abs(a[i][2]-a[j][2]) <= 5 &&
                abs(sumI - sumJ) <= 10) {
                cnt++;
                }

        }
    }
    cout << cnt << "\n";
    return 0;
}