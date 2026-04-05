#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    queue<int> memory;        // 模拟内存，队头是最早进来的
    bool inMemory[1001] = {}; // 标记某个单词是否在内存中
    int count = 0;            // 查词典次数

    for (int i = 0; i < n; i++) {
        int word;
        cin >> word;

        if (inMemory[word]) {
            // 内存里有，不查词典
            continue;
        }

        // 内存里没有，查词典
        count++;

        if ((int)memory.size() == m) {
            // 内存满了，踢掉最早进来的
            inMemory[memory.front()] = false;
            memory.pop();
        }

        // 存入新词
        memory.push(word);
        inMemory[word] = true;
    }

    cout << count << "\n";
    return 0;
}