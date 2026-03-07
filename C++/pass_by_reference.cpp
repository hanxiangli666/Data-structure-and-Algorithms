#include <iostream>
using namespace std;
// 以我的经验，如果是传递基本类型，比如 int、bool 等，用传值比较多，因为这类数据一般不需要在函数内部修改，而且复制的开销很小。

// 如果是传递容器数据结构，比如 vector、unordered_map 等，用传引用比较多，因为可以避免复制数据副本的开销，而且容器一般需要在函数内部修改。

// 特别注意一个可能出现的问题，就是当递归函数的参数中有容器数据结构时，千万别使用传值的方式，否则每次递归都会创建一个数据副本，消耗大量的内存和时间，非常容易导致超时或者超内存的错误。
void modifyReference(int &x) {
    x = 10;  // 修改原始数据
}

int main() {
    int num = 5;
    modifyReference(num);
    // 输出：10
    cout << "After modifyReference, num = " << num << endl;
    return 0;
}