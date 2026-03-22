#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int n;
    int arr[100];
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    sort(arr, arr + n);
    cout << arr[n-1] - arr[0] << "\n";

    return 0;
}