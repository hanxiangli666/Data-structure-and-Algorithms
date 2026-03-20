#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, int> mp;

    // 插入：直接用 [] 赋值
    mp["apple"] = 3;
    mp["banana"] = 5;
    mp["cherry"] = 2;

    // size：键值对数量
    cout << mp.size() << "\n";   // 3

    // empty：是否为空
    cout << mp.empty() << "\n";  // 0（false，不为空）

    // [] 访问：根据键取值
    cout << mp["apple"] << "\n"; // 3

    // ⚠️ 危险：访问不存在的键会自动创建，size 会变大！
    cout << mp["grape"] << "\n"; // 0（自动创建了键 grape，值为 0）
    cout << mp.size() << "\n";   // 4（grape 被悄悄插入了）

    // count：判断键是否存在（返回 0 或 1），访问前先用这个检查
    if (mp.count("apple")) {
        cout << mp["apple"] << "\n";  // 3，安全
    }

    if (!mp.count("durian")) {
        cout << "durian 不存在\n";
    }

    // erase：删除某个键
    mp.erase("banana");
    cout << mp.size() << "\n";   // 3（banana 被删除）

    // 确认删除
    if (!mp.count("banana")) {
        cout << "banana 已删除\n";
    }

    // 遍历：auto& [key, val] 自动拿到键和值
    // ⚠️ unordered_map 遍历顺序是随机的，不保证按插入顺序
    for (auto& [key, val] : mp) {
        cout << key << " -> " << val << "\n";
    }

    // clear：清空所有键值对（unordered_map 有 clear()！）
    mp.clear();
    if (mp.empty()) {
        cout << "map 已空\n";
    }

    return 0;
}
