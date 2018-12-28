#include <string>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <cstring>
#include <utility>
#include <vector>

using namespace std;

int main() {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    std::back_insert_iterator<std::vector<int>>  it(v);

    std::cout << *it;
    it = 99;

    for (auto const & i : v)
        std::cout << i << " " ;

}