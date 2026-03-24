#include <iostream>
#include <string>
using namespace std;

int n;
char a[11][11], b[11][11], tmp[11][11];

// 顺时针旋转90°，结果存到dst
void rot90(char src[][11], char dst[][11]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dst[j][n-1-i] = src[i][j];
}

// 水平翻转
void flip(char src[][11], char dst[][11]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dst[i][n-1-j] = src[i][j];
}

// 比较两个矩阵是否相同
bool equal(char x[][11], char y[][11]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (x[i][j] != y[i][j]) return false;
    return true;
}

int main() {
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];
    
    char tmp2[11][11];

    // 变换1：旋转90°
    rot90(a, tmp);
    if (equal(tmp, b)) { cout << 1; return 0; }

    // 变换2：旋转180°（90°再转90°）
    rot90(tmp, tmp2);
    if (equal(tmp2, b)) { cout << 2; return 0; }

    // 变换3：旋转270°（180°再转90°）
    rot90(tmp2, tmp);
    if (equal(tmp, b)) { cout << 3; return 0; }

    // 变换4：水平翻转
    flip(a, tmp);
    if (equal(tmp, b)) { cout << 4; return 0; }

    // 变换5：翻转后再旋转1/2/3次
    flip(a, tmp);      // 先翻转存进tmp
    rot90(tmp, tmp2);  // 翻转+90°
    if (equal(tmp2, b)) { cout << 5; return 0; }

    rot90(tmp2, tmp);  // 翻转+180°
    if (equal(tmp, b)) { cout << 5; return 0; }

    rot90(tmp, tmp2);  // 翻转+270°
    if (equal(tmp2, b)) { cout << 5; return 0; }

    // 变换6：不变
    if (equal(a, b)) { cout << 6; return 0; }

    // 变换7：无法转换
    cout << 7;
    
    return 0;
}