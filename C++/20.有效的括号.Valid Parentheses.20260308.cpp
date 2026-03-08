/*
 * @lc app=leetcode.cn id=20 lang=cpp
 * @lcpr version=30400
 *
 * [20] 有效的括号
 */
#include <iostream>
#include <stack>
using namespace std;
// @lc code=start
class Solution {
public:
    bool isValid(string str) {
        stack<char> left;
        for (char c : str) {
            if (c == '(' || c == '{' || c == '[') {
                left.push(c);
            } else {
                if (!left.empty() && leftOf(c) == left.top()) {
                    left.pop();
                } else {
                    return false;
                }
            }
        }
        return left.empty();
    }

    char leftOf(char c) {
        switch (c) {
            case ')': return '(';
            case '}': return '{';
            case ']': return '[';
            default: return 0;
        }
    }
};
// @lc code=end



/*
// @lcpr case=start
// "()"\n
// @lcpr case=end

// @lcpr case=start
// "()[]{}"\n
// @lcpr case=end

// @lcpr case=start
// "(]"\n
// @lcpr case=end

// @lcpr case=start
// "([])"\n
// @lcpr case=end

// @lcpr case=start
// "([)]"\n
// @lcpr case=end

 */

