#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "algorithm"

using namespace std;

template<typename T>
vector<T> FindGreaterElements(const set<T> &elements, const T &border) {
    auto found = find_if(elements.begin(), elements.end(), [&border](const auto &el)->bool { return el > border; });
    vector<T> res;
    if ((found != elements.end())) {
        for (auto item = found; found != elements.end(); found++)
            res.push_back(*found);
    }
    return res;
}


int main() {
    for (int x : FindGreaterElements(set<int>{1, 2, 4, 5, 6, 7}, 3)) {
        cout << x << " ";
    }
    cout << endl;

    string to_find = "Python";
    cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
    return 0;
}
