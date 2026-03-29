#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p1, p2, p3;
    cin >> p1 >> p2 >> p3;

    string s;
    cin >> s;

    string result;

    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] != '-') {
            result += s[i];
            continue;
        }

        // 当前字符是'-'，检查能否展开
        // 需要左右两侧都存在
        if (i == 0 || i == (int)s.size() - 1) {
            result += '-';
            continue;
        }

        char left  = s[i - 1];
        char right = s[i + 1];

        // 判断两侧是否同为字母或同为数字，且右边严格大于左边
        bool both_letters = islower(left) && islower(right);
        bool both_digits  = isdigit(left) && isdigit(right);

        if ((!both_letters && !both_digits) || right <= left) {
            // 不满足展开条件，原样保留'-'
            result += '-';
            continue;
        }

        if (right == left + 1) {
            // 右边恰好是左边的后继，只删减号，不填充
            continue;
        }

        // 需要填充 left+1 到 right-1 的字符
        // 根据p3决定遍历方向
        int start = left + 1;
        int end   = right - 1; // 填充范围（包含两端）

        // 生成填充序列的起点、终点、步长
        int from = (p3 == 1) ? start : end;
        int to   = (p3 == 1) ? end   : start;
        int step = (p3 == 1) ? 1     : -1;

        for (int c = from; c != to + step; c += step) {
            char fill_char;
            if (p1 == 3) {
                fill_char = '*';
            } else if (both_digits) {
                fill_char = (char)c; // 数字直接用
            } else {
                // both_letters
                if (p1 == 1) fill_char = (char)c;              // 小写
                else         fill_char = (char)(c - 'a' + 'A'); // 大写
            }
            // 每个字符重复p2次
            for (int repeat = 0; repeat < p2; repeat++) {
                result += fill_char;
            }
        }
    }

    cout << result << "\n";
    return 0;
}