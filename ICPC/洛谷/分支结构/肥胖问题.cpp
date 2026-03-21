#include <iostream>
using namespace std;

int main() {
    double m, h;
    cin >> m >> h;
    
    double bmi = m / (h * h);  // 填公式
    
    if (bmi < 18.5) {
        cout << "Underweight\n";
    } else if (18.5 <= bmi && bmi < 24) {
        cout << "Normal\n";
    } else {
        cout << bmi << "\n"
             << "Overweight\n";
        // 先输出bmi，再输出Overweight
        
    }
    
    return 0;
}