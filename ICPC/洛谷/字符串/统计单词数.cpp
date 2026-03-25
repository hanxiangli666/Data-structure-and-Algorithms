#include <bits/stdc++.h>
using namespace std;

int main() {
    string word, article;
    cin >> word;
    cin.ignore();
    getline(cin, article);

    // 全部转小写
    for (char& c : word) c = tolower(c);
    for (char& c : article) c = tolower(c);

    int cnt = 0, firstPos = -1;
    int wLen = word.size();
    int aLen = article.size();

    int start = 0;
    while (true) {
        int pos = article.find(word, start);  // 从start位置开始找
        if (pos == string::npos) break;       // 找不到了就退出

        // 验证边界：确保是独立单词，不是某个单词的一部分
        bool leftOK  = (pos == 0 || article[pos - 1] == ' ');
        bool rightOK = (pos + wLen == aLen || article[pos + wLen] == ' ');

        if (leftOK && rightOK) {
            cnt++;
            if (firstPos == -1) firstPos = pos;  // 记录第一次出现位置
        }

        start = pos + 1;  // 从下一个位置继续找，避免死循环
    }

    if (cnt == 0) cout << -1 << "\n";
    else cout << cnt << " " << firstPos << "\n";

    return 0;
}
