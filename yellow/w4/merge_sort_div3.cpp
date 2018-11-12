#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <numeric>

using namespace std;

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2) return;

    int partitionSize = (range_end - range_begin) / 3;

    vector<typename RandomIt::value_type> left(range_begin, range_begin + partitionSize);
    vector<typename RandomIt::value_type> middle(range_begin + partitionSize, range_begin + 2 * partitionSize);
    vector<typename RandomIt::value_type> right(range_begin + 2 * partitionSize, range_end);

    MergeSort(left.begin(), left.end());
    MergeSort(middle.begin(), middle.end());
    MergeSort(right.begin(), right.end());

    vector<typename RandomIt::value_type> aux;
    merge(left.begin(), left.end(), middle.begin(), middle.end(), back_inserter(aux));
    merge(aux.begin(), aux.end(), right.begin(), right.end(), range_begin);
}

int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 2};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}
