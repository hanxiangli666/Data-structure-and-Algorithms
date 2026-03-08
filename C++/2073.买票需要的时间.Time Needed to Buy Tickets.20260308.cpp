/*
 * @lc app=leetcode.cn id=2073 lang=cpp
 * @lcpr version=30400
 *
 * [2073] 买票需要的时间
 */
#include <vector>
#include <queue>
using namespace std;
// @lc code=start
class Solution {
public:
    int timeRequiredToBuy(vector<int>& tickets, int k) {
        // 用队列模拟整个过程
        // 初始化队列，存储每个人的编号 id
        queue<int> queue;
        for (int i = 0; i < tickets.size(); i++) {
            queue.push(i);
        }

        int time = 0;
        while (!queue.empty()) {
            // 队头的人买票
            int front = queue.front();
            queue.pop();
            time++;
            tickets[front]--;
            
            if (front == k && tickets[front] == 0) {
                // 如果是 k 号买完票了，返回总时间
                return time;
            }

            if (tickets[front] == 0) {
                continue;
            }

            // 如果还要继续买票，重新排到队尾
            queue.push(front);
        }
        return time;
    }
};
// @lc code=end



/*
// @lcpr case=start
// [2,3,2]\n2\n
// @lcpr case=end

// @lcpr case=start
// [5,1,1,1]\n0\n
// @lcpr case=end

 */

