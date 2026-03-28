#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

int main() {
    // 建立单词到数字的映射
    string words[] = {
        "zero","one","two","three","four","five","six","seven","eight","nine",
        "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
        "seventeen","eighteen","nineteen","twenty",
        "a","both","another","first","second","third"
    };
    int vals[] = {
        0,1,2,3,4,5,6,7,8,9,
        10,11,12,13,14,15,16,17,18,19,20,
        1,2,1,1,2,3
    };
    
    vector<string> nums;  // 存所有两位数字符串
    
    string word;
    while (cin >> word) {
        if (word == ".") break;
        // 转小写，方便比较
        for (char& c : word) c = tolower(c);
        // 查找是否是数字单词
        for (int i = 0; i < 27; i++) {
            if (word == words[i]) {
                int sq = (vals[i] * vals[i]) % 100;
                // 转成两位字符串
                string s = "";
                s += (char)('0' + sq/10);
                s += (char)('0' + sq%10);
                nums.push_back(s);
                break;
            }
        }
    }
    
    if (nums.empty()) { cout << 0; return 0; }
    
    // 对所有两位数字符串排序，找最小拼接
    // 排序规则：a+b < b+a 时，a排前面
    sort(nums.begin(), nums.end(), [](const string& a, const string& b) {
        return a+b < b+a;
    });
    
    // 拼接
    string result = "";
    for (string& s : nums) result += s;
    
    // 去前导零
    int start = 0;
    while (start < result.size()-1 && result[start] == '0') start++;
    result = result.substr(start);
    
    cout << result << "\n";
    return 0;
}