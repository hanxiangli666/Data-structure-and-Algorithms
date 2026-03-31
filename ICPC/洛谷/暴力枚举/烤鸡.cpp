#include <bits/stdc++.h>
using namespace std;

int n;
int a[11];                        // a[1]~a[10] 存每种配料的克数
vector<array<int,10>> ans;        // 存所有合法方案

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    // 10层暴力枚举，每层1~3
    for(a[1]=1; a[1]<=3; a[1]++)
    for(a[2]=1; a[2]<=3; a[2]++)
    for(a[3]=1; a[3]<=3; a[3]++)
    for(a[4]=1; a[4]<=3; a[4]++)
    for(a[5]=1; a[5]<=3; a[5]++)
    for(a[6]=1; a[6]<=3; a[6]++)
    for(a[7]=1; a[7]<=3; a[7]++)
    for(a[8]=1; a[8]<=3; a[8]++)
    for(a[9]=1; a[9]<=3; a[9]++)
    for(a[10]=1; a[10]<=3; a[10]++){
        int sum = 0;
        for(int i = 1; i <= 10; i++) sum += a[i];
        if(sum == n){
            array<int,10> tmp;
            for(int i = 0; i < 10; i++) tmp[i] = a[i+1];
            ans.push_back(tmp);
        }
    }

    cout << ans.size() << "\n";
    for(auto& row : ans){
        for(int i = 0; i < 10; i++){
            cout << row[i];
            if(i < 9) cout << " ";
        }
        cout << "\n";
    }

    return 0;
}