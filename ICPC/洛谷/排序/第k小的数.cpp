#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count()); // 随机数生成器

// partition: 将 a[left..right] 以 a[left] 为 pivot 分成两半
// 返回 pivot 最终落的位置
int partition(vector<int>& a, int left, int right) {
    int pivot = a[left];
    int i = left, j = right;

    while (i < j) {
        while (i < j && a[j] >= pivot) j--;  // 从右找第一个小于 pivot 的
        while (i < j && a[i] <= pivot) i++;  // 从左找第一个大于 pivot 的
        if (i < j) swap(a[i], a[j]);
    }
    // 循环结束时 i == j，这就是 pivot 应该在的位置
    swap(a[left], a[i]);
    return i;
}

// 在 a[left..right] 中找第 k 小（0-indexed）
int quickSelect(vector<int>& a, int left, int right, int k) {
    // 随机选 pivot，交换到 left 位置
    int rand_index = left + rng() % (right - left + 1);
    swap(a[left], a[rand_index]);

    int p = partition(a, left, right);  // pivot 落在位置 p

    if (p == k) return a[p];
    else if (p > k) return quickSelect(a, left, p - 1, k);   // 答案在左半边
    else            return quickSelect(a, p + 1, right, k);  // 答案在右半边
}

int main() {
    ios::sync_with_stdio(false);  // 关闭 C/C++ 输入输出同步，cin 变快
    cin.tie(nullptr);             // 解绑 cin 和 cout，进一步加速

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    cout << quickSelect(a, 0, n - 1, k) << endl;

    return 0;
}