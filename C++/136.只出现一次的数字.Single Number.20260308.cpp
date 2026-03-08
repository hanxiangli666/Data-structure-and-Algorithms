/*
 * @lc app=leetcode.cn id=136 lang=cpp
 * @lcpr version=30400
 *
 * [136] 只出现一次的数字
 */
#include <vector>
#include <unordered_map>
using namespace std;
// @lc code=start
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_map<int, int> count;
        // 遍历数组，统计每个数字出现的次数
        for (int num : nums) {
            count[num]++;
        }
        for (int num : nums) {
            if (count[num] == 1) {
                return num;
            }
        }
        return -1; // 题目保证一定存在一个只出现一次的数字
    }
};
// @lc code=end



/*
// @lcpr case=start
// [2,2,1]\n
// @lcpr case=end

// @lcpr case=start
// [4,1,2,1,2]\n
// @lcpr case=end

// @lcpr case=start
// [1]\n
// @lcpr case=end

 */

