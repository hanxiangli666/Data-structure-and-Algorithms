#include <iostream>
using namespace std;

void modifyValue(int x) {
    x = 10;  // 只修改副本，不会影响原始数据
}

int main() {
    int num = 5;
    modifyValue(num);
    // 输出：5
    cout << "After modifyValue, num = " << num << endl;
    return 0;
}