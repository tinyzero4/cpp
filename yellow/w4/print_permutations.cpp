#include "algorithm"
#include "vector"
#include "string"
#include "iostream"
#include <numeric>

using namespace std;

int main() {
    uint32_t n;
    cin >> n;

    vector<int> values;
    iota(values.begin(), values.end(), 1);

    vector<vector<int>> reverse(n);

    do {
        reverse.push_back(values);
    } while (std::next_permutation(values.begin(), values.end()));

    for (auto it = reverse.rbegin(); it != reverse.rend(); it++) {
        for (auto const &item : *it) {
            cout << item << " ";
        }
        cout << endl;
    }


    return 0;

}