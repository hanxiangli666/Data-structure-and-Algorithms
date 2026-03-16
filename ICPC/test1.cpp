#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <numeric>
#include <cmath>
using namespace std;

typedef long long ll;
const int MOD = 1e9 + 7;

ll power(ll a, ll b, ll mod) {
    ll result = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) result = result * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    return 0;
}