#include <iostream>
#include <unordered_map>
using namespace std;
// unordered_map 是 C++ 标准库中的一种哈希表实现，它提供了基于键值对（key-value）的存储，提供了常数时间复杂度的查找、插入和删除键值对的操作。
int main() {
    // 初始化一个空的哈希表 map
    unordered_map<int, string> hashmap;

    // 初始化一个包含一些键值对的哈希表 map2
    unordered_map<int, string> hashmap2{{1, "one"}, {2, "two"}, {3, "three"}};
}