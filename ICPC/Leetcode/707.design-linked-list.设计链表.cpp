/*
 * @lc app=leetcode.cn id=707 lang=cpp
 * @lcpr version=30400
 *
 * [707] 设计链表
 */

// @lc code=start
class MyLinkedList {
private:
    ListNode *dummy; // 哨兵头节点，本身不存数据，统一所有操作的前驱查找逻辑
    int size;        // 链表有效节点数

public:
    // 初始化：创建哨兵节点，链表为空
    MyLinkedList() {
        dummy = new ListNode(0);
        size = 0;
    }

    // 获取第 index 个节点的值
    // 思路：从 dummy->next 出发走 index 步即到达目标节点
    int get(int index) {
        if (index < 0 || index >= size) return -1; // 越界直接返回 -1
        ListNode *cur = dummy->next;
        for (int i = 0; i < index; i++) cur = cur->next;
        return cur->val;
    }

    // 头部插入：等价于在 index=0 处插入
    void addAtHead(int val) {
        addAtIndex(0, val);
    }

    // 尾部插入：等价于在 index=size 处插入
    void addAtTail(int val) {
        addAtIndex(size, val);
    }

    // 在第 index 个节点前插入新节点
    // 思路：找到第 index-1 个节点（前驱），在其后接入新节点
    //       dummy 作为起点，走 index 步即到前驱（index=0 时前驱是 dummy 本身）
    void addAtIndex(int index, int val) {
        if (index > size) return;      // index 超出范围，不操作
        if (index < 0) index = 0;      // index 为负数时等同于头部插入
        ListNode *prev = dummy;
        for (int i = 0; i < index; i++) prev = prev->next;
        ListNode *node = new ListNode(val);
        node->next = prev->next;       // 新节点指向原 index 位置节点
        prev->next = node;             // 前驱指向新节点
        size++;
    }

    // 删除第 index 个节点
    // 思路：找到前驱节点，跳过目标节点并释放内存
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size) return; // 越界不操作
        ListNode *prev = dummy;
        for (int i = 0; i < index; i++) prev = prev->next;
        ListNode *del = prev->next;    // 待删除节点
        prev->next = del->next;        // 前驱直接指向后继
        delete del;                    // 释放内存，避免内存泄漏
        size--;
    }
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */
// @lc code=end



/*
// @lcpr case=start
// ["MyLinkedList","addAtHead","addAtTail","addAtIndex","get","deleteAtIndex","get"]\n[[],[1],[3],[1,2],[1],[1],[1]]\n
// @lcpr case=end

 */
