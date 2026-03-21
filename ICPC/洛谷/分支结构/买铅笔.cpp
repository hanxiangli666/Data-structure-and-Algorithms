#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int ans = INT_MAX;  // 先设一个极大值，方便后面取最小
    
    for (int i = 0; i < 3; i++) {
        int cnt, price;
        cin >> cnt >> price;
        
        int packs = (n + cnt - 1) / cnt; // 需要买几包？（向上取整）
        int cost  =  price * packs; // 总花费
        
        ans = min(ans, cost);
    }
    
    cout << ans << endl;
    return 0;
}