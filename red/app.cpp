#include <string>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <cstring>
#include <utility>
#include <vector>
#include <list>

using namespace std;

int main() {
    list<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    cout << v.back() << " " << v.front() << endl;

    for (auto const & i : v)
        std::cout << i << " " ;

}