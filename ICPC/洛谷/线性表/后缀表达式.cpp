#include <bits/stdc++.h>
using namespace std;

int main() {
    stack<long long> st;
    char c;

    while (cin.get(c)) { // 逐字符读入
        if (c == '@') {
            // 结束，输出答案
            cout << st.top() << "\n";
            break;
        } else if (isdigit(c)) {
            // 开始读一个完整的数，直到遇到 '.'
            long long num = 0;
            while (c != '.') {
                num = num * 10 + (c - '0');
                cin.get(c);
            }
            st.push(num);
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // 取出栈顶两个数，注意顺序！
            long long b = st.top(); st.pop(); // 后入栈的是右操作数
            long long a = st.top(); st.pop(); // 先入栈的是左操作数
            if      (c == '+') st.push(a + b);
            else if (c == '-') st.push(a - b);
            else if (c == '*') st.push(a * b);
            else               st.push(a / b);
        }
        // 其他字符（比如换行）直接忽略
    }

    return 0;
}