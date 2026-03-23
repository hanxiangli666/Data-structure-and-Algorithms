#include <iostream>
using namespace std;

int main() {
    int l, m;
    cin >> l >> m;
    
    bool tree[10001];
    for (int i = 0; i <= l; i++) tree[i] = true;
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        // 把u到v的树标记为移走
        for (int j = u; j <= v; j++) {
            tree[j] = false;
        }
    }
    
    // 统计剩余树的数量
    int cnt = 0;
    for (int i = 0; i <= l; i++) {
        if (tree[i]) cnt++;
    }
    cout << cnt << "\n";
        
    
    return 0;
}