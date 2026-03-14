/*
 * @lc app=leetcode.cn id=707 lang=cpp
 * @lcpr version=30400
 *
 * [707] 设计链表
 */

#include <algorithm>
#include <iostream>
using namespace std;

// Definition for singly-linked list node.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// @lc code=start
class MyLinkedList {
public:
    MyLinkedList() {
        this->size = 0;
        this->head = new ListNode(0);
    }
    
    int get(int index) {
        if (index < 0 || index >= size) {
            return -1;
        }
        ListNode *cur = head;
        for (int i = 0; i <= index; i++) {
            cur = cur->next;
        }
        return cur->val;
    }
    
    void addAtHead(int val) {
        addAtIndex(0, val);
    }
    
    void addAtTail(int val) {
        addAtIndex(size, val);
    }
    
    void addAtIndex(int index, int val) {
        if (index > size) {
            return;
        }
        index = max(0, index);
        size++;
        ListNode *pred = head;
        for (int i = 0; i < index; i++) {
            pred = pred->next;
        }
        ListNode *toAdd = new ListNode(val);
        toAdd->next = pred->next;
        pred->next = toAdd;
    }
    
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size) {
            return;
        }
        size--;
        ListNode *pred = head;
        for (int i = 0; i < index; i++) {
            pred = pred->next;
        }
        ListNode *p = pred->next;
        pred->next = pred->next->next;
        delete p;
    }
private:
    int size;
    ListNode *head;
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

int main() {
    MyLinkedList* obj = new MyLinkedList();
    obj->addAtHead(1);
    obj->addAtTail(3);
    obj->addAtIndex(1, 2);   // 链表变为 1->2->3
    cout << obj->get(1) << endl;     // 输出 2
    obj->deleteAtIndex(1);           // 链表变为 1->3
    cout << obj->get(1) << endl;     // 输出 3
    delete obj;
    return 0;
}



/*
// @lcpr case=start
// ["MyLinkedList","addAtHead","addAtTail","addAtIndex","get","deleteAtIndex","get"]\n[[],[1],[3],[1,2],[1],[1],[1]]\n
// @lcpr case=end

 */

