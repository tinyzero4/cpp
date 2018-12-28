#include "test_runner.h"
#include <algorithm>
#include <utility>
#include <iterator>

using namespace std;


#define PRINT(items) \
    for (auto &item : items) cout << item << " "; \
    cout << endl; \


template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    const long size = distance(range_begin, range_end);

    if (size < 2) return;
    vector<typename RandomIt::value_type> items(make_move_iterator(range_begin), make_move_iterator(range_end));

    const long split_size = size / 3;

    MergeSort(range_begin, next(range_begin, split_size));
    MergeSort(next(range_begin, split_size), next(range_begin, 2 * split_size));
    MergeSort(next(range_begin, 2 * split_size), range_end);

    vector<typename RandomIt::value_type> tmp;
    merge(
            make_move_iterator(range_begin), make_move_iterator(next(range_begin, split_size)),
            make_move_iterator(next(range_begin, split_size)), make_move_iterator(next(range_begin, 2 * split_size)),
            back_inserter(tmp)
    );
    merge(
            make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()),
            make_move_iterator(next(range_begin, 2 * split_size)), make_move_iterator(range_end),
            range_begin
    );

}

void TestIntVector() {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}