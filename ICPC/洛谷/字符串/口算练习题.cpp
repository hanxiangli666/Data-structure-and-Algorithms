#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    cin.ignore();  // 吃掉第一行末尾的换行符，否则getline会读到空行

    char op = 'a';  // 记录上一次的运算类型

    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);       // 读取整行
        stringstream ss(line);   // 装进流里方便解析

        string first;
        ss >> first;  // 先读第一个词

        int a, b;
        if (first == "a" || first == "b" || first == "c") {
            // 三个数据：第一个是运算类型
            op = first[0];
            ss >> a >> b;
        } else {
            // 两个数据：第一个就是第一个运算数
            a = stoi(first);  // string转int
            ss >> b;
        }

        // 计算结果 & 确定符号
        int result;
        char sign;
        if (op == 'a') { result = a + b; sign = '+'; }
        else if (op == 'b') { result = a - b; sign = '-'; }
        else { result = a * b; sign = '*'; }

        // 拼出完整算式字符串
        string expr = to_string(a) + sign + to_string(b) + "=" + to_string(result);

        cout << expr << "\n";
        cout << expr.size() << "\n";
    }

    return 0;
}