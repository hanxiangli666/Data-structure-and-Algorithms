#include <iostream>
#include <list>
using namespace std;
// 一般来说，当我们想在头部增删元素时会使用双链表，因为它在头部增删元素的效率比 vector 高。但我们通过索引访问元素，这种场景下我们会使用 vector
int main() {
    int n = 7;

    // 初始化一个空的双向链表 lst
    std::list<int> lst;

    // 初始化一个大小为 n 的链表 lst2`，链表中的值默认都为 0
    std::list<int> lst2(n);

    // 初始化一个包含元素 1, 3, 5 的链表 lst3
    std::list<int> lst3{1, 3, 5};

    // 初始化一个大小为 n 的链表 lst4，其中值都为 2
    std::list<int> lst4(n, 2);
}