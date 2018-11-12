#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <numeric>

using namespace std;

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2) return;

    vector<typename RandomIt::value_type> left(range_begin, range_begin + (range_end - range_begin) / 2);
    vector<typename RandomIt::value_type> right(range_begin + (range_end - range_begin) / 2, range_end);
    MergeSort(left.begin(), left.end());
    MergeSort(right.begin(), right.end());

    merge(left.begin(), left.end(), right.begin(), right.end(), range_begin);
}

int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}
