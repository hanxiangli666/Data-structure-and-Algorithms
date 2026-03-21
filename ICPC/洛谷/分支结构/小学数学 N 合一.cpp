#include<iostream>
#include<cmath>
using namespace std;
int main() {
    int T;
    cin >> T;
    if (T == 1) {
        cout << "I love Luogu!";

    } else if (T == 2) {
        cout << 2 + 4 << " " << 10 - 2 - 4;

    } else if (T == 3) {
        // 每人得到14/4=3，分出去3*4=12，放回14-12=2
        cout << 14/4 << "\n" << (14/4)*4 << "\n" << 14%4;

    } else if (T == 4) {
        // 500/3，保留6位有效数字，不用科学计数法
        printf("%.3f", 500.0/3);  // 166.667 是6位有效数字

    } else if (T == 5) {
        // 两车相向，车头相遇后，还需要走完两车总长度
        // 相对速度 = 12+20 = 32 m/s
        // 总长度 = 260+220 = 480 m
        // 时间 = 480/32 = 15 s
        cout << (260+220)/(12+20);

    } else if (T == 6) {
        // 对角线 = sqrt(6*6 + 9*9)
        cout << sqrt(6.0*6 + 9.0*9);

    } else if (T == 7) {
        // 100 存10 花20 取出
        int money = 100;
        money += 10;  cout << money << "\n";
        money -= 20;  cout << money << "\n";
        money = 0;    cout << money << "\n";

    } else if (T == 8) {
        // r=5, π=3.141593
        // 周长=2πr, 面积=πr², 球体积=4/3*πr³
        double pi = 3.141593, r = 5;
        cout << 2*pi*r << "\n";
        cout << pi*r*r << "\n";
        cout << 4.0/3*pi*r*r*r << "\n";

    } else if (T == 9) {
        // 从第4天往回推
        // 第4天剩1个
        // 第3天：(x/2)-1=1 → x=4
        // 第2天：(x/2)-1=4 → x=10
        // 第1天：(x/2)-1=10 → x=22
        cout << 22;

    } else if (T == 10) {
        // 设队列初始任务数为Q，单位时间新增任务数为r
        // 8台30分钟刚好完成：8*30 = Q + r*30
        // 10台6分钟刚好完成：10*6  = Q + r*6
        // 解方程：240 = Q+30r，60 = Q+6r
        // 相减：180 = 24r → r=7.5，Q=60-6*7.5=15
        // 设x台10分钟完成：10x = Q+10r = 15+75 = 90 → x=9
        cout << 9;

    } else if (T == 11) {
        // 小B追小A：相对速度=8-5=3 m/s，距离100m
        // 时间=100/3
        cout << 100.0/3;

    } else if (T == 12) {
        // M是第13个字母：'M'-'A'+1=13
        // 第18个字母：'A'+17='R'
        cout << ('M'-'A'+1) << "\n" << (char)('A'+17);

    } else if (T == 13) {
        // 两球体积之和 = 4/3*π*(4³+10³) = 4/3*π*1064
        // 正方体体积=球体积之和，棱长=cbrt(体积)，向下取整
        double pi = 3.141593;
        double vol = 4.0/3*pi*(4.0*4*4 + 10.0*10*10);
        cout << (int)cbrt(vol);

    } else if (T == 14) {
        // 暴力枚举
        for (int price = 1; price <= 110; price++) {
        int people = 10 + (110 - price);  // 降价越多，人越多
        if (price * people == 3500) {
        cout << price;
        break;
        }
        }
    }
    return 0;
}