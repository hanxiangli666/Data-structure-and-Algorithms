#include <bits/stdc++.h>
using namespace std;

int val[300];
int tree[300];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    int total = 1 << n;

    for (int i = 1; i <= total; i++) cin >> val[i];

    for (int i = 1; i <= total; i++) tree[total + i - 1] = i;

    for (int i = total - 1; i >= 1; i--) {
        int left  = tree[2 * i];
        int right = tree[2 * i + 1];
        tree[i] = (val[left] > val[right]) ? left : right;
    }

    // 亚军就是决赛（根节点）的另一侧，即 tree[2] 和 tree[3] 中不是冠军的那个
    int champion = tree[1];
    int runner_up = (tree[2] == champion) ? tree[3] : tree[2];

    cout << runner_up << endl;
    return 0;
}