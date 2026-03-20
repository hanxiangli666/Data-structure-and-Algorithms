#include <iostream>
#include <climits>  // INT_MAX 需要这个头文件
using namespace std;

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
private:
    int minDepthValue = INT_MAX;
    int currentDepth = 0;

public:
    int minDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        traverse(root);
        return minDepthValue;
    }

    void traverse(TreeNode* root) {
        if (root == nullptr) return;

        // 前序位置：进入节点
        currentDepth++;
        cout << "进入节点 " << root->val 
             << "，当前深度 = " << currentDepth << endl;

        // 判断是否叶子节点
        if (root->left == nullptr && root->right == nullptr) {
            cout << "  → 节点 " << root->val 
                 << " 是叶子节点！记录深度 = " << currentDepth << endl;
            minDepthValue = min(minDepthValue, currentDepth);
        }

        traverse(root->left);
        traverse(root->right);

        // 后序位置：离开节点
        cout << "离开节点 " << root->val 
             << "，深度从 " << currentDepth << " 恢复到 " << currentDepth - 1 << endl;
        currentDepth--;
    }
};

int main() {
    // 手动构建这棵树：
    //     3
    //    / \
    //   9  20
    //     /  \
    //    15   7

    TreeNode* root = new TreeNode(3);
    root->left        = new TreeNode(9);
    root->right       = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right= new TreeNode(7);

    Solution sol;
    int result = sol.minDepth(root);

    cout << "\n最终答案：最小深度 = " << result << endl;

    // 释放内存
    delete root->right->right;
    delete root->right->left;
    delete root->right;
    delete root->left;
    delete root;

    return 0;
}