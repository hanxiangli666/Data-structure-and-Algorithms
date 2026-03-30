#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
int L[MAXN], R[MAXN];   // L[i]表示i的左邻居，R[i]表示i的右邻居
bool deleted[MAXN];      // 标记是否被删除

// 在节点 pos 右边插入节点 x
void insertRight(int pos, int x) {
    R[x] = R[pos];       // x的右边 = pos原来的右边
    L[x] = pos;          // x的左边 = pos
    L[R[pos]] = x;       // pos原右邻居的左边 = x
    R[pos] = x;          // pos的右边 = x
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // 初始化：0是虚拟头，n+1是虚拟尾
    R[0] = 1; L[1] = 0;     // 头 → 1号
    R[1] = n + 1; L[n + 1] = 1; // 1号 → 尾

    // 2~n号同学依次入列
    for (int i = 2; i <= n; i++) {
        int k, p;
        cin >> k >> p;
        if (p == 1) {
            insertRight(k, i);   // 插到k右边
        } else {
            insertRight(L[k], i); // 插到k左边 = 插到k的左邻居右边
        }
    }

    // 删除操作
    int m;
    cin >> m;
    while (m--) {
        int x;
        cin >> x;
        if (deleted[x]) continue; // 已删除，忽略
        deleted[x] = true;
        L[R[x]] = L[x];  // x右邻居的左指针跳过x
        R[L[x]] = R[x];  // x左邻居的右指针跳过x
    }

    // 从虚拟头出发，遍历输出
    bool first = true;
    for (int cur = R[0]; cur != n + 1; cur = R[cur]) {
        if (deleted[cur]) continue; // 其实已删除的不会出现在链表里，双重保险
        if (!first) cout << " ";
        cout << cur;
        first = false;
    }
    cout << "\n";

    return 0;
}
