#include <iostream>
#include <vector>
using namespace std;
// 因为根据「数组」的特性，利用索引访问元素很高效，从尾部增删元素也是很高效的；而从中间或头部增删元素要涉及搬移数据，很低效。
int main() {
    int n = 7, m = 8;

    // 初始化一个 int 型的空数组 nums1
    vector<int> nums1;

    // 初始化一个大小为 n 的数组 nums2，数组中的值默认都为 0
    vector<int> nums2(n);

    // 初始化一个元素为 1, 3, 5 的数组 nums3
    vector<int> nums3{1, 3, 5};

    // 初始化一个大小为 n 的数组 nums4，其值全都为 2
    vector<int> nums4(n, 2);

    // 初始化一个二维 int 数组 dp
    vector<vector<int>> dp;

    // 初始化一个大小为 m * n 的布尔数组 dp2，
    // 其中的值都初始化为 true
    vector<vector<bool>> dp2(m, vector<bool>(n, true));
}