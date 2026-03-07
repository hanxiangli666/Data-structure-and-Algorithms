#include <iostream>
#include <unordered_set>
using namespace std;

int main() {
    // 初始化哈希集合
    unordered_set<int> hashset{1, 2, 3, 4};

    // 检查哈希集合是否为空，输出：0 (false)
    cout << hashset.empty() << endl;

    // 获取哈希集合的大小，输出：4
    cout << hashset.size() << endl;

    // 查找指定元素是否存在
    // 输出：Element 3 found.
    if (hashset.contains(3)) {
        cout << "Element 3 found." << endl;
    } else {
        cout << "Element 3 not found." << endl;
    }

    // 插入一个新的元素
    hashset.insert(5);

    // 删除一个元素
    hashset.erase(2);
    // 输出：Element 2 not found.
    if (hashset.contains(2)) {
        cout << "Element 2 found." << endl;
    } else {
        cout << "Element 2 not found." << endl;
    }

    // 遍历哈希集合
    // 输出（顺序可能不同）：
    // 1
    // 3
    // 4
    // 5
    for (const auto &element : hashset) {
        cout << element << endl;
    }

    return 0;
}